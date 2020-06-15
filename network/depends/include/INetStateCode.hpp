/*********************************************************************************************
 *  @file:    INetStateCode.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/6/9 5:53
**********************************************************************************************/

#ifndef _INETSTATECODE_H_
#define _INETSTATECODE_H_

namespace doyou
{
	namespace io
	{
#define msg_type_req  1
#define msg_type_resp 2
#define msg_type_push 3
#define msg_type_broadcast 4
#define msg_type_push_pro 5

#define state_code_ok 0
#define state_code_error 1
#define state_code_timeout 2
#define state_code_undefine_cmd 3
#define state_code_server_busy 4
#define state_code_server_off 5

	}
}

#endif  //_INETSTATECODE_H_


