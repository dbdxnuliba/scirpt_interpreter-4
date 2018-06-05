//
// Created by 朱熙 on 2018/6/1.
//

#ifndef PARSERSERVER_SERVERMANAGER_H
#define PARSERSERVER_SERVERMANAGER_H

#include "../tcp/tcpServer.h"
#include "../robotStateServer/RobotStateServer.h"

class TcpServer;
class RobotStateServer;

class ServerManager {

public:
    ServerManager();
    ~ServerManager();

    void createManager();
    void startManager();


public:

    mutex m_mqRecvBuffMutex;
    buffer_type m_mqRecvBuff;

    mutex m_mqSendBuffMutex;
    buffer_type m_mqSendBuff;

    thread *m_pTcpThread;
    thread *m_pRobotStateThread;

    TcpServer *m_pTcpServer;
    RobotStateServer *m_pRobotStateServer;


    src::severity_channel_logger<severity_level, std::string> scl;

};


#endif //PARSERSERVER_SERVERMANAGER_H
