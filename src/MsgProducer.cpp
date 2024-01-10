#include "WebsocketServer.h"
#include "Config.h"
#include <iostream>
#include <fstream>
#include <exception>

#include <time.h>
#ifdef WIN32
#  include <windows.h>
#else
#  include <sys/time.h>
#endif
#ifdef WIN32
int gettimeofday(struct timeval *tp, void *tzp)
{
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);
  tm.tm_year   = wtm.wYear - 1900;
  tm.tm_mon   = wtm.wMonth - 1;
  tm.tm_mday   = wtm.wDay;
  tm.tm_hour   = wtm.wHour;
  tm.tm_min   = wtm.wMinute;
  tm.tm_sec   = wtm.wSecond;
  tm. tm_isdst  = -1;
  clock = mktime(&tm);
  tp->tv_sec = clock;
  tp->tv_usec = wtm.wMilliseconds * 1000;
  return (0);
}
#endif

int32_t system_sec()
{
	timeval current_time_tmp;
	gettimeofday(&current_time_tmp, NULL);
	return current_time_tmp.tv_sec;
}

// int main(int argc, char* argv[])
// {
//     rr::RrConfig config;
//     config.ReadConfig("config.ini");
//     // uint32_t interval = config.ReadInt("Test", "Msg_Interval_Sec", 6000);
//     uint32_t start_delay = config.ReadInt("Test", "Msg_Case_Start_Delay", 1000);
// 
// 	// SimOne
// 	bool new_case_start = true;
// 
// 	// Server
// 	utility_server s;
// 
// 	std::thread th_s([&]{
// 		int32_t timer = system_sec();
// 		uint32_t cnt = 0;
// 		bool launch = true;
// 		bool pre_msg = false;
// 		float pre_throttle = 0.0;
// 
// 		while (true)
// 		{
// 			// ------ tmp
// 			//cnt += (system_sec() - timer);
// 			//printf("------------ timer: %ld\n", cnt);
// 			//if (cnt < interval)
// 			if (so.isCaseStarted() && so.isCaseRunning())
// 			{
// 				if (pre_msg != true) // msg Pending -> Running
// 				{
// 					launch = true;
// 					pre_msg = true;
// 				}
// 				s.flip_msg(true);
// 			}
// 			else
// 			{
// 				if (pre_msg != false) // msg Running -> Pending
// 				{
// 					launch = true;
// 					pre_msg = false;
// 				}
// 				s.flip_msg(false);
// 			}
// 
// 			// ------ tmp
// 			//if (cnt > (2*interval))
// 			//{
// 			//	timer = system_sec();
// 			//	cnt = 0;
// 			//}
// 
// 			//if (launch && s.get_clis_size())
// 			//{
// 			//	launch = false;
// 			//	s.resp();
// 			//}
// 
// 			// send once
// 			if (launch && s.get_clis_size())
// 			{
// 				launch = false;
// 
// 				if (new_case_start && pre_msg)
// 				{
// 					new_case_start = false;
// 
// 					//while(!pre_throttle)
// 					//{
// 					//	pre_throttle = so.get_throttle();
// 					//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 					//	if (!so.isCaseRunning()) break;
// 					//}
// 
// 					//while(pre_throttle == so.get_throttle())
// 					//{
// 					//	std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 					//	if (!so.isCaseRunning()) break;
// 					//}
// 
// 					//pre_throttle = 0.0;
// 
// 					std::this_thread::sleep_for(std::chrono::milliseconds(start_delay));
// 				}
// 
// 				s.resp();
// 			}
// 
// 			uint32_t hz = s.hz();
// 			std::this_thread::sleep_for(std::chrono::milliseconds(hz));
// 		}
// 	});
// 
// 	std::thread th_soListener([&]{
// 		s.run();
// 	});
// 
// 	 while (true)
// 	 {
// 	 	try
// 	 	{
// 	 		while (true)
// 	 		{
// 	 			if (so.isCaseStopped())
// 	 			{
// 					new_case_start = true;
// 	 				so.simone_terminate();
// 					std::this_thread::sleep_for(std::chrono::milliseconds(6000));
// 	 				break;
// 	 			}
// 				std::this_thread::sleep_for(std::chrono::milliseconds(100));
// 	 		}
// 			new_case_start = true;
// 	 	}
// 	 	catch (std::exception &e)
// 	 	{
// 	 		printf("\033[1m\033[31mexception: %s\033[0m\n", e.what());
// 	 	}
// 	 }
// 
// 	th_soListener.join();
// 	th_s.join();
// 
// 	// Client
// 	// utility_client c;
// 	// c.connect("ws://127.0.0.1:9002");
// 	// c.set_msg(msg);
// 	// c.set_timeout(5);
// 	// std::thread th([&c]{c.run();});
// 	// Sleep(5000);
// 	// c.set_msg("change msg1");
// 	// printf("-------------- first\n");
// 	// Sleep(5000);
// 	// c.set_msg("change msg2");
// 	// printf("-------------- second\n");
// 	// Sleep(5000);
// 	// c.set_msg("change msg3");
// 	// printf("-------------- third\n");
// 	// Sleep(5000);
// 	// printf("-------------- closing ...\n");
// 	// c.close_timer();
// 	// th.join();
// 
//     return 0;
// }

void on_open(server* s, websocketpp::connection_hdl hdl)
{
	std::cout << "on_open called with hdl: " << hdl.lock().get() << std::endl;
	// cli_handler cli;// cli.hdl = hdl;// cli.msg = msg;
  // s->send(hdl, "msg from handler", websocketpp::frame::opcode::text);
}

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
	std::string filename = "/home/me/share/99_exchange/renjunmei/Websocket/myWebSocket/trace_can6_py03_steering.json";
  std::ifstream file(filename);
  if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
          // std::cout << "line : "<< line <<std::endl;
          s->send(hdl, line, websocketpp::frame::opcode::text);
      }
      file.close();
  } else {
      std::string error_message = "Failed to open file: " + filename;
      //s->send(hdl, error_message, websocketpp::frame::opcode::text);
  }   
	// std::cout << "payload: " << msg->get_payload() << std::endl; // " opcode: " << msg->get_opcode() << std::endl;
}

int main() {
    server echo_server;

    try {
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        echo_server.init_asio();
        echo_server.set_message_handler(std::bind(&on_message, &echo_server, std::placeholders::_1, std::placeholders::_2));
		echo_server.set_open_handler(std::bind(&on_open, &echo_server, std::placeholders::_1));
        echo_server.listen(9002);
        echo_server.start_accept();
        echo_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception" << std::endl;
    }
}

