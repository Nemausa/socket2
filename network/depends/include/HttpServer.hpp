/*********************************************************************************************
 *  @file:    HttpServer.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/5/25 21:37
**********************************************************************************************/

#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_


#include "TcpServerMgr.hpp"
#include "HttpClient.hpp"

namespace doyou {
	namespace io {
		class TcpHttpServer : public TcpServerMgr
		{
		public:
			virtual Client *makeClientOjb(SOCKET cSock)
			{
				return new HttpClients(cSock, _nSendBuffSize, _nRecvBuffSize);
			}

		};
	}
}
#endif  //_HTTPSERVER_H_
