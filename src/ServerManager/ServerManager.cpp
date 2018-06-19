//
// Created by 朱熙 on 2018/6/1.
//

#include "ServerManager.h"
ServerManager::ServerManager(): scl(keywords::channel = "ServerManager_class") {
    m_mqRecvBuff.resize(MQ_RECV_BUF_SIZE);
}

ServerManager::~ServerManager() {
    delete m_pTcpServer;
    m_pTcpServer = nullptr;

    delete m_pRobotStateServer;
    m_pRobotStateServer = nullptr;

    delete m_pParser;
    m_pParser = nullptr;
}

void ServerManager::createManager() {
    m_pTcpServer = new TcpServer(this);
    m_pRobotStateServer = new RobotStateServer(this);
    m_pParser = new Parser(this);
}

void ServerManager::startManager() {
    m_pTcpServer->run();
    m_pRobotStateServer->run();
    //m_pParser->parse();

    BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ << " all server start.";
    m_pTcpThread->join();
    m_pRobotStateThread->join();
}





