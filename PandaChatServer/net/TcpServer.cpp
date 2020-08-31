#include "TcpServer.h"

#include <stdio.h>  // snprintf
#include <functional>

#include "../base/Platform.h"
#include "..\Utils\AsyncLog.h"
#include "../Utils/Singleton.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "Sockets.h"

using namespace net;

TcpServer::TcpServer(EventLoop* loop,
    const InetAddress& listenAddr,
    const std::string& nameArg,
    Option option)
    : loop_(loop),
    hostport_(listenAddr.toIpPort()),
    name_(nameArg),
    acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
    //threadPool_(new EventLoopThreadPool(loop, name_)),
    connectionCallback_(defaultConnectionCallback),
    messageCallback_(defaultMessageCallback),
    started_(0),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::~TcpServer()
{
    loop_->assertInLoopThread();
    LOGD("TcpServer::~TcpServer [%s] destructing", name_.c_str());
    stop();
}

//void TcpServer::setThreadNum(int numThreads)
//{
//  assert(0 <= numThreads);
//  threadPool_->setThreadNum(numThreads);
//}

void TcpServer::start(int workerThreadCount/* = 4*/)
{
    if(started_ == 0) //û������
    {
		//�����̳߳ع���
        eventLoopThreadPool_.reset(new EventLoopThreadPool());
        eventLoopThreadPool_->init(loop_, workerThreadCount);
        eventLoopThreadPool_->start();
        
        //threadPool_->start(threadInitCallback_);
        //assert(!acceptor_->listenning());

		//���������������л�������ִ��
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));//�����˿ڲ�ע�����·����
        started_ = 1;
    }
}

//�ر�TcpServer�Ļ�����������:
//������������ConnectionMap��������е�TcpConnection��, ��������reset��map�е�TcpConnectionPtr���Ҳ��Ǻ����ΪʲôҪ������һ����reset��֮ǰ�ģ�Ϊʲô����it->second->getLoop()->runInLoop()���ǲ��ǻ�����ڴ�й¶�����⣬���Ǻ���⣬�д���һ����֤!!!
//getLoop()�����ǻ�ȡ��ǰTcpConnection��Ӧ��EventLoop����ʵ���ڴ���TcpConnection��ʱ�򣬻���̳߳���ѡһ���Ѿ����ڵ�EventLoop������Ӧ��˵���TcpConnection�������EventLoop��
void TcpServer::stop()
{
    if (started_ == 0)
        return; 
    for (ConnectionMap::iterator it = connections_.begin(); it != connections_.end(); ++it)
    {
        TcpConnectionPtr conn = it->second;
        it->second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
        conn.reset();
    }
    eventLoopThreadPool_->stop();
    started_ = 0;
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    loop_->assertInLoopThread();
	//�������ӷ����߳�
    EventLoop* ioLoop = eventLoopThreadPool_->getNextLoop();
    char buf[32];
    snprintf(buf, sizeof buf, ":%s#%d", hostport_.c_str(), nextConnId_);
    ++nextConnId_;
    string connName = name_ + buf;

    LOGD("TcpServer::newConnection [%s] - new connection [%s] from %s", name_.c_str(), connName.c_str(), peerAddr.toIpPort().c_str());

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
	//����map��������
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);//ָ��ҵ���Session
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    //���̷߳�����io�¼�����������TcpConnection::connectEstablished
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    // FIXME: unsafe
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	//���߳�ִ��
    loop_->assertInLoopThread();
    LOGD("TcpServer::removeConnectionInLoop [%s] - connection %s", name_.c_str(), conn->name().c_str());
    size_t n = connections_.erase(conn->name());  //���ü�����1����ǰӦ����2
    //(void)n;
    //assert(n == 1);
    if (n != 1)
    {
        //���������������TcpConneaction�����ڴ��������У��Է��ͶϿ������ˡ�
        LOGD("TcpServer::removeConnectionInLoop [%s] - connection %s, connection does not exist.", name_.c_str(), conn->name().c_str());
        return;
    }
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));

	//ִ����������� ���ͷ�handlecloseջ�϶��� ���ü���-1
	//��ǰӦ����1
}

