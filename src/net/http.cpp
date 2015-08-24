#include "http.h"


#include "net/fossa.h"
#include "core/alarm.h"
#include "core/string.h"
#include "core/tuple.hpp"
#include "core/vector.hpp"


namespace {
    jle::signal<struct ns_connection*, int, struct http_message&>&  get_signal_http_msg()
    {
        static auto result = new jle::signal<struct ns_connection*, int, struct http_message&>;
        return *result;
    }

    static void ev_handler(struct ns_connection *nc, int ev, void *ev_data) {
      struct http_message *hm = (struct http_message *) ev_data;

      switch (ev) {
        case NS_HTTP_REQUEST:
            get_signal_http_msg().emit(nc, ev, *hm);
          break;
        default:
          break;
      }
    }

    std::string  ns_str2string(const struct ns_str& ns)
    {
        static char buf[1024];
        long unsigned size = sizeof(buf) - 1 < ns.len  ? sizeof(buf) - 1 : ns.len;
        memcpy(buf, ns.p, size);
        buf[size] = 0;
        return std::string(buf);
    }

    std::string get_current_time_response() {
        char buffer[100];
        time_t now = time(0);
        struct tm tstruct = *gmtime(&now);
        strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", &tstruct);

        return std::string(buffer);
    }

    //struct http_message {
    //  struct ns_str message; /* Whole message: request line + headers + body */
    //
    //  struct ns_str proto; /* "HTTP/1.1" -- for both request and response */
    //  /* HTTP Request line (or HTTP response line) */
    //  struct ns_str method; /* "GET" */
    //  struct ns_str uri;    /* "/my_file.html" */
    //  /* For responses, code and response status message are set */
    //  int resp_code;
    //  struct ns_str resp_status_msg;
    //
    //  /*
    //   * Query-string part of the URI. For example, for HTTP request
    //   *    GET /foo/bar?param1=val1&param2=val2
    //   *    |    uri    |     query_string     |
    //   *
    //   * Note that question mark character doesn't belong neither to the uri,
    //   * nor to the query_string
    //   */
    //  struct ns_str query_string;
    //
    //  /* Headers */
    //  struct ns_str header_names[NS_MAX_HTTP_HEADERS];
    //  struct ns_str header_values[NS_MAX_HTTP_HEADERS];
    //
    //  /* Message body */
    //  struct ns_str body; /* Zero-length for requests with no body */

    jle::map<std::string, std::string>  get_headers(struct http_message& hm)
    {
        auto result = jle::map<std::string, std::string>();
        for(int i=0; hm.header_names[i].p != 0; ++i) {
            result.insert(std::make_pair(
                            ns_str2string(hm.header_names[i]),
                            ns_str2string(hm.header_values[i])));
        }
        return result;
    }

    jle::tuple<std::string, jle::list<std::string> >
    get_cookies(const jle::map<std::string, std::string>& headers)
    {
        auto it_find = headers.find("Cookie");
        if(it_find == headers.cend())
            return std::make_tuple("", jle::list<std::string>());

        auto v_cookies = jle::s_split(it_find->second, "|");
        auto l_cookies = jle::list<std::string>();
        for(const auto& c : v_cookies) {
            l_cookies.push_back(c);
        }

        return std::make_tuple(it_find->second, l_cookies);
    }
};


namespace jle { namespace net { namespace http {




std::ostream& operator<<(std::ostream& out, const Map& m)
{
    out
        << "full: " << m.full << std::endl;
    for(auto it = m.map.cbegin(); it!= m.map.cend(); ++it) {
        out << "  " << it->first << ": " << it->second << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const List& m)
{
    out
        << "full: " << m.full << std::endl;
    out << "  [";
    for(auto it = m.list.cbegin(); it!= m.list.cend(); ++it) {
        out << *it << ", ";
    }
    out << " ]" << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Req_resp_line& line)
{
    out
        << "full: " << line.full << std::endl
        << "method: " << line.method << std::endl
        << "uri: " << line.uri << std::endl
        << "query_string: " << line.query_string << std::endl
        << "proto: " << line.proto << std::endl;
    return out;
}


std::ostream& operator<<(std::ostream& out, const Request_msg& rq)
{
    out
        << "full: " << rq.full << std::endl
        << "----------------------------------------" << std::endl
        << "----------------------------------------" << std::endl
        << "request line: " << rq.request_line << std::endl
        << "----------------------------------------" << std::endl
        << "headers: " << rq.headers << std::endl
        << "----------------------------------------" << std::endl
        << "cookies: " << rq.cookies << std::endl
        << "----------------------------------------" << std::endl
        << "body: " << rq.body << std::endl;
    return out;
}



Response_msg response_ok(const std::string& body) {
    return Response_msg {
        200,
        "OK",
        "text/html",
        get_current_time_response(),
        body
    };
}


Request::Request(struct ns_connection *_nc, const Request_msg& _rq, const std::string _listen_port)
:   request_msg(_rq),
    listen_port(_listen_port),
    nc(_nc),
    valid_server(true)
{}

void Request::send_response(const Response_msg& resp)
{
    if(valid_server == false)
        throw  jle::alarm(JLE_HERE, "Connection::send_response", JLE_SS("invalid server sending response... " << resp.body), jle::al::priority::critic);

    if(nc==0)
        throw  jle::alarm(JLE_HERE, "Connection::send_response", JLE_SS("connection canceled... " << resp.body), jle::al::priority::critic);

    //ns_printf(&_nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
    ns_printf(nc, "HTTP/1.1 %s %s\r\nTransfer-Encoding: %s\r\n\r\n",
                                JLE_SS(resp.code).c_str(),
                                resp.phrase.c_str(),
                                "chunked");
        //  alternative to chunked...
        ////append headers
        //sprintf(&header_buffer[strlen(header_buffer)], "Server: %s %s\r\n", SERVER_NAME, SERVER_VERSION);
        //sprintf(&header_buffer[strlen(header_buffer)], "Date: %s\r\n", res.date.c_str());
        //sprintf(&header_buffer[strlen(header_buffer)], "Content-Type: %s\r\n", res.type.c_str());
        //sprintf(&header_buffer[strlen(header_buffer)], "Content-Length: %zd\r\n", body_len);
        //                        value().type.c_str());
    ns_printf_http_chunk(nc, "%s", resp.body.c_str());
    ns_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
    nc = 0;
}

void    Request::invalidated_server()
{
    valid_server = false;
}


Server::Server(const std::string& _port)
    :
    mgr(new struct ns_mgr),
    port(_port)
{
    struct ns_connection *nc;
    ns_mgr_init(mgr.get(), NULL);
    nc = ns_bind(mgr.get(), port.c_str(), ev_handler);
    if (nc == NULL) {
        jle::alarm(JLE_HERE, "HttpServer::ctor", JLE_SS("Failed creating socket on port "<<_port), jle::al::priority::critic);
    }
    nc->user_data = (void*)(&port);
    ns_set_protocol_http_websocket(nc);

    JLE_CONNECT_THIS(get_signal_http_msg(), on_fossa_request);
}

Server::~Server()
{
    ns_mgr_free(mgr.get());
    signal_closing_server.notify();
}

void Server::check_messages(void)
{
    ns_mgr_poll(mgr.get(), 10);
}


void Server::on_fossa_request(struct ns_connection* _nc, int /*ev*/, struct http_message& hm) {
    if(*((std::string*)_nc->user_data) != port)
        return;

    Request_msg rq = parse_rq(hm);
    auto connection = jle::shared_ptr<Request>(new Request(_nc, rq, port));
    signal_closing_server.connect(connection.get(), &Request::invalidated_server);
    signal_request_received.notify(connection);

    //on_request(_nc, rq);

//        if (ns_vcmp(&hm.uri, "/hi") == 0) {
//            //ns_printf(&_nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//            ns_printf(&_nc, "HTTP/1.1 %s %s\r\nTransfer-Encoding: %s\r\n\r\n",
//                                        JLE_SS(res.value().code).c_str(),
//                                        res.value().phrase.c_str(),
//                                        "chunked");
//                //  alternative to chunked...
//                ////append headers
//                //sprintf(&header_buffer[strlen(header_buffer)], "Server: %s %s\r\n", SERVER_NAME, SERVER_VERSION);
//                //sprintf(&header_buffer[strlen(header_buffer)], "Date: %s\r\n", res.date.c_str());
//                //sprintf(&header_buffer[strlen(header_buffer)], "Content-Type: %s\r\n", res.type.c_str());
//                //sprintf(&header_buffer[strlen(header_buffer)], "Content-Length: %zd\r\n", body_len);
//                //                        value().type.c_str());
//            ns_printf_http_chunk(&_nc, "%s", res.value().body.c_str());
//            ns_send_http_chunk(&_nc, "", 0);  /* Send empty chunk, the end of response */
//        } else {
//            auto s_http_server_opts = ns_serve_http_opts();
//            s_http_server_opts.document_root = ".";
//            s_http_server_opts.enable_directory_listing = "yes";
//            ns_serve_http(&_nc, &hm, s_http_server_opts);  /* Serve static content */
//        }
}


Request_msg Server::parse_rq(struct http_message& hm)
{
    auto headers = get_headers(hm);
    auto cookies = get_cookies(headers);
    Request_msg result = {
        ns_str2string(hm.message),
        Req_resp_line {
            "--pending--",
            ns_str2string(hm.method),
            ns_str2string(hm.uri),
            Map{        //  query string
                ns_str2string(hm.query_string),
                jle::map<std::string, std::string>()
            },
            ns_str2string(hm.proto)
        },
        Map{    //  headers
            "--pending--",
            headers
        },
        List{    //  cookies
            std::get<0>(cookies),
            std::get<1>(cookies)
        },
        ns_str2string(hm.body)
    };

    return result;

}




}   // namespace http
}   // namespace  net
}   // namespace jle


