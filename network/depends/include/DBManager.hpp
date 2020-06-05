/*********************************************************************************************
 *  @file:    DBManager.hpp
 *  @version: ver 1.0
 *  @author:  kevin
 *  @brief:  
 *  @change:
 *  @email:   tappanmorris@outlook.com
 *  date:     2020/6/6 3:08
**********************************************************************************************/

#ifndef _DBMANAGER_H_
#define _DBMANAGER_H_

#include "Log.hpp"
#include "CppSQLite3.h"


namespace doyou
{
	namespace io
	{
		class DBManager
		{
		protected:
			CppSQLite3DB _db;
			std::string _db_name;
		public:
			bool open(const char* db_name)
			{
				_db_name = db_name;
				try 
				{
					_db.open(db_name);
				}
				catch(CppSQLite3Exception& e)
				{
					CELLLog_Error("DBManager::open(%s) error:%s", _db_name, e.errorMessage());
					return false;
				}
				
				return true;
			}

			bool close()
			{
				try
				{
					_db.close();
				}
				catch (CppSQLite3Exception& e)
				{
					CELLLog_Error("DBManager::close(%s) error:%s", _db_name.c_str(), e.errorMessage());
					return false;
				}
				return true;
			}

			bool findByKV(const char* table, const char* k, const char* v)
			{


				return false;
			}

			bool create_table_info()
			{
				auto sql_user_info =
					"CREATE TABLE user_info ( \
					id          INTEGER PRIMARY KEY,\
					userId      INTEGER UNIQUE,\
					username    TEXT    UNIQUE,\
					password    TEXT,\
					nickname    TEXT    UNIQUE,\
					sex         INTEGER,\
					state       INTEGER,\
					create_date INTEGER\
					);\
					";

				try
				{
					if (!_db.tableExists("user_info"))
					{
						_db.execDML(sql_user_info);
					}
				}
				catch (CppSQLite3Exception& e)
				{
					CELLLog_Error("DBManager::create table(%s) error:%s", _db_name, e.errorMessage());
				}
				
				return true;
			}
		};
	}
}


#endif  //_DBMANAGER_H_


