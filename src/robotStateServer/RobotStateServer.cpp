//
// Created by 朱熙 on 2018/6/5.
//

#include "RobotStateServer.h"
#include "../Parser/vm/robot_state.h"

RobotStateServer::RobotStateServer(ServerManager *pServerManager) : m_isStop(false), scl(keywords::channel = "RobotStateServer_class") {
    m_pServerManager = pServerManager;

    shared_memory_object::remove("RobotState");
    m_shm = shared_memory_object(create_only, "RobotState", read_write);

    m_shm.truncate(SHARE_MEMORY_SIZE);
    m_region = mapped_region(m_shm, read_write);

    memset(m_region.get_address(), 0, m_region.get_size());

    m_sendTimestep = -1;

}

#ifdef _DEBUG
    int test = 0;
#endif

void robotState_thread(RobotStateServer * pServer) {
    RobotStateServer *pRobotStateServer = pServer;
    RobotState *pRobotState = pRobotStateServer->m_pServerManager->m_pParser->m_vm.m_robotState;
    Parser * pParser = pRobotStateServer->m_pServerManager->m_pParser;
    char * shareData = new char[SHARE_MEMORY_SIZE];
    char * sendData = new char[SHARE_MEMORY_SIZE];
    while (!pRobotStateServer->isStop()) {

        //读取共享内存数据
        pRobotStateServer->getShareData(shareData);

        //共享内存数据转为robot state对象
        pRobotState->unpackFromMem((uint8_t*)shareData, SHARE_MEMORY_SIZE);

        //todo 更新当前vm中dis dos
        IntegerUtil::Integer2Binary(pRobotState->mb_data_.digitalInputBits, pParser->m_vm.m_globalParams.dis, 16);
        IntegerUtil::Integer2Binary(pRobotState->mb_data_.digitalOutputBits, pParser->m_vm.m_globalParams.dos, 16);

#ifdef _DEBUG
        BOOST_LOG_SEV(pRobotStateServer->scl, info) << "receive dis: " << pRobotState->mb_data_.digitalInputBits << ", receive dos: " << pRobotState->mb_data_.digitalOutputBits;
        for (int i=0; i<16; i++)
            BOOST_LOG_SEV(pRobotStateServer->scl, info) << "current dis" << i << ": " << pParser->m_vm.m_globalParams.dis[i];
        for (int i=0; i<16; i++)
            BOOST_LOG_SEV(pRobotStateServer->scl, info) << "current dos" << i << ": " << pParser->m_vm.m_globalParams.dos[i];
#endif
        // 添加当前脚本id
        pRobotState->labelMessage_.id = pParser->m_vm.m_curScriptId;

#ifdef _DEBUG
        pRobotState->robotcommMessage_.warningLevel = test % 10000000;
        test++;
#endif

        //robot state转为网络发送包
        int len = pRobotState->pack((uint8_t*)sendData);

        // 添加返回值 size + string
        uint8_t size = pParser->m_vm.m_curResult.length();
        memcpy(sendData+len, &size, sizeof(size));
        memcpy(sendData+len+sizeof(size), pParser->m_vm.m_curResult.c_str(), pParser->m_vm.m_curResult.length());

        //将包的总长度加上errorcode size
        unsigned int totalLength = len + sizeof(size) + size;
        totalLength = htonl(totalLength);
        memcpy(sendData, &totalLength, sizeof(totalLength));

        //发送共享内存数据
        pRobotStateServer->m_pServerManager->m_pTcpServer->send(sendData, len+sizeof(size)+pParser->m_vm.m_curResult.length());

        if (pRobotStateServer->m_sendTimestep < 0)
            usleep(100000);
        else
            sleep(pRobotStateServer->m_sendTimestep);
    }
    delete[] shareData;
    shareData = nullptr;

    delete[] sendData;
    sendData = nullptr;
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

int RobotStateServer::getShareData(char* buf) {
    memcpy(buf, m_region.get_address(), m_region.get_size());
    return 1;
}

RobotStateServer::~RobotStateServer() {

}









