#include"Config.hpp"
#include "TcpHttpClient.hpp"
using namespace doyou::io;


class MyHttpClient : public TcpHttpClient
{
public:
	MyHttpClient()
	{
		InitSocket(AF_INET, 10240, 10240);
	}
public:
	virtual void OnNetMsg(netmsg_DataHeader* header)
	{

	}

	void get(const char* url)
	{
		hostname2ip(url, "80");
	}

	int hostname2ip(const char* hostname, const char* port)
	{
		if (!hostname)
		{
			Log::Warring("hostname2ip(hostname is null ptr).");
			return -1;
		}

		if (!port)
		{
			Log::Warring("hostname2ip(port is null ptr).");
			return -1;
		}

		unsigned short port_ = 80;
		if (port, strlen(port) > 0)
			port_ = atoi(port);


		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_IP;
		hints.ai_flags = AI_ALL;
		addrinfo* pAddrList = nullptr;
		int ret = getaddrinfo(hostname, nullptr, &hints, &pAddrList);
		if (0 != ret)
		{
			Log::PError("%s getaddrinfo", hostname);
			freeaddrinfo(pAddrList);
			return ret;
		}

		char ipStr[256] = {};
		for (auto pAddr = pAddrList; pAddr != nullptr; pAddr = pAddr->ai_next)
		{
			ret = getnameinfo(pAddr->ai_addr, pAddr->ai_addrlen, ipStr, 255, nullptr, 0, NI_NUMERICHOST);
			if (0 != ret)
			{
				Log::PError("%s getnameinfo", hostname);
				continue;
			}
			else {
				if (pAddr->ai_family == AF_INET6)
					Log::Info("%s ipv6: %s", hostname, ipStr);
				else if (pAddr->ai_family == AF_INET)
					Log::Info("%s ipv4: %s", hostname, ipStr);
				else {
					Log::Info("%s addr: %s", hostname, ipStr);
					continue;
				}

			}
		}

		freeaddrinfo(pAddrList);
		return ret;
	}
};

int main(int argc, char* args[])
{
	//设置运行日志名称
	Log::Instance().setLogPath("clientLog", "w", false);
	Config::Instance().Init(argc, args);

	MyHttpClient pClient;
	char hname[128] = {};
	gethostname(hname, 127);

	pClient.get(hname); 
	pClient.Close();
	return 0;
}  