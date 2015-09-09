#ifndef JLE_NET_HTTP_SERVER_H
#define JLE_NET_HTTP_SERVER_H

#include <string>
#include "core/map.hpp"
#include "core/signal_slot.hpp"
#include "core/list.hpp"


//!  \example  ./net/ex_http_server.cpp



struct ns_mgr;
struct ns_connection;
struct http_message;


namespace jle { namespace net { namespace http {


struct  Map {
    std::string  full;

    jle::map<std::string, std::string>  map;
};
std::ostream& operator<<(std::ostream& out, const Map& m);

struct  List {
    std::string  full;

    jle::list<std::string>  list;
};
std::ostream& operator<<(std::ostream& out, const List& m);

struct  Req_resp_line {
    std::string     full;

    std::string     method;
    std::string     uri;
    Map             query_string;
    std::string     proto;
};
std::ostream& operator<<(std::ostream& out, const Req_resp_line& line);

struct Request_msg {
    Request_msg() = delete;

    std::string     full;
    Req_resp_line   request_line;
    Map             headers;
    List            cookies;
    std::string     body;
};
std::ostream& operator<<(std::ostream& out, const Request_msg& rq);


struct Response_msg {
    Response_msg()=delete;

    int code;
    std::string phrase;
    std::string type;
    std::string date;
    std::string body;
};


Response_msg response_ok(const std::string& body);



class Server;

class Request : public jle::signal_receptor {
public:
    void send_response(const Response_msg& resp);
    const   Request_msg     request_msg;
    const   std::string     listen_port;

private:
    Request(struct ns_connection *_nc, const Request_msg& _rq, const std::string _listen_port);



    struct  ns_connection   *nc;
    bool                    valid_server;

    void    invalidated_server();

friend class Server;
};


class Server  :  public jle::signal_receptor  {

public:
    Server(const std::string& _port);

    ~Server();

    void check_messages(void);

    jle::signal<const jle::shared_ptr<Request>&>  signal_request_received;


private:
    jle::shared_ptr<struct ns_mgr>  mgr;
    std::string                     port;

    void on_fossa_request(struct ns_connection* _nc, int /*ev*/, struct http_message& hm);
    Request_msg parse_rq(struct http_message& hm);

    jle::signal<>  signal_closing_server;
};



}   // namespace http
}   // namespace  net
}   // namespace jle

#endif // JLE_NET_HTTP_SERVER_H
