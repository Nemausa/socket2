#include"Config.hpp"
#include"TcpWebSocketClient.hpp"

using namespace doyou::io;

int main(int argc, char *args[])
{
#if _WIN32 && _CONSOLE
	system("chcp 65001");
#endif

	//设置运行日志名称
	Log::Instance().setLogPath("clientLog", "w", false);
	Config::Instance().Init(argc, args);
	
	TcpWebSocketClient wsClient;
	wsClient.connect("ws://192.168.1.103:4567/");
	wsClient.onopen = [](WebSocketClientC* pWSClient) 
	{
		std::string msg = "bu hao.";
		for (int i = 0; i < 10; i++)
			msg += "-hello web";
		msg += "===";
		pWSClient->writeText(msg.c_str(), msg.length());
	};

	wsClient.onmessage = [](WebSocketClientC* pWSClient)
	{
		auto data = pWSClient->fetch_data();
		CELLLog_Info("websocket server say: %s", data);
		/*WebSocketHeader& wsh = pWSClient->WebsocketHeader();
		pWSClient->writeText(data, wsh.len);*/
	};

	while (true)
	{
		wsClient.OnRun(1);
	}

	wsClient.Close();
	return 0;
}