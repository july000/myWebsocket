#ifndef WEBSOCKETCLIENT_H__
#define WEBSOCKETCLIENT_H__

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

class utility_client
{
public:
    utility_client();
    ~utility_client();

    void connect(std::string url);
    void on_open(client *c, websocketpp::connection_hdl hdl);
    void on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg);
    void Timeout(client *c, websocketpp::connection_hdl &hdl, boost::asio::deadline_timer *pt, const boost::system::error_code &ec);
    void set_timeout(const uint32_t sec);
    void close_timer();
    void set_msg(std::string msg);
    void run();

private:
	client m_endpoint;
    websocketpp::connection_hdl m_hdl;
    uint32_t m_timeout;
    std::string m_msg;
    boost::asio::deadline_timer* m_pt;
};

#endif
