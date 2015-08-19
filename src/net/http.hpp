#include "net/fossa.h"
#include <iostream>

#include "core/alarm.h"
#include "core/signal_slot.hpp"
#include "core/map.hpp"
#include "core/string.h"


void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << al << std::endl;
}


//static void handle_sum_call(struct ns_connection * /*nc*/, struct http_message * /*hm*/) {
//  char n1[100], n2[100];
//  double result;
//
//  /* Get form variables */
//  ns_get_http_var(&hm->body, "n1", n1, sizeof(n1));
//  ns_get_http_var(&hm->body, "n2", n2, sizeof(n2));
//
//  /* Send headers */
//  ns_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//
//  /* Compute the result and send it back as a JSON object */
//  result = strtod(n1, NULL) + strtod(n2, NULL);
//  ns_printf_http_chunk(nc, "{ \"result\": %lf }", result);
//  ns_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
//}


//static void ev_handler(struct ns_connection *nc, int ev, void *ev_data) {
//  struct http_message *hm = (struct http_message *) ev_data;
//
//  switch (ev) {
//    case NS_HTTP_REQUEST:
//      if (ns_vcmp(&hm->uri, "/api/v1/sum") == 0) {
//        handle_sum_call(nc, hm);                    /* Handle RESTful call */
//      } else if (ns_vcmp(&hm->uri, "/printcontent") == 0) {
//        char buf[100] = {0};
//        memcpy(buf, hm->body.p,
//               sizeof(buf) - 1 < hm->body.len? sizeof(buf) - 1 : hm->body.len);
//        printf("%s\n", buf);
//      } else if (ns_vcmp(&hm->uri, "/hi") == 0) {
//        ns_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//          ns_printf_http_chunk(nc, "{ \"result\":  }%s", "aa");
//          ns_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
//      } else {
//          auto s_http_server_opts = ns_serve_http_opts();
//          s_http_server_opts.document_root = ".";
//          s_http_server_opts.enable_directory_listing = "yes";
//        ns_serve_http(nc, hm, s_http_server_opts);  /* Serve static content */
//      }
//      break;
//    default:
//      break;
//  }
//}


jle::signal<struct ns_connection&, int, struct http_message&>&  get_signal_http_msg()
{
    static auto result = new jle::signal<struct ns_connection&, int, struct http_message&>;
    return *result;
}


static void ev_handler(struct ns_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case NS_HTTP_REQUEST:
        get_signal_http_msg().emit(*nc, ev, *hm);
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


struct  Map {
    std::string  full;

    jle::map<std::string, std::string>  map;
};


struct Query_string {
    std::string     full;
    Map             params;
};

struct  Req_resp_line {
    std::string     full;

    std::string     method;
    std::string     uri;
    Query_string    query_string;
    std::string     proto;
};



struct Request {
    Request() = delete;

    std::string     full;
    Req_resp_line   request_line;
    Map             headers;
    Map             cookies;
    std::string     body;
};

class Http_server  :  public jle::signal_receptor  {

public:
    Http_server(const std::string& _port)
        : port(_port)
    {
        struct ns_connection *nc;
        ns_mgr_init(&mgr, NULL);
        nc = ns_bind(&mgr, port.c_str(), ev_handler);
        if (nc == NULL) {
            std::cerr << "Error starting server on port " << port << std::endl;
            exit(1);
        }
        ns_set_protocol_http_websocket(nc);

        JLE_CONNECT_THIS(get_signal_http_msg(), on_request);

        std::cout << "starting http server on port " << port << std::endl;
        std::cout << "nc... " << nc->sock << std::endl;
    }

    ~Http_server()
    {
        ns_mgr_free(&mgr);
    }

    void check_messages(void)
    {
        ns_mgr_poll(&mgr, 10);
    }


private:
    struct ns_mgr   mgr;
    std::string     port;

    void on_request(const Request& rq) {
        std::cout << rq.message << std::endl;

        std::cout << std::endl << std::endl
         << rq.method  << std::endl;
    }

    void on_request(struct ns_connection& _nc, int /*ev*/, struct http_message& hm) {
        if(_nc.mgr->ctl[0] != mgr.ctl[0]  ||  _nc.mgr->ctl[1] != mgr.ctl[1])
            return;

        Request rq = Request{
            "", //  method
            "", //  uri
            "", //  params
            jle::map<std::string, std::string>(),   // headers
            jle::map<std::string, std::string>(),   // query
            jle::map<std::string, std::string>(),   //  cookies
            ns_str2string(hm.message)   //  full message
        };

        parse_rq(hm, rq);

        on_request(rq);

        if (ns_vcmp(&hm.uri, "/hi") == 0) {
            ns_printf(&_nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
            ns_printf_http_chunk(&_nc, "{ \"result\":  }%s", "aa");
            ns_send_http_chunk(&_nc, "", 0);  /* Send empty chunk, the end of response */
        } else {
            auto s_http_server_opts = ns_serve_http_opts();
            s_http_server_opts.document_root = ".";
            s_http_server_opts.enable_directory_listing = "yes";
            ns_serve_http(&_nc, &hm, s_http_server_opts);  /* Serve static content */
        }

        /* Send headers */
//        ns_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//        ns_printf_http_chunk(nc, "{ \"result\": %s }", "aaa");
//        ns_send_http_chunk(nc, "", 0);  /* Send empty chunk, the end of response */
    }


    void parse_rq(struct http_message& hm, Request& req)
    {
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
        //};

//        jle::vector<std::string> hl = jle::s_split(jle::s_trim(req.message, " \n\r"), "\n");
//        req.method = hl[0];

//            // Parse request headers
//            int i = 0;
//            char * pch;
//            for (pch = strtok(headers.c_str(), "\n"); pch; pch = strtok(NULL, "\n" ))
//            {
//                if(i++ == 0)  {
//                    vector<string> R;
//                    string line(pch);
//                    this->split(line, " ", 3, &R);
//
//        //            cout << R.size() << endl;
//
//                    if(R.size() != 3) {
//        //                throw error
//                    }
//
//                    req->method = R[0];
//                    req->path = R[1];
//
//                    size_t pos = req->path.find('?');
//
//                    // We have GET params here
//                    if(pos != string::npos)  {
//                        vector<string> Q1;
//                        this->split(req->path.substr(pos + 1), "&", -1, &Q1);
//
//                        for(vector<string>::size_type q = 0; q < Q1.size(); q++) {
//                            vector<string> Q2;
//                            this->split(Q1[q], "=", -1, &Q2);
//
//                            if(Q2.size() == 2) {
//                                req->query[Q2[0]] = Q2[1];
//                            }
//                        }
//
//                        req->path = req->path.substr(0, pos);
//                    }
//                } else {
//                    vector<string> R;
//                    string line(pch);
//                    this->split(line, ": ", 2, &R);
//
//                    if(R.size() == 2) {
//                        req->headers[R[0]] = R[1];
//
//                        // Yeah, cookies!
//                        if(R[0] == "Cookie") {
//                            vector<string> C1;
//                            this->split(R[1], "; ", -1, &C1);
//
//                            for(vector<string>::size_type c = 0; c < C1.size(); c++) {
//                                vector<string> C2;
//                                this->split(C1[c], "=", 2, &C2);
//
//                                req->cookies[C2[0]] = C2[1];
//                            }
//                        }
//                    }
//                }
//            }
    }
};


int main(int /*argc*/, char ** /*argv[]*/)
{
    Http_server  server1("8000");
    Http_server  server2("8001");
    for (;;) {
        server1.check_messages();
        server2.check_messages();
    }

  return 0;
}

 
