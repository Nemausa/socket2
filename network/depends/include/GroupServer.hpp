#ifndef _doyou_io_GroupServer_HPP_
#define _doyou_io_GroupServer_HPP_

#include<regex>
#include"INetClient.hpp"
#include "DBUser.hpp"
#include "UserManager.hpp"

namespace doyou {
	namespace io {
		class GroupServer
		{
		private:
			INetClient _csGate;
			DBUser _dbuser;
			UserManager _userManager;
		public:
			void Init()
			{
				_dbuser.init();
				auto csGate = Config::Instance().getStr("csGateUrl", "ws://127.0.0.1:4567");
				_csGate.connect("csGate", csGate);

				_csGate.reg_msg_call("onopen", std::bind(&GroupServer::onopen_csGate, this, std::placeholders::_1, std::placeholders::_2));

				_csGate.reg_msg_call("cs_msg_group_create", std::bind(&GroupServer::cs_msg_group_create, this, std::placeholders::_1, std::placeholders::_2));
				_csGate.reg_msg_call("cs_msg_group_join", std::bind(&GroupServer::cs_msg_group_join, this, std::placeholders::_1, std::placeholders::_2));
				_csGate.reg_msg_call("cs_msg_group_exit", std::bind(&GroupServer::cs_msg_group_exit, this, std::placeholders::_1, std::placeholders::_2));
				_csGate.reg_msg_call("cs_msg_group_say", std::bind(&GroupServer::cs_msg_group_say, this, std::placeholders::_1, std::placeholders::_2));
			}

			void Run()
			{
				_csGate.run(1);
				_dbuser.run();
			}

			void Close()
			{
				_csGate.close();
			}

		private:

			const std::string make_token(int64 userId, int clientId)
			{
				std::stringstream ss;
				ss << Time::getNowInMilliSec() <<'@'<< userId <<'@'<< clientId;
				gloox::SHA sha1;
				sha1.feed(ss.str());
				return sha1.hex();
			}

			void onopen_csGate(INetClient* client, neb::CJsonObject& msg)
			{
				neb::CJsonObject json;
				json.Add("type", "GroupServer");
				json.Add("name", "GroupServer001");
				json.Add("sskey", "ssmm00@123456");
				json.AddEmptySubArray("apis");
				json["apis"].Add("cs_msg_login");
				json["apis"].Add("cs_msg_register");
				json["apis"].Add("cs_msg_change_pw");

				client->request("ss_reg_api", json, [](INetClient* client, neb::CJsonObject& msg) {
					CELLLog_Info(msg("data").c_str());
				});
			}

			void cs_msg_group_create(INetClient* client, neb::CJsonObject& msg)
			{

			}

			void cs_msg_group_join(INetClient* client, neb::CJsonObject& msg)
			{

			}

			void cs_msg_group_exit(INetClient* client, neb::CJsonObject& msg)
			{

			}

			void cs_msg_group_say(INetClient* client, neb::CJsonObject& msg)
			{

			}
			

		};
	}
}
#endif // !_doyou_io_GroupServer_HPP_
