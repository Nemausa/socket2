/*********************************************************************************************
 *  @file:    UserManager.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/6/9 2:50
**********************************************************************************************/

#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include <cstdint>
#include <string>
#include <map>

namespace doyou
{
	namespace io
	{
		class UserManager
		{
		private:
			class User
			{
			public:
				std::string token;
				int64_t userId = 0;
				int clientId = 0;
			};
			typedef User* UserPtr;
			//ͨ��token������ѯuser
			std::map<std::string, UserPtr>_token2user;
			//ͨ��userId������ѯuser
			std::map<int64_t, UserPtr>_userId2user;
			//ͨ��clientId������ѯuser
			std::map<int, UserPtr>_clientId2user;
		public:

			bool add(const std::string& token, int64_t userId, int clientId)
			{
				if (get_by_token(token) || get_by_userId(userId) || get_by_clientId(clientId))
				{
					return false;
				}

				UserPtr user = new User();
				user->token = token;
				user->userId = userId;
				user->clientId = clientId;

				_token2user[token] = user;
				_userId2user[userId] = user;
				_clientId2user[clientId] = user;

				return true;
			}

			void remove(UserPtr user)
			{
				if (!user)
					return;
				_token2user.erase(user->token);
				_userId2user.erase(user->userId);
				_clientId2user.erase(user->clientId);

				delete user;
			}

			UserPtr get_by_token(const std::string token)
			{
				auto itr = _token2user.find(token);
				if (itr == _token2user.end())
					return nullptr;
				return itr->second;
			}

			UserPtr get_by_userId(int64_t userId)
			{
				auto itr = _userId2user.find(userId);
				if (itr == _userId2user.end())
					return nullptr;
				return itr->second;
			}

			UserPtr get_by_clientId(int clientId)
			{
				auto itr = _clientId2user.find(clientId);
				if (itr == _clientId2user.end())
					return nullptr;
				return itr->second;
			}

		};
	}
}

#endif  //_USERMANAGER_H_

