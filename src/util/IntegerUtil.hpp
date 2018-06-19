//
// Created by 朱熙 on 2018/6/15.
//

#ifndef PARSERSERVER_INTEGERUTIL_H
#define PARSERSERVER_INTEGERUTIL_H
#include <iostream>
#include <vector>
#include <sstream>

class IntegerUtil {

public:
    /**
     * Int转bool数组,取低位，长度位len
     * @param i
     * @param b
     * @parma len
     */
    static void Integer2Binary(int integer, bool * b, int len) {
        for (int i=0; i<len; i++) {
            b[len-1-i] = ((integer>>i)&1);
        }
    }

    /***
     * bool数组转int
     * @param b
     * @param len
     * @return
     */
    static int Binary2Integer(bool * b, int len) {
        std::stringstream ss;
        for (int i = 0; i < len; ++i) {
            ss << b[i];
        }
        return std::stoi(ss.str(), 0 ,2);
    }


};


#endif //PARSERSERVER_INTEGERUTIL_H
