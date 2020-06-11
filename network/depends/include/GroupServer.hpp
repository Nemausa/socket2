#ifndef _doyou_io_GroupServer_HPP_
#define _doyou_io_GroupServer_HPP_

#include<regex>
#include"INetClient.hpp"
#include "GroupManager.hpp"
#include <sstream>

namespace doyou {
	namespace io {
		class GroupServer
		{
		private:
			INetClient _csGate;
			GroupManager _group_manager;
		public:
			void Init()
			{
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
				json["apis"].Add("cs_msg_group_create");
				json["apis"].Add("cs_msg_group_join");
				json["apis"].Add("cs_msg_group_exit");
				json["apis"].Add("cs_msg_group_say");

				client->request("ss_reg_server", json, [](INetClient* client, neb::CJsonObject& msg) {
					CELLLog_Info(msg("data").c_str());
				});
			}

			void cs_msg_group_create(INetClient* client, neb::CJsonObject& msg)
			{
				//1鉴定权限
				//请求者是否具备创建group的权限
				//已登录的client或者server
				int clientId = 0;
				if (!msg.Get("clientId", clientId))
				{
					CELLLog_Error("not found key<clientId>.");
					return;
				}

				bool is_ss_link = false;
				msg.Get("is_ss_link", is_ss_link);

				int64_t userId = 0;
				msg.Get("userId", userId);

				if (userId == 0 && !is_ss_link)
				{
					client->resp_error(msg, "not login");
					return;
				}

				//2.group的id
				//可以请求者提供，也可以是服务来分配
				int group_id = 0;
				if (!msg["data"].Get("group_id", group_id))
				{
					client->resp_error(msg,"not found key<group_id>");
					return;
				}


				//3.group的key
				//可以请求者提供，也可以是服务来分配
				int group_key = 0;
				if (!msg["data"].Get("group_key", group_key))
				{
					client->resp_error(msg, "not found key<group_key>");
					return;
				}

				if (!_group_manager.create(group_id, group_key, clientId))
				{
					client->resp_error(msg, "create group failed");
					return;
				}

				CELLLog_Info("group.create:id<%d>key<%d>", group_id, group_key);

				neb::CJsonObject json;
				json.Add("group_id", group_id);
				json.Add("group_key", group_key);
				client->response(msg, json);

			}

			void cs_msg_group_join(INetClient* client, neb::CJsonObject& msg)
			{
				int clientId = 0;
				if (!msg.Get("clientId", clientId))
				{
					CELLLog_Error("not found key<clientId>.");
					return;
				}

				bool is_ss_link = false;
				msg.Get("is_ss_link", is_ss_link);

				int64_t userId = 0;
				msg.Get("userId", userId);

				if (userId == 0 && !is_ss_link)
				{
					client->resp_error(msg, "not login");
					return;
				}


				int group_id = 0;
				if (!msg["data"].Get("group_id", group_id))
				{
					client->resp_error(msg, "not found key<group_id>");
					return;
				}


				int group_key = 0;
				if (!msg["data"].Get("group_key", group_key))
				{
					client->resp_error(msg, "not found key<group_key>");
					return;
				}

				if (!_group_manager.join(group_id, group_key, clientId))
				{
					client->resp_error(msg, "join failed");
					return;
				}

				CELLLog_Info("group.join:id<%d>key<%d>", group_id, group_key);

				neb::CJsonObject json;
				json.Add("group_id", group_id);
				json.Add("group_key", group_key);
				client->response(msg, json);

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
