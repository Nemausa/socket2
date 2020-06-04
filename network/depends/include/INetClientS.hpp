#ifndef _doyou_io_INetServerS_HPP_
#define _doyou_io_INetServerS_HPP_

#include"WebSocketClientS.hpp"
#include "CJsonObject.hpp"

namespace doyou {
	namespace io {

		//客户端数据类型
		class INetClientS :public WebSocketClientS
		{
		public:
			INetClientS(SOCKET sockfd = INVALID_SOCKET, int sendSize = SEND_BUFF_SZIE, int recvSize = RECV_BUFF_SZIE) :
				WebSocketClientS(sockfd, sendSize, recvSize)
			{

			}

			void response(int msgId, std::string data)
			{
				neb::CJsonObject ret;
				ret.Add("msgId", msgId);
				ret.Add("time", Time::system_clock_now());
				ret.Add("data", data);

				std::string retStr = ret.ToString();
				writeText(retStr.c_str(), retStr.length());
			}

			void response(neb::CJsonObject& msg, std::string data)
			{
				int msgId = 0;
				if (!msg.Get("msgId", msgId))
				{
					CELLLog_Info("not found key<%s>", "msgId");
					return;
				}

				response(msgId, data);
			}
		};
	}
}
#endif // !_doyou_io_INetServerS_HPP_