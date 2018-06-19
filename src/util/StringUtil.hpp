//
// Created by 朱熙 on 2018/6/8.
//

#ifndef PARSERSERVER_STRINGUTIL_H
#define PARSERSERVER_STRINGUTIL_H

#include <iostream>
#include <vector>

class StringUtil {

public:
    static std::vector<std::string> split(const  std::string& s, const std::string& delim)
    {
        std::vector<std::string> elems;
        size_t pos = 0;
        size_t len = s.length();
        size_t delim_len = delim.length();
        if (delim_len == 0) return elems;
        while (pos < len)
        {
            int find_pos = s.find(delim, pos);
            if (find_pos < 0)
            {
                elems.push_back(s.substr(pos, len - pos));
                break;
            }
            elems.push_back(s.substr(pos, find_pos - pos));
            pos = find_pos + delim_len;
        }
        return elems;
    }

//通过符号进行分割字符串  int/a==5 => [int, /, a, ==, 5] 注意'.'按数字算
    static std::vector<std::string> splitWithFlag(const  std::string& s)
    {
        std::vector<std::string> elems;
        bool isFlag = false;
        std:string e;
        for (int i=0; i<s.size(); i++) {
            if (isalpha(s[i]) || isdigit(s[i]) || s[i]=='.') {
                if (!isFlag) {
                    e += s[i];
                }
                else {
                    elems.push_back(e);
                    e = s[i];
                }
                isFlag = false;
            }
            else {
                if (!isFlag) {
                    elems.push_back(e);
                    e = s[i];
                }
                else {
                    e += s[i];
                }
                isFlag = true;
            }
        }

        elems.push_back(e);

        return elems;
    }

//获得flag前面的字符串
    static std::string getHeadString(std::string& s, const std::string& strFlag) {
        size_t find_str_postion;
        find_str_postion = s.find(strFlag);

        if(find_str_postion<s.size())
        {
            std::string result=s.substr(0,find_str_postion-1);
            return result;
        }

        return "";
    }

//获得flag后面的字符串
    static std::string getTailString(std::string& s, const std::string& strFlag) {
        size_t find_str_postion;
        find_str_postion = s.find(strFlag);

        if(find_str_postion<s.size())
        {
            std::string result=s.substr(find_str_postion+1, s.size());
            return result;
        }

        return "";
    }

//获得两个flag中间字符串
    static std::string getTailString(std::string& s, const std::string& strFrontFlag, const std::string& strBackFlag) {
        size_t frontPostion, backPosition;
        frontPostion = s.find(strFrontFlag);
        backPosition = s.find(strBackFlag);

        if(frontPostion<s.size() || backPosition<s.size())
        {
            std::string result = s.substr(frontPostion+1, backPosition-1);
            return result;
        }

        return "";
    }

};


#endif //PARSERSERVER_STRINGUTIL_H
