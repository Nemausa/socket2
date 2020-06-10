#ifndef _doyou_io_INetClientS_HPP_
#define _doyou_io_INetClientS_HPP_

#include"WebSocketClientS.hpp"
#include"CJsonObject.hpp"
#include "INetStateCode.hpp"

namespace doyou {
	namespace io {

		//客户端数据类型
		class INetClientS :public WebSocketClientS
		{
		private:
			std::string _link_name;
			std::string _link_type = "client";
			bool _is_ss_link = false;
			bool _is_cc_link = false;
			//
			std::string _token;
			int64_t _userId = 0;
		public:
			INetClientS(SOCKET sockfd = INVALID_SOCKET, int sendSize = SEND_BUFF_SZIE, int recvSize = RECV_BUFF_SZIE) :
				WebSocketClientS(sockfd, sendSize, recvSize)
			{

			}

			std::string& link_name()
			{
				return _link_name;
			}

			void link_name(std::string& str)
			{
				_link_name = str;
			}

			std::string& link_type()
			{
				return _link_type;
			}

			void link_type(std::string& str)
			{
				_link_type = str;
			}

			bool is_ss_link()
			{
				return _is_ss_link;
			}

			void is_ss_link(bool b)
			{
				_is_ss_link = b;
			}

			bool is_cc_link()
			{
				return _is_cc_link;
			}

			void is_cc_link(bool b)
			{
				_is_cc_link = b;
			}

			const std::string& token()
			{
				return _token;
			}

			void token(const std::string& str)
			{
				_token = str;
			}

			int64_t userId()
			{
				return _userId;
			}

			void userId(int64_t n)
			{
				_userId = n;
			}

			bool is_login()
			{
				return _userId != 0;
			}

			template<class T>
			void response(neb::CJsonObject& msg, const T& data, int state = state_code_ok)
			{
				int msgId = 0;
				if (!msg.Get("msgId", msgId))
				{
					CELLLog_Error("not found key<%s>.", "msgId");
					return;
				}

				neb::CJsonObject ret;
				ret.Add("msgId", msgId);
				ret.Add("state", state);
				ret.Add("type", msg_type_resp);
				ret.Add("time", (int64)Time::system_clock_now());
				ret.Add("data", data);

				std::string retStr = ret.ToString();
				this->writeText(retStr.c_str(), retStr.length());
			}

			template<class T>
			void resp_error(neb::CJsonObject& msg, const T& data, int state = state_code_error)
			{
				response(msg, data, state);
			}

		};
	}
}
#endif // !_doyou_io_INetClientS_HPP_
