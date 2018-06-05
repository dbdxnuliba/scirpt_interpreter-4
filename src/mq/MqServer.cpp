//
// Created by 朱熙 on 2018/6/1.
//

#include "MqServer.h"
#include <thread>
#include <iterator>

MqServer::MqServer(ServerManager *pServerManager)
        : m_bIsStop(false) {
    this->m_pServerManager = pServerManager;

    m_mqSend = new message_queue(open_or_create, "message_queue_send", 10, MQ_RECV_PACKAGE_SIZE);
    m_mqRecv = new message_queue(open_or_create, "message_queue_recv", 10, MQ_RECV_PACKAGE_SIZE);
}

MqServer::~MqServer() {
    delete m_mqSend;
    m_mqSend = nullptr;

    delete m_mqRecv;
    m_mqRecv = nullptr;
}

void mq_thread(void * pMqServer) {
    MqServer* mqServer = (MqServer*)pMqServer;
    while (!mqServer->isStop()) {
        mqServer->recvFromMq();
        mqServer->sendToMq();
        sleep(100);
    }

}

void MqServer::run() {
    m_bIsStop = false;
    //m_pServerManager->m_pMqTrhead = new thread(mq_thread, this);
}

bool MqServer::isStop() {
    return m_bIsStop;
}

void MqServer::stop() {
    m_bIsStop = true;
}

int MqServer::recvFromMq() {
    unsigned int priority;
    message_queue::size_type recvd_size = 0;
    char msgRecv[MQ_RECV_PACKAGE_SIZE] = {0};
    try {
        m_mqRecv->receive(msgRecv, MQ_RECV_PACKAGE_SIZE, recvd_size, priority);
    }
    catch(interprocess_exception &ex){
        std::cout << ex.what() << std::endl;
    }

    for (int i=0; i<recvd_size; i++) {
        m_recvBuff.push_back(msgRecv[i]);
    }

    //判断是否存在完整消息，如果存在，将消息取出，放入tpc待发送队列
    m_pServerManager->m_mqRecvBuffMutex.lock();
    do {
        int len = isWholeMsg();
        char *recv = new char[len];
        if (len > 0) {
            for (int j = 0; j < len; ++j) {
                *(recv+j) = m_recvBuff.front();
                m_recvBuff.pop_front();
            }
            //todo 如果存在完整消息，对该消息进行判断分析


            //mq缓存超过最大容量，清空缓存
            if (m_pServerManager->m_mqRecvBuff.size() + len > MQ_RECV_BUF_SIZE)
                m_pServerManager->m_mqRecvBuff.clear();

            //将完整消息复制到发送缓存中
            m_pServerManager->m_mqRecvBuff.insert(m_pServerManager->m_mqRecvBuff.end(), recv, recv+len);
        }
        delete[] recv;
        recv = nullptr;
    }while(isWholeMsg());
    m_pServerManager->m_mqRecvBuffMutex.unlock();

    return recvd_size;
}

int MqServer::isWholeMsg() {
    int len = m_recvBuff[2] * 256 + m_recvBuff[3];

    return m_recvBuff.size() >= len ? len : 0;
}

int MqServer::sendToMq() {
    m_pServerManager->m_mqSendBuffMutex.lock();

    if (!m_pServerManager->m_mqSendBuff.empty()) {
        char *buffer = new char[m_pServerManager->m_mqSendBuff.size()];
        memcpy(buffer, &m_pServerManager->m_mqSendBuff[0], m_pServerManager->m_mqSendBuff.size()*sizeof(char));
        m_mqSend->send(buffer, sizeof(buffer), 0);
        delete[] buffer;
        buffer = nullptr;
    }

    m_pServerManager->m_mqSendBuffMutex.unlock();

    return 0;
}

