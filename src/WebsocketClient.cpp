#include "WebsocketClient.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

utility_client::utility_client(): m_timeout(0)
{
	m_endpoint.set_access_channels(websocketpp::log::alevel::all);
	m_endpoint.clear_access_channels(websocketpp::log::alevel::frame_payload);
	m_endpoint.clear_access_channels(websocketpp::log::alevel::frame_header);

	// 初始化 ASIO
	m_endpoint.init_asio();

	// 注册消息回调
	m_endpoint.set_message_handler(std::bind(&utility_client::on_message, this, &m_endpoint, ::_1, ::_2));
	m_endpoint.set_open_handler(std::bind(&utility_client::on_open, this, &m_endpoint, ::_1));
}

utility_client::~utility_client()
{
	m_endpoint.close(m_hdl, websocketpp::close::status::normal, "");
}

void utility_client::connect(std::string url)
{
	websocketpp::lib::error_code ec;
	client::connection_ptr con = m_endpoint.get_connection(url, ec);
	con->add_subprotocol("janus-protocol");
	if (ec)
	{
		std::cout << "create connection failed: " << ec.message() << std::endl;
		return;
	}

	m_hdl = con->get_handle();
	m_endpoint.connect(con);
}

void utility_client::on_open(client *c, websocketpp::connection_hdl hdl)
{
    std::string msg = "hello";
    c->send(hdl, msg, websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Tx: " + msg);
    
}

void utility_client::on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    // websocketpp::lib::error_code ec;
    // c->send(hdl,msg->get_payload(),msg->get_opcode(),ec);
    // if(ec)
    // {
    //     std::cout << "Echo failed because " << ec.message() << std::endl;
    // }
}

// 定时器回调函数
void utility_client::Timeout(client *c, websocketpp::connection_hdl &hdl, boost::asio::deadline_timer *pt, const boost::system::error_code &ec)
{
    if(ec)
    {
        std::cout << "timer is cancel " << std::endl;
        return;
    }

    // static int count = 0;
    m_endpoint.send(m_hdl, m_msg, websocketpp::frame::opcode::text);
    // count++;
    // if(count > 5)//定时器触发五次后关闭连接
    // {
    //     c->close(hdl, websocketpp::close::status::normal, "");
    //     return;
    // }

    pt->expires_at(pt->expires_at() + boost::posix_time::seconds(m_timeout));
    pt->async_wait(bind(&utility_client::Timeout, this, c, hdl, pt, ::_1));
}

void utility_client::set_timeout(const uint32_t sec)
{
    m_timeout = sec;
}

void utility_client::run()
{
	boost::asio::deadline_timer t(m_endpoint.get_io_service(), boost::posix_time::seconds(m_timeout)); //设置一个超时的定时器
	t.async_wait(bind(&utility_client::Timeout, this, &m_endpoint, m_hdl, &t, ::_1));
    m_pt = &t;
    m_endpoint.run();
}

void utility_client::close_timer()
{
    m_pt->cancel();
}

void utility_client::set_msg(std::string msg)
{
    m_msg = msg;
}
