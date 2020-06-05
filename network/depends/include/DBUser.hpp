/*********************************************************************************************
 *  @file:    DBUser.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/6/6 2:14
**********************************************************************************************/

#ifndef _DBUSER_H_
#define _DBUSER_H_

#include "Log.hpp"


namespace doyou
{
	namespace io
	{
		class DBUser
		{
		public:
			bool has_username(const std::string& username)
			{
				return false;
			}

			bool has_nickname(const std::string& nickname)
			{
				return false;
			}

			int64_t add_user(const std::string& username, const std::string& password, const std::string& nickname, int sex)
			{
				return 0;
			}
		};
	}
}


#endif  //_DBUSER_H_


