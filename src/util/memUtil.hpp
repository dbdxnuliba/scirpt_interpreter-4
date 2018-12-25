//
// Created by 朱熙 on 2018/6/21.
//

#ifndef PARSERSERVER_MEMUTIL_H
#define PARSERSERVER_MEMUTIL_H

#include <iostream>

#define ZX_MEMCPY(des, src, len) memUtil::zxMemcpy(des, src, len);
#define ZX_DELETE(des) memUtil::zxDelete(des);


class memUtil {
public:

    /**
     * 内存拷贝，避免空指针
     * @param des 拷贝地址
     * @param src 被拷贝地址
     * @param len 数据长度
     */
    static void zxMemcpy(char* &des, char* &&src, int len) {
        if(des) {
            delete des;
            des = nullptr;
        }

        if (len > 0) {
            des = new char[len];
            memcpy(des, src, len);
        }

    }

    /**
     * 释放内存，避免空指针
     * @param des 释放内存地址
     */
    static void zxDelete(char* &des) {
        if (des) {
            delete des;
            des = nullptr;
        }
    }
};

#endif //PARSERSERVER_MEMUTIL_H
