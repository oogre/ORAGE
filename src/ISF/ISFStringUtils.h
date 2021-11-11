//
//  ISFStringUtils.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFStringUtils__h
#define ISFStringUtils__h

#include "ISFRange.h"

namespace ISF {
    using namespace std;
    
    //    functions for doing some basic path manipulation
    std::vector<std::string> PathComponents(const std::string & n){
        //cout << __PRETTY_FUNCTION__ << endl;
        vector<string>        returnMe(0);
        //    count the # of the path delineators in the string, reserve space in the vector we're returning
#if defined(VVGL_SDK_WIN)
        size_t                delimCount = count(n.begin(), n.end(), '\\') + 1;
#else
        size_t                delimCount = count(n.begin(), n.end(), '/') + 1;
#endif
        returnMe.reserve(delimCount);
        //    get the ptr to the string data
        char                *inString = const_cast<char*>(n.data());
        //    tokenize the string data, creating strings in the vector we'll be returning
#if defined(VVGL_SDK_WIN)
        const char            *delimPtr = "\\";
#else
        const char            *delimPtr = "/";
#endif
        char                *token = strtok(inString, delimPtr);
        while (token != nullptr)    {
            //cout << "\ttoken: " << token << endl;
            returnMe.push_back(string(token));
            token = strtok(NULL, delimPtr);
        }
        return returnMe;
    }
    std::string LastPathComponent(const std::string & n){
#if defined(VVGL_SDK_WIN)
        size_t        lastSlashIndex = n.find_last_of('\\');
#else
        size_t        lastSlashIndex = n.find_last_of('/');
#endif
        if (lastSlashIndex == string::npos)
            return n;
        else if (lastSlashIndex == (n.length()-1))
            return n;
        return n.substr(lastSlashIndex+1);
    }
    std::string StringByDeletingLastPathComponent(const std::string & n){
#if defined(VVGL_SDK_WIN)
        size_t        lastSlashIndex = n.find_last_of('\\');
#else
        size_t        lastSlashIndex = n.find_last_of('/');
#endif
        if (lastSlashIndex == string::npos)
            return n;
        else if (lastSlashIndex == 0)
            return n;
        return n.substr(0, lastSlashIndex);
    }
    std::string PathFileExtension(const std::string & n){
        size_t            extensionIndex = n.find_last_of(".");
        if (extensionIndex == string::npos || extensionIndex == 0)
            return n;
        else if (extensionIndex == (n.length()-1))
            return n;
        return n.substr(extensionIndex+1);
    }
    std::string StringByDeletingExtension(const std::string & n){
        size_t            extensionIndex = n.find_last_of(".");
        if (extensionIndex == string::npos)
            return n;
        return n.substr(0, extensionIndex);
    }
    std::string StringByDeletingLastAndAddingFirstSlash(const std::string & n){
        if (n.size()<1)
            return string("");
        
        int            tmpLen = int(n.size());
#if defined(VVGL_SDK_WIN)
        bool        hasFirst = (n[0] == '\\') ? true : false;
        bool        hasLast = (n[tmpLen - 1] == '\\') ? true : false;
#else
        bool        hasFirst = (n[0]=='/') ? true : false;
        bool        hasLast = (n[tmpLen-1]=='/') ? true : false;
#endif
        string        returnMe = n;
        if (hasLast)    {
            if (tmpLen > 1) {
                returnMe.pop_back();
            }
        }
        if (!hasFirst)    {
#if defined(VVGL_SDK_WIN)
            returnMe.insert(0, 1, '\\');
#else
            returnMe.insert(0, 1, '/');
#endif
        }
        return returnMe;
    }
    std::string StringByDeletingLastSlash(const std::string & n){
        if (n.size()<1)
            return string("");
        
        //bool        hasFirst = (n[0]=='/') ? true : false;
        int            tmpLen = int(n.size());
#if defined(VVGL_SDK_WIN)
        bool        hasLast = (n[tmpLen - 1] == '\\') ? true : false;
#else
        bool        hasLast = (n[tmpLen-1]=='/') ? true : false;
#endif
        string        returnMe = n;
        if (hasLast)    {
            if (tmpLen > 1) {
                returnMe.pop_back();
            }
        }
        //if (!hasFirst)
        //    returnMe.insert(0, 1, '/');
        return returnMe;
    }
    bool CaseInsensitiveCompare(const std::string & a, const std::string & b){
        size_t        sz = a.size();
        if (b.size() != sz)
            return false;
        for (unsigned int i = 0; i < sz; ++i)    {
            if (tolower(a[i]) != tolower(b[i])) {
                return false;
            }
        }
        return true;
    }
    //    this function returns a std::string instance created by passing a c-style format std::string + any number of arguments
    std::string FmtString(const char * fmt, ...){
        va_list            args;
        va_start(args, fmt);
        int                tmpLen = vsnprintf(nullptr, 0, fmt, args) + 1;
        va_end(args);
        
        if (tmpLen < 1)
            return string("");
        
        va_start(args, fmt);
        char            *buf = (char*)malloc(tmpLen*sizeof(char));
        memset(buf, 0, tmpLen);
        vsnprintf(buf, tmpLen, fmt, args);
        va_end(args);
        string            returnMe = string(buf);
        free(buf);
        return returnMe;
    }
    //    this function returns the number of lines in the passed std::string
    int NumLines(const std::string & n){
        int            returnMe = 0;
        for (const char & c : n)    {
            switch (c)    {
                case '\r':
                case '\n':
                    ++returnMe;
                    break;
                default:
                    break;
            }
        }
        return returnMe;
    }
}//ISF

#endif /* ISFStringUtils__h */
