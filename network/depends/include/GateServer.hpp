#ifndef _doyou_io_GateServer_HPP_
#define _doyou_io_GateServer_HPP_

#include"INetServer.hpp"
#include"INetTransfer.hpp"

namespace doyou {
	namespace io {
		class GateServer
		{
		private:
			INetServer _netserver;
			INetTransfer _transfer;
		public:
			void Init()
			{
				const char* strIP = Config::Instance().getStr("strIP", "any");
				uint16_t nPort = Config::Instance().getInt("nPort", 4567);
				_netserver.Init(strIP, nPort);
				_netserver.on_other_msg = std::bind(&GateServer::on_other_msg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
				_netserver.on_broadcast_msg = std::bind(&GateServer::on_broadcast_msg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
				_netserver.on_client_leave = std::bind(&GateServer::on_client_leave, this, std::placeholders::_1);
				_netserver.reg_msg_call("cs_msg_heart", std::bind(&GateServer::cs_msg_heart, this,std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				_netserver.reg_msg_call("ss_reg_server", std::bind(&GateServer::ss_reg_server, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				//_netserver.reg_msg_call("cs_reg_client", std::bind(&GateServer::cs_reg_client, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				//_netserver.reg_msg_call("ss_msg_user_logout", std::bind(&GateServer::ss_msg_user_logout, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				//_netserver.reg_msg_call("ss_msg_user_login", std::bind(&GateServer::ss_msg_user_login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				
			}

			void Close()
			{
				_netserver.Close();
			}

		private:
			void cs_msg_heart(Server* server, INetClientS* client, neb::CJsonObject& msg)
			{
				if (client->is_cc_link() || client->is_ss_link())
				{

				}
			}

			void ss_reg_server(Server* server, INetClientS* client, neb::CJsonObject& msg)
			{
				auto sskey = msg["data"]("sskey");
				auto sskey_local = Config::Instance().getStr("sskey", "ssmm00@123456");
				if (sskey != sskey_local)
				{
					client->resp_error(msg, "sskey error");
					return;
				}
				auto type = msg["data"]("type");
				auto name = msg["data"]("name");

				client->link_type(type);
				client->link_name(name);
				client->is_ss_link(true);

				auto apis = msg["data"]["apis"];

				if (!apis.IsArray())
				{
					client->resp_error(msg, "not found apis");
					return;
				}

				int size = apis.GetArraySize();
				for (size_t i = 0; i < size; i++)
				{
					CELLLog_Info("ss_reg_api: %s >> %s", name.c_str(), apis(i).c_str());
					_transfer.add(apis(i), client);
				}

				client->response(msg, "ss_reg_server ok!");
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

			void ss_msg_user_login(Server* server, INetClientS* client, neb::CJsonObject& msg)
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

				auto ClientS = dynamic_cast<INetClientS*>(server->find_client(clientId));
				if (!ClientS)
				{
					CELLLog_Error("GateServer::ss_msg_user_login::server->find_client(%d) miss", clientId);
					return;
				}

				ClientS->userId(userId);
				ClientS->token(token);
				

			}

			void ss_msg_user_logout(Server* server, INetClientS* client, neb::CJsonObject& msg)
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

				auto ClientS = dynamic_cast<INetClientS*>(server->find_client(clientId));
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
				auto str = msg.ToString();
				int ret = _transfer.on_net_msg_do(cmd, str);
				if (state_code_undefine_cmd == ret)
				{
					CELLLog_Info("on_other_msg: transfer not found cmd<%s>.", cmd.c_str());
					client->response(msg, "undefine cmd", state_code_undefine_cmd);
				}
				else if (state_code_server_busy == ret)
				{
					CELLLog_Info("on_other_msg: transfer not found cmd<%s>.", cmd.c_str());
					client->response(msg, "server busy", state_code_server_busy);
				}
				else if (state_code_server_off == ret)
				{
					CELLLog_Info("on_other_msg: transfer not found cmd<%s>.", cmd.c_str());
					client->response(msg, "server offline", state_code_server_off);
				}
				
			}


			template<class T>
			void broadcast(const std::string& cmd, const T& data)
			{
				neb::CJsonObject ret;
				ret.Add("cmd", cmd);
				ret.Add("type", msg_type_broadcast);
				ret.Add("time", Time::system_clock_now());
				ret.Add("data", data);
				
				auto str = ret.ToString();
				_transfer.on_broadcast_do(cmd, str);
			}

			void on_broadcast_msg(Server* server, INetClientS* client, std::string& cmd, neb::CJsonObject& msg)
			{
				auto str = msg.ToString();
				_transfer.on_broadcast_do(cmd, str);

			}

			void on_client_leave(INetClientS* client)
			{
				if (client->is_ss_link())
					_transfer.del(client);

				if (client->is_login())
				{
					neb::CJsonObject msg;
					msg.Add("userId", client->userId());
					msg.Add("clientId", client->sockfd());

					broadcast("ss_msg_user_exit", msg);
				}
				
				{
					neb::CJsonObject msg;
					msg.Add("userId", client->userId());
					msg.Add("clientId", client->sockfd());

					broadcast("ss_msg_client_exit", msg);
				}

				
			}



		};
	}
}
#endif // !_doyou_io_GateServer_HPP_
