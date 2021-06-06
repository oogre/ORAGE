#ifndef Tools_h
#define Tools_h

#include <string>
#include <vector>
#include <sstream>

class Tools{
	public :
		static void split(std::string input, char splitter, std::string * before, std::string * after){
            std::string::size_type pos = input.find(splitter);
			if (pos != std::string::npos)
			{
				(*before) = input.substr(0, pos);
				(*after) = input.substr(pos+1, input.length());
			}
			else
			{
				(*before) = input;
			}
        }

};

#endif /* Math_h */
