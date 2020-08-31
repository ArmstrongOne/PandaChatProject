/**
 * �������֧��http����, HttpServer.cpp
 * zhangyl 2018.05.16
 */
#include "HttpServer.h"
#include "../net/InetAddress.h"
#include "../Utils/AsyncLog.h"
#include "../Utils/Singleton.h"
//#include "../net/eventloop.h"
#include "../net/EventLoopThread.h"
#include "../net/EventLoopThreadPool.h"
#include "HttpSession.h"
#include "HttpServer.h"

bool HttpServer::init(const char* ip, short port, EventLoop* loop)
{
    InetAddress addr(ip, port);
    m_server.reset(new TcpServer(loop, addr, "ZYL-MYHTTPSERVER", TcpServer::kReusePort));
    m_server->setConnectionCallback(std::bind(&HttpServer::onConnected, this, std::placeholders::_1));
    //��������
    m_server->start();
    return true;
}

void HttpServer::uninit()
{
    if (m_server)
        m_server->stop();
}

//�����ӵ������û����ӶϿ���������Ҫͨ��conn->connected()���жϣ�һ��ֻ����loop�������
void HttpServer::onConnected(std::shared_ptr<TcpConnection> conn)
{
    if (conn->connected())
    {
        std::shared_ptr<HttpSession> spSession(new HttpSession(conn));
        conn->setMessageCallback(std::bind(&HttpSession::onRead, spSession.get(), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        {
            std::lock_guard<std::mutex> guard(m_sessionMutex);
            m_sessions.push_back(spSession);
        }
    }
    else
    {
        onDisconnected(conn);
    }
}

//���ӶϿ�
void HttpServer::onDisconnected(const std::shared_ptr<TcpConnection>& conn)
{
    //TODO: �����Ĵ����߼�̫���ң���Ҫ�Ż�
    std::lock_guard<std::mutex> guard(m_sessionMutex);
    for (auto iter = m_sessions.begin(); iter != m_sessions.end(); ++iter)
    {
        if ((*iter)->getConnectionPtr() == NULL)
        {
            LOGE("connection is NULL");
            break;
        }

        //ͨ���ȶ�connection�����ҵ���Ӧ��session
        if ((*iter)->getConnectionPtr() == conn)
        {
            m_sessions.erase(iter);
            LOGI("monitor client disconnected: %s", conn->peerAddress().toIpPort().c_str());
            break;
        }
    }
}