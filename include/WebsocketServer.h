#ifndef WEBSOCKETSERVER_H__
#define WEBSOCKETSERVER_H__

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

//typedef struct {
//    websocketpp::connection_hdl hdl;
//    server::message_ptr msg;
//} cli_handler;

class utility_server
{
public:
    utility_server();
    ~utility_server();
    void recv_handler(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg);
	void open_handler(server* s, websocketpp::connection_hdl hdl);
	void close_handler(server* s, websocketpp::connection_hdl hdl);
    void run();
    void flip_msg(bool latch);
    void resp();
	float hz();
    uint32_t get_clis_size();

private:
    server m_endpoint;
    int32_t m_port;
    std::string m_msg_running;
    std::string m_msg_pending;
	float m_msg_hz;
    bool m_msg_switch; // true running; false pending
    // std::vector<cli_handler> m_clis;
	std::vector<websocketpp::connection_hdl> m_clis;
};

#endif