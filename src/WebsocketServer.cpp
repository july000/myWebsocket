#include "WebsocketServer.h"
#include "Config.h"
#include "nlohmann/json.hpp"

// Server
utility_server::utility_server()
{
    rr::RrConfig config;
    config.ReadConfig("config.ini");
    m_port = config.ReadInt("WebsocketServer", "Port", 9002);
    m_msg_running = config.ReadString("WebsocketServer", "Msg_SimOneCaseRunning", "True");
    m_msg_pending = config.ReadString("WebsocketServer", "Msg_SimOneCasePending", "False");
    m_msg_hz = config.ReadFloat("WebsocketServer", "Msg_Hz", 1.0);
    m_msg_switch = false;

    // 设置log
    m_endpoint.set_error_channels(websocketpp::log::elevel::all);
    m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

    // 初始化Asio
    m_endpoint.init_asio();

    // 设置消息回调为echo_handler
    m_endpoint.set_message_handler(std::bind(&utility_server::recv_handler, this, &m_endpoint, std::placeholders::_1, std::placeholders::_2));
	m_endpoint.set_open_handler(std::bind(&utility_server::open_handler, this, &m_endpoint, std::placeholders::_1));
	m_endpoint.set_close_handler(std::bind(&utility_server::close_handler, this, &m_endpoint, std::placeholders::_1));

	//void set_open_handler(open_handler h)
	//void set_close_handler(close_handler h)
}

utility_server::~utility_server()
{
	std::vector<websocketpp::connection_hdl>::iterator cli_iter = m_clis.begin();
	for (; cli_iter != m_clis.end();)
	{
		cli_iter = m_clis.erase(cli_iter);
	}
}

uint32_t utility_server::get_clis_size()
{
    return m_clis.size();
}

void utility_server::recv_handler(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    // 发送消息
    // m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
    // cli_handler cli; cli.hdl = hdl; cli.msg = msg; m_clis.emplace_back(cli);
	// m_clis.emplace_back(hdl);

    std::cout << "payload: " << msg->get_payload() << std::endl; // " opcode: " << msg->get_opcode() << std::endl;
    nlohmann::json j = nlohmann::json::parse(msg->get_payload().c_str());
    if(j.find("data") != j.end())
    {
        for(int i=0; i<j["data"].size(); i++)
        {
            if (j["data"][i].find("trace") != j["data"][i].end())
            {
                if (j["data"][i]["trace"].find("id") != j["data"][i]["trace"].end())
                {
                    if(j["data"][i]["trace"]["id"] == 260)
                    {
                        if(j["data"][i]["trace"].find("parsed") != j["data"][i]["trace"].end())
                        {
                            if(j["data"][i]["trace"]["parsed"].find("signals") != j["data"][i]["trace"]["parsed"].end())
                            {
                                for (int n=0; n<j["data"][i]["trace"]["parsed"]["signals"].size(); n++)
                                {
                                    if(j["data"][i]["trace"]["parsed"]["signals"].find("name") != j["data"][i]["trace"]["parsed"]["signals"].end())
                                    {
                                        std::string name_str = j["data"][i]["trace"]["parsed"]["signals"]["name"];
                                        if (name_str.compare("WH_Heckintervall") == 0)
                                        {
                                            if(j["data"][i]["trace"]["parsed"]["signals"].find("raw") != j["data"][i]["trace"]["parsed"]["signals"].end())
                                            {
                                                std::string raw_str = j["data"][i]["trace"]["parsed"]["signals"]["raw"];
                                                int raw = std::atoi(raw_str.c_str());
                                            }
                                            if(j["data"][i]["trace"]["parsed"]["signals"].find("phys") != j["data"][i]["trace"]["parsed"]["signals"].end())
                                            {
                                                std::string phys_str = j["data"][i]["trace"]["parsed"]["signals"]["phys"];
                                                int phys = std::atoi(phys_str.c_str());
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(j["data"][i]["trace"]["id"] == 516)
                    {;}
                }
            }

        }
    }


    // s->send(hdl, msg->get_payload(), msg->get_opcode());
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // m_endpoint.send(hdl, std::to_string(counter++), msg->get_opcode());
}

void utility_server::open_handler(server* s, websocketpp::connection_hdl hdl)
{
	std::cout << "on_open called with hdl: " << hdl.lock().get() << std::endl;
	// cli_handler cli;// cli.hdl = hdl;// cli.msg = msg;
	m_clis.emplace_back(hdl);
    s->send(hdl, "msg from handler", websocketpp::frame::opcode::text);
}

void utility_server::close_handler(server* s, websocketpp::connection_hdl hdl)
{
	std::cout << "on_close called with hdl: " << hdl.lock().get() << std::endl;
    std::vector<websocketpp::connection_hdl>::iterator cli_iter = m_clis.begin();
    for (; cli_iter != m_clis.end(); ++cli_iter)
	{
		if((*cli_iter).lock().get() == hdl.lock().get())
        {
            m_clis.erase(cli_iter);
            return;
        }
	}
}

void utility_server::resp()
{
    for (auto cli: m_clis)
    {
        if (m_msg_switch)
        {
			m_endpoint.send(cli, m_msg_running, websocketpp::frame::opcode::text); // m_endpoint.send(cli.hdl, m_msg_running, cli.msg->get_opcode());
			
        }
        else
        {
			m_endpoint.send(cli, m_msg_pending, websocketpp::frame::opcode::text); // m_endpoint.send(cli.hdl, m_msg_pending, cli.msg->get_opcode());
        }
    }
}

float utility_server::hz()
{
    return (1000/m_msg_hz);
}

void utility_server::run()
{
    // 监听端口
    m_endpoint.listen(m_port);

    m_endpoint.start_accept();

    // 开始Asio事件循环
    m_endpoint.run();
}

void utility_server::flip_msg(bool latch)
{
    m_msg_switch = latch;
}
