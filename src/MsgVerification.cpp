#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include "Config.h"
#include "nlohmann/json.hpp"
#include <queue>
#include <mutex>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

std::mutex mMutex;


void on_open(client *c, websocketpp::connection_hdl hdl)
{
    std::string msg = "hello";
    c->send(hdl, msg, websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Tx: " + msg);
}

// void parse_256(nlohmann::json data) {
//     Control_t ptl_data;
//     uint64_t timestamp = data["timestamp"];
//     ptl_data.timestamp = timestamp;
//     nlohmann::json parsed = data["parsed"];
//     std::string name = parsed["name"];
// 
//     for (const auto& signal : parsed["signals"]) {
//         std::string signal_name = signal["name"];
//         if (signal_name.compare("MU_Lenkradwinkel") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             float raw_value = std::stof(raw_value_str.c_str());
//             float phys_value = std::stof(phys_value_str.c_str());
//             ptl_data.steering = raw_value;
//         }
//         else if (signal_name.compare("MU_Fahrpedal") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             float raw_value = std::stof(raw_value_str.c_str());
//             float phys_value = std::stof(phys_value_str.c_str());
//             ptl_data.throttle = raw_value;
//         }
//         else if (signal_name.compare("MU_Bremspedal") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             float raw_value = std::stof(raw_value_str.c_str());
//             float phys_value = std::stof(phys_value_str.c_str());
//             ptl_data.brake = raw_value;
//         }
//     }
//     std::lock_guard<std::mutex> locker(mMutex);
//     myControlQueue.push(ptl_data);
// }
// 
// void parse_257(nlohmann::json data) {
//     SignalLight_t light_data;
//     uint64_t timestamp = data["timestamp"];
//     light_data.timestamp = timestamp;
// 
//     nlohmann::json parsed = data["parsed"];
//     std::string name = parsed["name"];
// 
//     for (const auto& signal : parsed["signals"]) {
//         std::string signal_name = signal["name"];
//         if (signal_name.compare("BH_Fernlicht:") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             int raw_value = std::atoi(raw_value_str.c_str());
//             int phys_value = std::atoi(phys_value_str.c_str());
//             light_data.HighBeam = raw_value;
//         }
//         else if (signal_name.compare("BH_Blinker_re:") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             int raw_value = std::atoi(raw_value_str.c_str());
//             int phys_value = std::atoi(phys_value_str.c_str());
//             light_data.RightBlinker = raw_value;
//         }
//         else if (signal_name.compare("BH_Blinker_li") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             int raw_value = std::atoi(raw_value_str.c_str());
//             int phys_value = std::atoi(phys_value_str.c_str());
//             light_data.LeftBlinker = raw_value;
//         }
//         else if (signal_name.compare("GE_Waehlhebel:") == 0){
//             std::string raw_value_str = signal["raw"];
//             std::string phys_value_str = signal["phys"];
//             int raw_value = std::atoi(raw_value_str.c_str());
//             int phys_value = std::atoi(phys_value_str.c_str());
//             light_data.LeftBlinker = raw_value;
//         }
//     }
//     std::lock_guard<std::mutex> locker(mMutex);
//     mySignalLightQueue.push(light_data);
// }
// 
// void parsed_data(nlohmann::json j) {
//     for (const auto& data : j["data"]) {
//         if (data.find("parsed") == data.end()) {
//             continue;
//         }
//         int id = data["id"];
//         if (id == 256)
//         {
//             parse_256(data);
//         }
//         else if (id == 257){
//             parse_257(data);
//         }
//     }
// }
// 

void on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get() << std::endl
              << "message: " << msg->get_payload() << std::endl;
    // nlohmann::json j = nlohmann::json::parse(msg->get_payload().c_str());
    // parsed_data(j);

    // websocketpp::lib::error_code ec;
    // c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);
    // if (ec)
    // {
    //     std::cout << "Echo failed because " << ec.message() << std::endl;
    // }
}

//定时器回调函数
void Timeout(client *c, websocketpp::connection_hdl &hdl, boost::asio::deadline_timer *pt, const boost::system::error_code &ec)
{

    // std::string test = "{\"opcode\":\"heartbeat\",\"requestId\":" + std::to_string(ZDBOX::WebsocketAPI.) + "}";
    // c->send(hdl, test, websocketpp::frame::opcode::text);
    // reqID++;
    // std::cout << "Send" << std::endl;

    // if(ec)
    // {
    //     std::cout << "timer is cancel " << std::endl;
    //     return;
    // }
    // static int count = 0;
    // c->send(hdl, "hello", websocketpp::frame::opcode::text);
    // count++;
    // if(count > 5)//定时器触发五次后关闭连接
    // {
    //     c->close(hdl, websocketpp::close::status::normal, "");
    //     return;
    // }
    pt->expires_at(pt->expires_at() + boost::posix_time::seconds(5));
    pt->async_wait(bind(Timeout, c, hdl, pt, ::_1));

}

int main(int argc, char *argv[])
{
    client c;

	rr::RrConfig config;
	config.ReadConfig("config.ini");
	std::string ip = config.ReadString("WebsocketClient", "ServerIp", "127.0.0.1");
	std::string port = config.ReadString("WebsocketClient", "ServerPort", "9002");
    std::string uri = "ws://" + ip + ":" + port;

	printf("------------uri: %s\n", uri.c_str());

    // c.set_access_channels(websocketpp::log::alevel::all);
    // c.clear_access_channels(websocketpp::log::alevel::frame_payload);
    // c.clear_access_channels(websocketpp::log::alevel::frame_header);

    // 初始化 ASIO
    c.init_asio();

    // 注册消息回调
    c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
    c.set_open_handler(bind(&on_open, &c, _1));

    websocketpp::lib::error_code ec;
    client::connection_ptr con = c.get_connection(uri, ec);
    con->add_subprotocol("janus-protocol");
    if(ec)
    {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
        return 0;
    }
    auto hdl = con->get_handle();

    c.connect(con);
    
    boost::asio::deadline_timer t(c.get_io_service(), boost::posix_time::seconds(5)); //设置一个5s超时的定时器
    t.async_wait(bind(&Timeout, &c, hdl, &t, ::_1));

    
    std::thread th([&c] { c.run(); });

    //休眠13s后取消定时器并关闭连接
    // Sleep(13);
    // t.cancel();
    // c.close(hdl, websocketpp::close::status::normal, "");

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Send message every second
        // c.send(hdl, jStr.c_str(), websocketpp::frame::opcode::text);
    }

    th.join();
	return 0;
}
