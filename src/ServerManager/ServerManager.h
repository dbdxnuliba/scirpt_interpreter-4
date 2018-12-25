//
// Created by 朱熙 on 2018/6/1.
//

#ifndef PARSERSERVER_SERVERMANAGER_H
#define PARSERSERVER_SERVERMANAGER_H

#include "../tcp/tcpServer.h"
#include "../robotStateServer/RobotStateServer.h"
#include "../Parser/Parser.h"

class TcpServer;
class RobotStateServer;
class Parser;


/**
 * Server Manager
 * 用于管理所有的server模块，包括TCPserver、RobotState server
 */
class ServerManager {

public:
    ServerManager();
    ~ServerManager();

    /**
     * 创建管理器
     */
    void createManager();
    /**
     * 启动管理器
     */
    void startManager();


public:

    mutex m_mqRecvBuffMutex;            //命令接收boost::mq缓存互斥锁
    buffer_type m_mqRecvBuff;           //boost::mq接收缓存

    mutex m_mqSendBuffMutex;            //命令发送boost::mq缓存互斥锁
    buffer_type m_mqSendBuff;           //boost::mq发送缓存

    thread *m_pTcpThread;               //tcp线程
    thread *m_pRobotStateThread;        //robotstate线程

    TcpServer *m_pTcpServer;
    RobotStateServer *m_pRobotStateServer;
    Parser *m_pParser;                  //脚本解释器


    src::severity_channel_logger<severity_level, std::string> scl;      //日志

};


#endif //PARSERSERVER_SERVERMANAGER_H
