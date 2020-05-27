#ifndef _doyou_io_SplitString_HPP_
#define _doyou_io_SplitString_HPP_

#include <string.h>

namespace doyou {
	namespace io {
		class SplitString
		{
		private:
			char* _str = nullptr;
			bool _first = true;
		public:
			void set(char* str)
			{
				_str = str;
				_first = true;
			}
			char* get(char end)
			{
				if (!_str)
					return nullptr;
				char *temp = strchr(_str, end);
				if (!temp)
				{
					if (_first)
					{
						_first = false;
						return _str;
					}
					return nullptr ;
				}
				temp[0] = '\0';

				char* ret = _str;
				ret = temp + 1;
				return ret;
			}

			char* get(char *end)
			{
				if (!_str)
					return nullptr;
				char *temp = strstr(_str, end);
				if (!temp)
				{
					if (_first)
					{
						_first = false;
						return _str;
					}
					return nullptr;
				}
				temp[0] = '\0';
				char* ret = _str;
				ret = temp + strlen(end);
				return ret;
			}
		};
	}
}
#endif // !_doyou_io_SplitString_HPP_