/*********************************************************************************************
 *  @file:    GroupManager.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/6/12 4:37
**********************************************************************************************/

#ifndef _GROUPMANAGER_H_
#define _GROUPMANAGER_H_
#include <vector>
#include "INetClientS.hpp"

namespace doyou
{
	namespace io
	{
		class GroupManager
		{
			class Group
			{
			public:
				void add(int64_t client)
				{
					auto itr = std::find(_Group.begin(), _Group.end(), client);
					if (itr == _Group.end())
					{
						_Group.push_back(client);
					}
				}

				void del(int64_t client)
				{
					auto itr = std::find(_Group.begin(), _Group.end(), client);
					if (itr != _Group.end())
					{
						_Group.erase(itr);
					}
				}

				int key()
				{
					return _key;
				}

				void key(int key)
				{
					 _key = key;;
				}
				
				bool empty()
				{
					return _Group.empty();
				}

			private:
				std::vector<int64_t> _Group;
				int _key = 0;
			};
		private:
			std::map<int, Group> _map_member;
			int _index_id = 10000;
		public:
			bool create(int& group_id, int group_key, int64_t client)
			{
				//如果group_值为0
				//由GroupManager分配一个group_id
				if (group_id <= 0)
				{
					group_id = ++_index_id;
				}

				//先查找group_id是否已经存在
				auto itr = _map_member.find(group_id);
				if (itr != _map_member.end())
				{
					//join可能失败
					return join(group_id, group_key, client);
				}
				else
				{
					Group a;
					a.add(client);
					a.key(group_key);
					_map_member[group_id] = a;
				}

				return true;
			}

			bool join(int group_id, int group_key, int64_t client)
			{
				auto itr = _map_member.find(group_id);
				if (itr == _map_member.end())
				{
					return false;
				}

				if (itr->second.key() != group_key)
					return false;

				return true;
			}

			bool del(int group_id, int64_t client)
			{
				auto itr = _map_member.find(group_id);
				if (itr == _map_member.end())
					return false;

				itr->second.del(client);
				if (itr->second.empty())
					_map_member.erase(itr);
			}
		};
	}
}


#endif  //_GROUPMANAGER_H_


