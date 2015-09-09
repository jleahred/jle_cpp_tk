#include "net/http_server.h"


void jle::alarm_msg(const jle::alarm& al)
{
    std::cout << al << std::endl;
}


void on_request_received(const jle::shared_ptr<jle::net::http::Request>& rq)
{
    static int i = 0;
    std::cout << "received request on port " << rq->listen_port << std::endl;
    std::cout << rq->request_msg << std::endl;
    rq->send_response(jle::net::http::response_ok(JLE_SS("All is OK " << ++i << " on port " << rq->listen_port)));
}

int main(int /*argc*/, char ** /*argv[]*/)
{
    auto start_server = [] (const std::string& port) {
        std::cout << "starting server on port " << port << std::endl;
        auto server = std::make_shared<jle::net::http::Server>(port);
        server->signal_request_received.connect(on_request_received);
        return server;
    };

    auto server1 = start_server("8000");
    auto server2 = start_server("8001");

    //for (;;) {
        server1->check_messages();
        server2->check_messages();
    //}

  return 0;
}

