//
// Created by 朱熙 on 2018/5/31.
//

#ifndef PARSERSERVER_TCPSERVER_H
#define PARSERSERVER_TCPSERVER_H


#include "../../include/global.h"
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>

#include "../ServerManager/ServerManager.h"


using namespace boost::asio;

class ServerManager;

/**
 * Tcp Server类
 * 建立TCP server，监听连接，向Client发送数据
 */
class TcpServer {

    typedef TcpServer this_type;
    typedef ip::tcp::acceptor acceptor_type;
    typedef ip::tcp::endpoint endpoint_type;
    typedef ip::tcp::socket socket_type;
    typedef ip::address address_type;
    typedef boost::shared_ptr<socket_type> sock_ptr;


public:
    TcpServer(ServerManager *pServerManager);

    /**
     * 启动TCP server线程
     */
    void run();

    /**
     * 启动监听
     */
    void accept();

    //void send(buffer_type& buf);
    /**
     * 发送数据
     * @param buf 数据地址
     * @param len 数据长度
     */
    void send(const char* buf, int len);
private:

    /**
     * 监听回调函数
     * @param ec 错误码
     * @param sock socket
     */
    void accept_handler(const boost::system::error_code& ec, sock_ptr sock);
    /**
     * 接收回调函数
     * @param ec
     * @param sock
     */
    void read_handler(const boost::system::error_code&ec, sock_ptr sock);

    /**
     * 粘包处理函数
     */
    void unpack_handler();
    //void write_handler(const boost::system::error_code&ec, sock_ptr sock);
    //void postpone_timer(sock_ptr sock);
    //void writesock(sock_ptr sock);

private:
    src::severity_channel_logger<severity_level, std::string> scl;      //log模块

    mutex m_tcpRecvBuffMutex;                                           //tcp接收buffer互斥锁

public:
    io_service m_io;
    acceptor_type m_acceptor;
    //deadline_timer m_timer;
    buffer_type m_buf;          //接收缓存
    deque<char> m_unpackBuf;    //接收缓存，用于拆包


    char m_sendBuf[4096];       //发送缓存
    sock_ptr m_sock;

    ServerManager *m_pServerManager;

};


#endif //PARSERSERVER_TCPSERVER_H
