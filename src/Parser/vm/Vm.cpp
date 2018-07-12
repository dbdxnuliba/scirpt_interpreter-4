//
// Created by 朱熙 on 2018/6/4.
//

#include "Vm.h"

Vm::Vm() : scl(keywords::channel = "vm_class"), m_bStop(true), m_bWaitRes(false), m_pProcessThread(nullptr) {

    message_queue::remove("message_queue_send");
    message_queue::remove("message_queue_recv");

    m_pMqSend = new message_queue(open_or_create, "message_queue_send", 10, 256);       //发送脚本
    m_pMqRecv = new message_queue(open_or_create, "message_queue_recv", 10, 32);        //接收返回值

    m_xmlReader.readXml("./xml/param.xml");

    m_robotState = new RobotState(m_cv);

    shared_memory_object::remove("RobotCommand");
    m_shm = shared_memory_object(create_only, "RobotCommand", read_write);
    m_shm.truncate(sizeof(COMMAND));
    m_region = mapped_region(m_shm, read_write);

    memset(m_region.get_address(), 0, m_region.get_size());

    m_curResult = "CODE_0";

}

Vm::~Vm() {

}

void process_thread(Vm * pVm) {
    Vm * vm = pVm;
    vm->m_bStop = false;
    vm->m_bWaitRes = false;
    vm->evalNodeList(vm->m_nodeList);
    vm->m_bStop = true;
    vm->m_bWaitRes = false;

    cout<<"vm thread finish"<<endl;
}

void Vm::start() {

    unsigned int priority;
    message_queue::size_type recvd_size;
    char res[320];
    m_pMqRecv->try_receive(res, sizeof(res), recvd_size, priority);

    m_pProcessThread = new thread(process_thread, this);
}

/***
 * 清除当前虚拟机内脚本
 */
void Vm::reset() {
    for (auto node : m_nodeList) {
        delete node;
        node = nullptr;
    }

    m_nodeList.clear();
    m_globalParams.global_variable.clear();
}

int nSendScript = 0;
void Vm::sendToMq(string strData) {
    try{
        nSendScript++;
        BOOST_LOG_SEV(scl, info) << "send script, total : " << nSendScript;
        m_pMqSend->send(strData.c_str(), strData.size(), 0);
    }
    catch(interprocess_exception &ex){
        std::cout << ex.what() << std::endl;
    }

}

void Vm::stop() {
    m_bStop = true;
    if (m_pProcessThread) {
        m_pProcessThread->join();
        delete m_pProcessThread;
        m_pProcessThread = nullptr;
    }
    m_bWaitRes = false;
}

void Vm::sendCommand() {
    memcpy(m_region.get_address(), &m_command, sizeof(COMMAND));
}

void Vm::setDos(int dos) {
    m_command.dos = dos;
    sendCommand();
}

void Vm::setPowerState(bool bState) {
    m_command.isPowerOn = bState;
    sendCommand();
}

void Vm::setPlayState(bool bPlay) {
    if (bPlay) {
        m_command.isStop = false;
        m_command.isPause = false;
    } else {
        m_command.isStop = true;
        m_command.isPause = false;
    }
    sendCommand();
}

void Vm::setPauseState(bool bState) {
    m_command.isPause = bState;
}

void Vm::evalNodeList(list<Node *> &nodeList) {
    unsigned int priority;
    message_queue::size_type recvd_size;
    for (auto node : nodeList) {
        if (m_bStop) break;
        while (!m_bStop && m_bWaitRes) {
            char res[320] = {0};
            if (m_pMqRecv->try_receive(res, sizeof(res), recvd_size, priority)) {
                if (memcmp(res, "CODE_0", 6) == 0) {
                    m_bWaitRes = false;
                }else {
                    m_bStop = true;
                }
                m_curResult = string(res);
            }
        }


        node->eval();
        if (!m_bStop) {
            m_curScriptId = node->id;
            if (typeid(*node) == typeid(Movej_Node) ||
                typeid(*node) == typeid(Movel_Node) ||
                typeid(*node) == typeid(Speedj_Node) ||
                typeid(*node) == typeid(Speedl_Node) ||
                typeid(*node) == typeid(Stopj_Node) ||
                typeid(*node) == typeid(Stopl_Node) ||
                typeid(*node) == typeid(Teach_Mode_Node) ||
                typeid(*node) == typeid(End_Teach_Mode_Node) ||
                typeid(*node) == typeid(MoveCamera_Node)) {
                m_bWaitRes = true;
            }
        }
    }

    while (!m_bStop && m_bWaitRes) {
        char res[320] = {0};
        if (m_pMqRecv->try_receive(res, sizeof(res), recvd_size, priority)) {
            m_bStop = true;
            m_curResult = string(res);
        }
    }
}


