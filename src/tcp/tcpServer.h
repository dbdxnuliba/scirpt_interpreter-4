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

class TcpServer {

    typedef TcpServer this_type;
    typedef ip::tcp::acceptor acceptor_type;
    typedef ip::tcp::endpoint endpoint_type;
    typedef ip::tcp::socket socket_type;
    typedef ip::address address_type;
    typedef boost::shared_ptr<socket_type> sock_ptr;


public:
    TcpServer(ServerManager *pServerManager);

    void run();
    void accept();

    //void send(buffer_type& buf);
    void send(const char* buf, int len);
private:

    void accept_handler(const boost::system::error_code& ec, sock_ptr sock);
    void read_handler(const boost::system::error_code&ec, sock_ptr sock);

    //拆包处理
    void unpack_handler();
    //void write_handler(const boost::system::error_code&ec, sock_ptr sock);
    //void postpone_timer(sock_ptr sock);
    //void writesock(sock_ptr sock);

private:
    src::severity_channel_logger<severity_level, std::string> scl;

    mutex m_tcpRecvBuffMutex;

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
