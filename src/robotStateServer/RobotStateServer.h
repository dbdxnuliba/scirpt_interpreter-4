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

    /**
     * 启动线程
     */
    void run();

    /**
     * 停止线程
     */
    void stop();

    /**
     * 判断是否停止
     * @return
     */
    bool isStop();

    /**
     * 读取共享内存数据
     * @param buf
     * @return
     */
    int getShareData(char* buf);
public:
    ServerManager *m_pServerManager;

    int m_sendTimestep;    //发送状态间隔
    src::severity_channel_logger<severity_level, std::string> scl;
private:

    shared_memory_object m_shm;     //共享内存，用于接收robot内部状态数据
    mapped_region m_region;         //共享内存映射，用于读写共享内存

    bool m_isStop;

    


};


#endif //PARSERSERVER_ROBOTSTATESERVER_H
