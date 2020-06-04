#ifndef _doyou_io_GateServer_HPP_
#define _doyou_io_GateServer_HPP_

#include "INetServer.hpp"

namespace doyou {
	namespace io {
		class GateServer
		{
		private:
			INetServer _netserver;
		public:
			void Init()
			{
				_netserver.Init();
				_netserver.reg_msg_call("cs_msg_heart", std::bind(&GateServer::on_net_msg_heart,this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			}

			void Close()
			{
				_netserver.Close();
			}

		private:
			void on_net_msg_heart(Server* pserver, INetClientS* client, neb::CJsonObject& msg)
			{
				CELLLog_Info("GateServer::on_net_msg_heart");
				client->response(msg, "666");

			}
		};
	}
}
#endif // !_doyou_io_GateServer_HPP_
