﻿#ifndef _doyou_io_INetClient_HPP_
#define _doyou_io_INetClient_HPP_

#include"TcpWebSocketClient.hpp"
#include"CJsonObject.hpp"
#include"Config.hpp"
#include"Timestamp.hpp"
#include "INetStateCode.hpp"

namespace doyou {
	namespace io {

		//客户端数据类型
		class INetClient
		{
		private:
			//
			TcpWebSocketClient _client;
			//
			Timestamp _time2heart;
			//发送请求后_timeout_dt毫秒会触发超时
			time_t _timeout_dt = 0;
			//
			std::string _link_name;
			//
			std::string _url;
			//
			int _msgId = 0;
			//
			int _cleint_id = 0;
		private:
			//
			typedef std::function<void(INetClient*, neb::CJsonObject&)> NetEventCall;
			struct NetEventCallData 
			{
				NetEventCall callFun = nullptr;
				time_t dt = 0;
			};
			//
			std::map<std::string, NetEventCall> _map_msg_call;
			//
			std::map<int, NetEventCallData> _map_request_call;
		public:
			NetEventCall on_other_push = nullptr;
		public:
			int64_t ClientId()
			{
				return _cleint_id;
			}

			void ClientId(int n)
			{
				_cleint_id = n;
			}

			void connect(const char* link_name,const char* url)
			{
				_link_name = link_name;
				_url = url;

				int s_size = Config::Instance().getInt("nSendBuffSize", SEND_BUFF_SZIE);
				int r_size = Config::Instance().getInt("nRecvBuffSize", RECV_BUFF_SZIE);

				_client.send_buff_size(s_size);
				_client.recv_buff_size(r_size);

				//do
				_client.onopen = [this](WebSocketClientC* pWSClient)
				{
					CELLLog_Info("%s::INetClient::connect(%s) success.", _link_name.c_str(), _url.c_str());
					neb::CJsonObject json;
					json.Add("link_name", _link_name);
					json.Add("url", _url);
					on_net_msg_do("onopen", json);
				};

				_client.onmessage = [this](WebSocketClientC* pWSClient)
				{
					WebSocketHeader& wsh = pWSClient->WebsocketHeader();
					if (wsh.opcode == opcode_PONG)
					{
						CELLLog_Info("websocket server say: PONG");
						return;
					}

					auto pStr = pWSClient->fetch_data();
					std::string dataStr(pStr, wsh.len);
					CELLLog_Info("websocket server say: %s", dataStr.c_str());

					neb::CJsonObject json;
					if (!json.Parse(dataStr))
					{
						CELLLog_Error("json.Parse error : %s", json.GetErrMsg().c_str());
						return;
					}

					//响应
					int msg_type = 0;
					if (!json.Get("type", msg_type))
					{
						CELLLog_Error("not found key<type>.");
						return;
					}


					if (msg_type_resp == msg_type)
					{
						int msgId = 0;
						if (!json.Get("msgId", msgId))
						{
							CELLLog_Error("not found key<%s>.", "msgId");
							return;
						}

						on_net_msg_do(msgId, json);
						return;
					}
					

					//请求或推送消息
					if (msg_type_req == msg_type || msg_type_push == msg_type || msg_type_broadcast == msg_type)
					{
						if (on_other_push && msg_type_push == msg_type)
						{
							do 
							{
								int clientId = 0;
								if(!json.Get("clientId", clientId))
									break;
								if(clientId == _cleint_id)
									break;

								on_other_push(this, json);
								return;
							} while (false);
						}
						std::string cmd;
						if (!json.Get("cmd", cmd))
						{
							CELLLog_Error("not found key<%s>.", "cmd");
							return;
						}

						on_net_msg_do(cmd, json);
						return;
					}
				};

				_client.onclose = [this](WebSocketClientC* pWSClient)
				{
					neb::CJsonObject json;
					json.Add("link_name", _link_name);
					json.Add("url", _url);
					on_net_msg_do("onclose", json);
				};

				_client.onerror = [this](WebSocketClientC* pWSClient)
				{
					neb::CJsonObject json;
					json.Add("link_name", _link_name);
					json.Add("url", _url);
					on_net_msg_do("onerror", json);
				};

			}

			bool run(int microseconds = 1)
			{
				if (_client.isRun())
				{
					check_timeout();
					/*if (_time2heart.getElapsedSecond() > 5.0)
					{
						_time2heart.update();
						neb::CJsonObject json;
						request("cs_msg_heart", json);
					}*/
					return _client.OnRun(microseconds);
				}

				if (_client.connect(_url.c_str()))
				{
					_time2heart.update();
					CELLLog_Warring("%s::INetClient::connect(%s) success.", _link_name.c_str(), _url.c_str());
					return true;
				}
				Thread::Sleep(1000);
				return false;
			}

			void close()
			{
				_client.Close();
			}

			void timeout(time_t dt)
			{
				_timeout_dt = dt;
			}

			void check_timeout()
			{
				if (0 == _timeout_dt)
					return;

				time_t now = Time::system_clock_now();
				for (auto itr = _map_request_call.begin(); itr != _map_request_call.end();)
				{
					if (now - itr->second.dt >= _timeout_dt)
					{
						//触发超时
						neb::CJsonObject ret;
						ret.Add("state", state_code_timeout);
						ret.Add("data", "request timeout");
						itr->second.callFun(this, ret);
						//移除该请求的响应回调
						auto itrold = itr;
						++itr;
						_map_request_call.erase(itrold);
						continue;
					}

					++itr;
				}
			}

			void reg_msg_call(std::string cmd, NetEventCall call)
			{
				_map_msg_call[cmd] = call;
			}

			bool on_net_msg_do(const std::string& cmd, neb::CJsonObject& msgJson)
			{
				auto itr = _map_msg_call.find(cmd);
				if (itr != _map_msg_call.end())
				{
					itr->second(this, msgJson);
					return true;
				}
				CELLLog_Info("%s::INetClient::on_net_msg_do not found cmd<%s>.", _link_name.c_str(),cmd.c_str());
				return false;
			}

			bool on_net_msg_do(int msgId, neb::CJsonObject& msgJson)
			{
				auto itr = _map_request_call.find(msgId);
				if (itr != _map_request_call.end())
				{
					itr->second.callFun(this, msgJson);
					_map_request_call.erase(itr);
					return true;
				}
				CELLLog_Info("%s::INetClient::on_net_msg_do not found msgId<%d>.", _link_name.c_str(), msgId);
				return false;
			}

			 bool transfer(neb::CJsonObject& msg)
			{
				std::string resStr = msg.ToString();
				if (SOCKET_ERROR == _client.writeText(resStr.c_str(), resStr.length()))
				{
					CELLLog_Error("INetClient::transfer::writeText SOCKET ERROR");
					return false;
				}
				_time2heart.update();
				return true;
			}

			template<class T>
			bool request(const std::string& cmd, const T& data)
			{
				neb::CJsonObject msg;
				msg.Add("cmd", cmd);
				msg.Add("type", msg_type_req);
				msg.Add("msgId", ++_msgId);
				msg.Add("time", (int64)Time::system_clock_now());
				msg.Add("data", data);

				return transfer(msg);
			}

			template<class T>
			bool request(const std::string& cmd, const T& data, NetEventCall call)
			{
				if (!request(cmd, data))
					return false;
				
				NetEventCallData calldata;
				calldata.callFun = call;
				calldata.dt = Time::system_clock_now();
				_map_request_call[_msgId] = calldata;
				return true;
			}

			
			bool request(neb::CJsonObject& msg, NetEventCall call)
			{
				int msgId = 0;
				if (!msg.Get("msgId", msgId))
				{
					msg.Add("msgId", ++_msgId);
					
				}
				else
				{
					msg.Replace("msgId", ++_msgId);
				}


				if (!transfer(msg))
					return false;

				NetEventCallData calldata;
				calldata.callFun = call;
				calldata.dt = Time::system_clock_now();
				_map_request_call[_msgId] = calldata;
				return true;
			}

			template<class T>
			bool response(int clientId, int msgId,const T& data, int state = 0)
			{
				neb::CJsonObject ret;
				ret.Add("state", state);
				ret.Add("msgId", msgId);
				ret.Add("clientId", clientId);
				ret.Add("type", msg_type_resp);
				ret.Add("time", (int64)Time::system_clock_now());
				ret.Add("data", data);

				return transfer(ret);
			}

			template<class T>
			void response(neb::CJsonObject& msg, const T& data, int state = state_code_ok)
			{
				int clientId = 0;
				if (!msg.Get("clientId", clientId))
				{
					CELLLog_Error("not found key<%s>.", "clientId");
					return;
				}

				int msgId = 0;
				if (!msg.Get("msgId", msgId))
				{
					CELLLog_Error("not found key<%s>.", "msgId");
					return;
				}
				
				response(clientId, msgId, data, state);
			}

			template<class T>
			void resp_error(neb::CJsonObject& msg, const T& data, int state = state_code_error)
			{
				response(msg, data, state);
			}

			template<class T>
			bool push(int clientId, const std::string& cmd,const T& data, int state = state_code_ok)
			{
				neb::CJsonObject ret;
				ret.Add("state", state);
				ret.Add("cmd", cmd);
				ret.Add("clientId", clientId);
				ret.Add("type", msg_type_push);
				ret.Add("time", (int64)Time::system_clock_now());
				ret.Add("data", data);

				return transfer(ret);
			}

			template<class T>
			bool push(const std::vector<int64_t>& client, const std::string& cmd, const T& data, int state = state_code_ok)
			{
				neb::CJsonObject ret;
				ret.Add("state", state);
				ret.Add("cmd", cmd);
				ret.Add("type", msg_type_push_pro);
				ret.Add("time", (int64)Time::system_clock_now());
				ret.Add("data", data);
				ret.AddEmptySubArray("clients");

				auto length = client.size();
				for (size_t i = 0; i < length; i++)
				{
					ret["clients"].Add(client[i]);
				}

				return transfer(ret);
			}

			template<class T>
			bool broadcast(const std::string& cmd, const T& data)
			{
				neb::CJsonObject ret;
				ret.Add("cmd", cmd);
				ret.Add("type", msg_type_broadcast);
				ret.Add("time", (int64)Time::system_clock_now());
				ret.Add("data", data);

				return transfer(ret);
			}
		};
	}
}
#endif // !_doyou_io_INetClient_HPP_
