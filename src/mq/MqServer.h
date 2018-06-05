//
// Created by 朱熙 on 2018/6/1.
//

#ifndef PARSERSERVER_MQSERVER_H
#define PARSERSERVER_MQSERVER_H

#include "../../include/global.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include "../ServerManager/ServerManager.h"

using namespace boost::interprocess;

class ServerManager;

class MqServer {

public:
    MqServer(ServerManager *pServerManager);
    ~MqServer();

    void run();
    void stop();

    bool isStop();

public:
    int recvFromMq();
    int sendToMq();

private:
    int isWholeMsg();                       //判断是否存在完整的一条消息，如果存在返回总长度，不存在返回0

public:
    message_queue *m_mqSend;
    message_queue *m_mqRecv;

    ServerManager *m_pServerManager;

private:
    bool m_bIsStop;                         //mq线程是否终止
    deque<char> m_recvBuff;                       //接收缓存


};


#endif //PARSERSERVER_MQSERVER_H
