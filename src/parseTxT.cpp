#include <string>
#include <iostream>
#include <fstream>
int main()
{
  std::string filename = "/home/me/share/99_exchange/renjunmei/Websocket/myWebSocket/trace_can6_py03_steering.json";
  std::ifstream file(filename);
  if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
          std::cout << "line : "<< line <<std::endl;
          //s->send(hdl, line, websocketpp::frame::opcode::text);
      }
      file.close();
  } else {
      std::string error_message = "Failed to open file: " + filename;
      //s->send(hdl, error_message, websocketpp::frame::opcode::text);
  }

}