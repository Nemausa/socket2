#ifndef _doyou_io_TcpHttpClient_HPP_
#define _doyou_io_TcpHttpClient_HPP_

#include"TcpClientMgr.hpp"
#include"HttpClient.hpp"


namespace doyou {
	namespace io {
		class TcpHttpClient :public TcpClientMgr
		{
		/*	virtual Client* makeClientObj(SOCKET cSock)
			{
				return new HttpClient(cSock, _nSendBuffSize, _nRecvBuffSize);
			}*/
		};
	}
}
#endif // !_doyou_io_TcpHttpClient_HPP_
