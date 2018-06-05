//
// Created by 朱熙 on 2018/5/31.
//

#include "tcpServer.h"
#include <boost/bind/bind.hpp>


TcpServer::TcpServer(ServerManager *pServerManager) : scl(keywords::channel = "tcpServer_class"),
                                                      m_buf(TCP_RECV_BUF_SIZE, 0),
                                                      m_acceptor(m_io, endpoint_type(ip::tcp::v4(), 6688))
//m_timer(m_io, boost::posix_time::millisec(50))
{
    this->m_pServerManager = pServerManager;

    BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ << " success, create tcp server.";
}

void tcp_thread(void * ptcpServer) {
    TcpServer * tcpServer = (TcpServer*) ptcpServer;
    tcpServer->accept();
    tcpServer->m_io.run();

}

void TcpServer::run()
{
    m_pServerManager->m_pTcpThread = new thread(tcp_thread, this);
}

void TcpServer::accept()
{
    sock_ptr sock(new socket_type(m_io));
    m_acceptor.async_accept(*sock, boost::bind(&this_type::accept_handler, this, boost::asio::placeholders::error, sock));
}

void TcpServer::accept_handler(const boost::system::error_code& ec, sock_ptr sock)
{
    if (ec)
    {    return;    }
    if (m_sock.use_count()) {
        m_sock->close();
        m_sock.reset();
    }

    m_sock = sock;

    BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ << " Client: "<<sock->remote_endpoint().address();
    sock->async_read_some(buffer(m_buf), boost::bind(&TcpServer::read_handler, this, boost::asio::placeholders::error, sock));
    //postpone_timer(sock);
    // 发送完毕后继续监听，否则io_service将认为没有事件处理而结束运行
    accept();
}

void TcpServer::read_handler(const boost::system::error_code&ec, sock_ptr sock)
{
    if (ec)
    {
        return;
    }

    sock->async_read_some(buffer(m_buf), boost::bind(&TcpServer::read_handler, this, boost::asio::placeholders::error, sock));
    if (!m_buf.empty()) {
        BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ <<"receive from sock:"<< &m_buf[0];
        m_buf.assign(TCP_RECV_BUF_SIZE, 0);
    }

}

//void TcpServer::write_handler(const boost::system::error_code&ec, sock_ptr sock)
//{
//    //BOOST_LOG_SEV(scl, debug) << __FUNCTION__ << ":" << __LINE__ <<"send msg complete";
//    cout<<"send msg complete"<<endl;
//}

//void TcpServer::send(buffer_type& buf)
void TcpServer::send(char* buf) {

    if (m_sock.use_count()) {

        memcpy(m_sendBuf, buf, 4096);

        //使用异步发送，存在发送不及时，缓存增长，暂时采用同步发送
        //m_sock->async_write_some(buffer(m_sendBuf), boost::bind(&this_type::write_handler, this, boost::asio::placeholders::error, m_sock));

        boost::system::error_code ec;
        m_sock->write_some(buffer(m_sendBuf), ec);
    }
}

//void TcpServer::postpone_timer(sock_ptr sock) {
//    m_timer.expires_from_now(boost::posix_time::millisec(50));
//    m_timer.async_wait(boost::bind(&this_type::writesock, this, sock));
//}

//void TcpServer::writesock(sock_ptr sock)
//{
//    m_pServerManager->m_mqRecvBuffMutex.lock();
//    if (m_pServerManager->m_mqRecvBuff.size() > 0)
//    {
//        buffer_type buf(m_pServerManager->m_mqRecvBuff);
//        sock->async_write_some(buffer(buf), boost::bind(&this_type::write_handler, this, boost::asio::placeholders::error, sock));
//        m_pServerManager->m_mqRecvBuff.clear();
//    }
//    m_pServerManager->m_mqRecvBuffMutex.unlock();
//    postpone_timer(sock);
//}
