#ifndef _doyou_io_UserClient_HPP_
#define _doyou_io_UserClient_HPP_

#include"INetClient.hpp"

namespace doyou {
	namespace io {
		class UserClient
		{
		private:
			INetClient _csGate;
			
		public:
			void Init()
			{
				auto csGate = Config::Instance().getStr("csGateUrl", "ws://127.0.0.1:4567");
				_csGate.connect("csGate", csGate);

				_csGate.reg_msg_call("onopen", std::bind(&UserClient::onopen_csGate, this, std::placeholders::_1, std::placeholders::_2));

				_csGate.reg_msg_call("sc_msg_logout", std::bind(&UserClient::sc_msg_logout, this, std::placeholders::_1, std::placeholders::_2));
			}

			void Run()
			{
				_csGate.run(1);
			}

			void Close()
			{
				_csGate.close();
			}

		private:
			void onopen_csGate(INetClient* client, neb::CJsonObject& msg)
			{
				neb::CJsonObject json;
				json.Add("username", "testcc0");
				json.Add("password", "testmm0");

				client->request("cs_msg_login", json, [](INetClient* client, neb::CJsonObject& msg) 
				{
					int state = 0;
					if (!msg.Get("state", state))
					{
						CELLLog_Info("not found key <state>");
						return;
					}

					if (state != 0)
					{
						CELLLog_Error("cs_msg_login error <state=%d> msg:%s", state, msg("data").c_str());
						return;
					}
					int64_t userId = 0;
					if (!msg["data"].Get("userId", userId))
					{
						CELLLog_Info("not found key <userId>");
						return;
					}

					std::string token;
					if (!msg["data"].Get("token", token))
					{
						CELLLog_Info("not found key <token>");
						return;
					}

					CELLLog_Info("login:userId=%lld, token=%s", userId, token.c_str());
				});
			}

			void sc_msg_logout(INetClient* client, neb::CJsonObject& msg)
			{
				CELLLog_Info("Info logout:%s", msg("data").c_str());

				
			}
		};
	}
}
#endif // !_doyou_io_UserClient_HPP_
