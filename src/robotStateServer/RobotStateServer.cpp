//
// Created by 朱熙 on 2018/6/5.
//

#include "RobotStateServer.h"
#include "../robot_state.h"

RobotStateServer::RobotStateServer(ServerManager *pServerManager) : m_isStop(false) {
    m_pServerManager = pServerManager;

    shared_memory_object::remove("RobotState");
    m_shm = shared_memory_object(create_only, "RobotState", read_write);
    m_shm.truncate(SHARE_MEMORY_SIZE);
    m_region = mapped_region(m_shm, read_write);

    memset(m_region.get_address(), 0, m_region.get_size());

    m_shareData = new char[SHARE_MEMORY_SIZE];

}

void robotState_thread(RobotStateServer * pServer) {
    RobotStateServer *pRobotStateServer = pServer;
    while (!pRobotStateServer->isStop()) {

        //读取共享内存数据
        pRobotStateServer->getShareData();

        //发送共享内存数据
        pRobotStateServer->m_pServerManager->m_pTcpServer->send(pRobotStateServer->m_shareData);

        usleep(100);
        //sleep(2);
    }
}

void RobotStateServer::run() {
    m_isStop = false;
    m_pServerManager->m_pRobotStateThread = new thread(robotState_thread, this);
}

void RobotStateServer::stop() {
    m_isStop = true;
}

bool RobotStateServer::isStop() {
    return m_isStop;
}

int RobotStateServer::getShareData() {

    memcpy(m_shareData, m_region.get_address(), m_region.get_size());
    return 1;
}

RobotStateServer::~RobotStateServer() {
    delete[] m_shareData;
    m_shareData = nullptr;
}






