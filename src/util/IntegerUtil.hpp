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
     * Int转bool数组,取低位，长度为len
     * @param i int数据
     * @param b bool数组指针
     * @parma len 转换为bool数组的长度
     */
    static void Integer2Binary(int integer, bool * b, int len) {
        for (int i=0; i<len; i++) {
            b[len-1-i] = ((integer>>i)&1);
        }
    }

    /***
     * bool数组转int
     * @param b    待转换bool数组
     * @param len  数组长度
     * @return     转换后的int值
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
