//
// Created by 朱熙 on 2018/6/5.
//

#ifndef PARSERSERVER_ROBOTSTATESERVER_H
#define PARSERSERVER_ROBOTSTATESERVER_H

#include "../ServerManager/ServerManager.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include "../../include/global.h"
#include <boost/interprocess/mapped_region.hpp>

using namespace boost::interprocess;


class ServerManager;

class RobotStateServer {
public:
    RobotStateServer(ServerManager * pServerManager);
    ~RobotStateServer();

    void run();

    void stop();
    bool isStop();

    int getShareData(char* buf);
public:
    ServerManager *m_pServerManager;

    int m_sendTimestep;     //发送状态间隔
    src::severity_channel_logger<severity_level, std::string> scl;
private:

    shared_memory_object m_shm;
    mapped_region m_region;

    bool m_isStop;

    


};


#endif //PARSERSERVER_ROBOTSTATESERVER_H
