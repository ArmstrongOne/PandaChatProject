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
    if(started_ == 0) //没有启动
    {
		//启动线程池管理
        eventLoopThreadPool_.reset(new EventLoopThreadPool());
        eventLoopThreadPool_->init(loop_, workerThreadCount);
        eventLoopThreadPool_->start();
        
        //threadPool_->start(threadInitCallback_);
        //assert(!acceptor_->listenning());

		//将函数对象放入队列或者立即执行
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));//监听端口并注册进多路复用
        started_ = 1;
    }
}

//关闭TcpServer的基本操作就是:
//迭代器，遍历ConnectionMap（存放所有的TcpConnection）, 这里首先reset了map中的TcpConnectionPtr，我不是很理解为什么要，创建一个再reset掉之前的，为什么不用it->second->getLoop()->runInLoop()，是不是会出现内存泄露的问题，不是很理解，有待进一步查证!!!
//getLoop()函数是获取当前TcpConnection对应的EventLoop，其实是在创建TcpConnection的时候，会从线程池中选一个已经存在的EventLoop给它。应该说这个TcpConnection属于这个EventLoop。
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
	//给新连接分配线程
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
	//放入map容器管理
    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);//指向业务层Session
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, std::placeholders::_1)); // FIXME: unsafe
    //该线程分离完io事件后，立即调用TcpConnection::connectEstablished
    ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    // FIXME: unsafe
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
	//主线程执行
    loop_->assertInLoopThread();
    LOGD("TcpServer::removeConnectionInLoop [%s] - connection %s", name_.c_str(), conn->name().c_str());
    size_t n = connections_.erase(conn->name());  //引用计数减1，当前应该是2
    //(void)n;
    //assert(n == 1);
    if (n != 1)
    {
        //出现这种情况，是TcpConneaction对象在创建过程中，对方就断开连接了。
        LOGD("TcpServer::removeConnectionInLoop [%s] - connection %s, connection does not exist.", name_.c_str(), conn->name().c_str());
        return;
    }
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));

	//执行完这个函数 会释放handleclose栈上对象 引用计数-1
	//当前应该是1
}

