#ifndef _doyou_io_GateServer_HPP_
#define _doyou_io_GateServer_HPP_

#include"INetServer.hpp"
#include "INetClient.hpp"


namespace doyou {
	namespace io {
		class LinkServer
		{
		private:
			INetServer _netserver;
			INetClient _ss_gate;
		public:
			void Init()
			{
				auto ssGateUrl = Config::Instance().getStr("csGateUrl", "ws://127.0.0.1:4567");
				_ss_gate.connect("ssGate", ssGateUrl);

				_ss_gate.reg_msg_call("onopen", std::bind(&LinkServer::onopen_csGate, this, std::placeholders::_1, std::placeholders::_2));
				_ss_gate.reg_msg_call("ss_msg_user_logout", std::bind(&LinkServer::ss_msg_user_logout, this, std::placeholders::_1, std::placeholders::_2));
				_ss_gate.reg_msg_call("ss_msg_user_login", std::bind(&LinkServer::ss_msg_user_login, this, std::placeholders::_1, std::placeholders::_2));

				const char* strIP = Config::Instance().getStr("strIP", "any");
				uint16_t nPort = Config::Instance().getInt("nPort", 5000);
				_netserver.Init(strIP, nPort);
				_netserver.on_other_msg = std::bind(&LinkServer::on_other_msg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
				_netserver.on_client_leave = std::bind(&LinkServer::on_client_leave, this, std::placeholders::_1);
				_netserver.on_client_run = std::bind(&LinkServer::on_client_run, this, std::placeholders::_1);

				_netserver.reg_msg_call("cs_msg_heart", std::bind(&LinkServer::cs_msg_heart, this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				_netserver.reg_msg_call("cs_reg_client", std::bind(&LinkServer::cs_reg_client, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		
				
			}

			void Close()
			{
				_netserver.Close();
			}

		private:

			void onopen_csGate(INetClient* client, neb::CJsonObject& msg)
			{
				neb::CJsonObject json;
				json.Add("type", "LinkServer");
				json.Add("name", "LinkServer001");
				json.Add("sskey", "ssmm00@123456");
				json.AddEmptySubArray("apis");
				json["apis"].Add("ss_msg_user_logout");
				json["apis"].Add("ss_msg_user_login");

				client->request("ss_reg_server", json, [](INetClient* client, neb::CJsonObject& msg) {
					CELLLog_Info(msg("data").c_str());
				});
			}

			void cs_msg_heart(Server* server, INetClientS* client, neb::CJsonObject& msg)
			{
				if (client->is_cc_link() || client->is_ss_link())
				{

				}
			}

			void cs_reg_client(Server* server, INetClientS* client, neb::CJsonObject& msg)
			{
				auto cckey = msg["data"]("cckey");
				auto cckey_local = Config::Instance().getStr("cckey", "ccmm00@123456");
				if (cckey != cckey_local)
				{
					client->resp_error(msg, "cckey error");
					return;
				}
				auto type = msg["data"]("type");

				client->link_type(type);
				client->link_name(type);
				client->is_cc_link(true);

				/*std::string token;
				if (!msg.Get("token", token))
				{
					client->response(msg, "not found token");
					return;
				}*/
				client->response(msg, "cs_reg_client ok!");
			}

			void ss_msg_user_login(INetClient* client, neb::CJsonObject& msg)
			{
				int clientId = 0;
				if (!msg["data"].Get("clientId", clientId))
				{
					CELLLog_Error("not found key<clientId>");
					return;
				}

				std::string token;
				if (!msg["data"].Get("token", token))
				{
					CELLLog_Error("not found key<token>");
				}

				int64_t userId = 0;
				if (!msg["data"].Get("userId", userId))
				{
					CELLLog_Error("not found key<userId>");
					return;
				}

				clientId = ClientId::get_client_id(clientId);
				auto ClientS = dynamic_cast<INetClientS*>(_netserver.find_client(clientId));
				if (!ClientS)
				{
					CELLLog_Error("GateServer::ss_msg_user_login::server->find_client(%d) miss", clientId);
					return;
				}

				ClientS->userId(userId);
				ClientS->token(token);
				

			}

			void ss_msg_user_logout( INetClient* client, neb::CJsonObject& msg)
			{
				int clientId = 0;
				if (!msg["data"].Get("clientId", clientId))
				{
					CELLLog_Error("not found key<clientId>");
					return;
				}

				int64_t userId = 0;
				if (!msg["data"].Get("userId", userId))
				{
					CELLLog_Error("not found key<userId>");
					return;
				}

				clientId = ClientId::get_client_id(clientId);
				auto ClientS = dynamic_cast<INetClientS*>(_netserver.find_client(clientId));
				if (!ClientS)
				{
					CELLLog_Error("GateServer::ss_msg_user_logout::server->find_client(%d) miss", clientId);
					return;
				}

				if (userId != ClientS->userId())
				{
					CELLLog_Error("GateServer::ss_msg_user_logout::userId<%lld> != clients->userId<%lld>", userId, ClientS->userId());
					return;
				}

				ClientS->userId(0);
				ClientS->token("");
			}


			void on_other_msg(Server* server, INetClientS* client, std::string& cmd, neb::CJsonObject& msg)
			{
				int clientId = 0;
				auto str = msg.ToString();
				CELLLog_Info("%s",str);
				if (!msg.Get("clientId", clientId))
				{
					CELLLog_Error("not found clientId");
					return;
				}

				int msgId = 0;
				if (!msg.Get("msgId", msgId))
				{
					CELLLog_Error("LinkServer::on_other_msg::not found key<msgId>");
					return;
				}

				_ss_gate.request(msg,[this, clientId, msgId](INetClient* client, neb::CJsonObject& json) {
					auto clients = dynamic_cast<INetClientS*>(_netserver.find_client(clientId));
					if (!clients)
					{
						CELLLog_Error("LinkServer::on_other_msg::_netserver.find_client(%d) miss", clientId);
						return;
					}

					json.Replace("msgId", msgId);
					json.Delete("clientId");

					clients->transfer(json);
				});
				
			}


			void on_client_leave(INetClientS* client)
			{
				if (client->is_login())
				{
					neb::CJsonObject msg;
					msg.Add("userId", client->userId());
					msg.Add("clientId", client->sockfd());

					_ss_gate.broadcast("ss_msg_user_exit", msg);
				}
				
				{
					neb::CJsonObject msg;
					msg.Add("userId", client->userId());
					msg.Add("clientId", client->sockfd());

					_ss_gate.broadcast("ss_msg_client_exit", msg);
				}

				
			}

			void on_client_run(Server* pServer)
			{
				_ss_gate.run(0);
			}

		};
	}
}
#endif // !_doyou_io_GateServer_HPP_
