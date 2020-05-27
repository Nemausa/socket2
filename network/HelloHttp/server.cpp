#include"Log.hpp"
#include"Config.hpp"
#include"TcpHttpServer.hpp"

using namespace doyou::io;

class MyServer : public TcpHttpServer
{
public:
	virtual void OnNetMsg(Server* pServer, Client* pClient, netmsg_DataHeader* header)
	{
		HttpClient *pHttpClient = dynamic_cast<HttpClient*>(pClient);
		if (!pHttpClient)
			return;

		if (!pHttpClient->getRequestInfo())
			return;
		
		if (pHttpClient->url("/add"))
		{
			int a = pHttpClient->getInt("a", 0);
			int b = pHttpClient->getInt("b", 0);
			int c = a + b;

			char respBodyBuff[32] = {};
			sprintf(respBodyBuff, "a+b=%d", c);


			char response[512] = {};
			strcat(response, "HTTP/1.1 200 OK\r\n");
			strcat(response, "Content-Type: text/html;charset=UTF-8\r\n");

			char respBodyLen[32] = {};
			sprintf(respBodyLen, "Content-Length: %d\r\n", strlen(respBodyBuff));
			strcat(response, respBodyLen);
			strcat(response, "\r\n");
			
			strcat(response, respBodyBuff);
			pClient->SendData(response, strlen(response));
		}
		// http消息 响应行
		//std::string response = "HTTP/1.1 200 OK\r\n";
		//response += "Content-Type: text/html;charset=UTF-8\r\n";
		//response += "Content-Length: 5\r\n";
		//response += "\r\n";
		//response += "hello";
		//pClient->SendData(response.c_str(), response.length());
		//char response[512] = {};
		//strcat(response, "HTTP/1.1 200 OK\r\n");
		//strcat(response, "Content-Type: text/html;charset=UTF-8\r\n");
		//strcat(response, "Content-Length: 14\r\n");
		//strcat(response, "\r\n");
		//strcat(response, "OnNetMsg:hello");
		//pClient->SendData(response, strlen(response));
	}
private:

};

int main(int argc, char *args[])
{
	// 设置运行日志名称
	Log::Instance().setLogPath("serverLog", "w", false);
	Config::Instance().Init(argc, args);

	const char* strIP = Config::Instance().getStr("strIP", "any");
	uint16_t nPort = Config::Instance().getInt("nPort", 4567);
	int nThread = Config::Instance().getInt("nThread", 1);

	if (strcmp(strIP, "any") == 0)
	{
		strIP = nullptr;
	}

	MyServer server;
	if (Config::Instance().hasKey("-ipv6"))
	{
		CELLLog_Info("-ipv6");
		server.InitSocket(AF_INET6);
	}
	else {
		CELLLog_Info("-ipv4");
		server.InitSocket();

	}
	server.Bind(strIP, nPort);
	server.Listen(SOMAXCONN);
	server.Start(nThread);

	//在主线程中等待用户输入命令
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			server.Close();
			break;
		}
		else {
			CELLLog_Info("undefine cmd");
		}
	}

	CELLLog_Info("exit.");

	return 0;
}