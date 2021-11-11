//
//  ISFStringUtils_2.h
//  ORAGE
//
//  Created by Vincent Evrard on 10/11/21.
//

#ifndef ISFStringUtils_2_h
#define ISFStringUtils_2_h

#include "exprtk/exprtk.hpp"

namespace ISF {
    using namespace std;

struct ISFVal;
//struct Range;
    //    this function parses a std::string as a bool val, and returns either an ISFNullVal (if the std::string couldn't be decisively parsed) or an ISFBoolVal (if it could)
    ISFVal ParseStringAsBool(const std::string & n){
        string            testStrings[4] = { string("yes"), string("true"), string("no"), string("false") };
        for (int testStrIdx=0; testStrIdx<4; ++testStrIdx)    {
            if (testStrings[testStrIdx].size() == n.size())    {
                bool        match = true;
                auto        itA = testStrings[testStrIdx].begin();
                auto        itB = n.begin();
                for (itA=testStrings[testStrIdx].begin(), itB=n.begin(); itA!=testStrings[testStrIdx].end(); ++itA, ++itB)    {
                    if (tolower(*itA) != (*itB))    {
                        match = false;
                        break;
                    }
                }
                if (match)    {
                    if (testStrIdx < 2)
                        return ISFBoolVal(true);
                    else
                        return ISFBoolVal(false);
                }
            }
        }
        return ISFNullVal();
    }
    //    this function evaluates the passed std::string and returns a null ISFVal (if the std::string couldn't be evaluated) or a float ISFVal (if it could)
    ISFVal ISFValByEvaluatingString(const std::string & n, const std::map<std::string, double> & inSymbols=std::map<std::string,double>()){
        exprtk::expression<double>        expr;
        exprtk::symbol_table<double>    table;
        exprtk::parser<double>            parser;
        
        size_t            inSymbolsCount = inSymbols.size();
        //double            tmpVars[inSymbolsCount];
        double            *tmpVars = (double*)malloc(sizeof(double)*inSymbolsCount);
        
        if (inSymbolsCount > 0)    {
            //vector<double>        tmpVars;
            //tmpVars->reserve(inSymbolsCount);
            int                i=0;
            for (auto const & it : inSymbols)    {
                //double        tmpDouble = it.second;
                //table.add_variable(it.first.c_str(), tmpDouble);
                tmpVars[i] = it.second;
                table.add_variable(it.first.c_str(), tmpVars[i]);
                ++i;
            }
            expr.register_symbol_table(table);
        }
        free(tmpVars);
        tmpVars = nullptr;
        
        parser.compile(n, expr);
        
        return ISFFloatVal(expr.value());
    }
    
    std::string TrimWhitespace(const std::string & inBaseStr){
        Range        wholeRange(0, inBaseStr.size());
        //cout << "\t" << __PRETTY_FUNCTION__ << "- FINISHED\n";
        //return inBaseStr.substr(wholeRange.loc, wholeRange.len);
        
        Range        trimmedRange = wholeRange;
        size_t            tmpPos = inBaseStr.find_last_not_of(" \t\r\n");
        if (tmpPos != string::npos)
            trimmedRange.len = tmpPos+1;
        tmpPos = inBaseStr.find_first_not_of(" \t\r\n");
        if (tmpPos != string::npos)
            trimmedRange.loc = tmpPos;
        trimmedRange.len -= trimmedRange.loc;
        if (wholeRange == trimmedRange)
            return inBaseStr;
        return inBaseStr.substr(trimmedRange.loc, trimmedRange.len);
    }
    
    //    this function parses a function call from a std::string, dumping the strings of the function arguments
    //    to the provided array.  returns the size of the function std::string (from first char of function call
    //    to the closing parenthesis of the function call)
    Range LexFunctionCall(const std::string & inBaseStr, const Range & inFuncNameRange, std::vector<std::string> & outVarArray){
        //cout << __PRETTY_FUNCTION__ << endl;
        
        if (inFuncNameRange.len==0 || inFuncNameRange.max()>inBaseStr.size())
            return Range(0,0);
        //if (inFuncLen==0 || ((inFuncLen+inFuncLen)>inBaseStr.size()))
        //    return Range(0,0);
        size_t        searchStartIndex = inFuncNameRange.max();
        size_t        lexIndex = searchStartIndex;
        size_t        openGroupingCount = 0;
        Range    substringRange(0,0);
        substringRange.loc = searchStartIndex + 1;
        do    {
            switch (inBaseStr[lexIndex])    {
                case '(':
                case '{':
                    ++openGroupingCount;
                    break;
                case ')':
                case '}':
                    --openGroupingCount;
                    if (openGroupingCount == 0)    {
                        substringRange.len = lexIndex - substringRange.loc;
                        string        groupString = inBaseStr.substr(substringRange.loc, substringRange.len);
                        groupString = TrimWhitespace(groupString);
                        outVarArray.push_back(groupString);
                    }
                    break;
                case ',':
                    if (openGroupingCount == 1)    {
                        substringRange.len = lexIndex - substringRange.loc;
                        string        groupString = inBaseStr.substr(substringRange.loc, substringRange.len);
                        groupString = TrimWhitespace(groupString);
                        outVarArray.push_back(groupString);
                        substringRange.loc = lexIndex + 1;
                    }
                    break;
            }
            ++lexIndex;
        } while (openGroupingCount > 0);
        Range    rangeToReplace = Range(inFuncNameRange.loc, lexIndex-inFuncNameRange.loc);
        return rangeToReplace;
    }

    void FindAndReplaceInPlace(std::string & inSearch, std::string & inReplace, std::string & inBase){
        size_t        pos = 0;
        while ((pos=inBase.find(inSearch, pos)) != string::npos)    {
            inBase.replace(pos, inSearch.length(), inReplace);
            pos += inReplace.length();
        }
    }
    void FindAndReplaceInPlace(const char * inSearch, const char * inReplace, std::string & inBase){
        string        is(inSearch);
        string        ir(inReplace);
        FindAndReplaceInPlace(is, ir, inBase);
    }


    std::string FullPath(const std::string & inRelativePath){
    #if _WIN32
        char        *outPath = _fullpath(NULL, inRelativePath.c_str(), 0);
        return std::string(outPath);
    #else
        char        *outPath = realpath(inRelativePath.c_str(), NULL);
        return std::string(outPath);
    #endif
    }

}
#endif /* ISFStringUtils_2_h */
