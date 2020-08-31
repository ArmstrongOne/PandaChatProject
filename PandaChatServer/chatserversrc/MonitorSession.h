/**
 * ��ػỰ��, MonitorSession.h
 * zhangyl 2017.03.09
 */
#ifndef __MONITOR_SESSION_H__
#define __MONITOR_SESSION_H__

#include "../net/Buffer.h"
#include "../base/Timestamp.h"
#include "../net/TcpConnection.h"
#include <memory>

using namespace net;

class MonitorSession
{
public:
    MonitorSession(std::shared_ptr<TcpConnection>& conn);
    ~MonitorSession() = default;
    MonitorSession(const MonitorSession& rhs) = delete;
    MonitorSession& operator =(const MonitorSession& rhs) = delete;

public:
    //�����ݿɶ�, �ᱻ�������loop����
    void onRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime);

    std::shared_ptr<TcpConnection> getConnectionPtr()
    {
        if (m_tmpConn.expired())
            return NULL;

        return m_tmpConn.lock();
    }

    void showHelp();
    void send(const char* data, size_t length);

private:
    bool process(const std::shared_ptr<TcpConnection>& conn, const std::string& inbuf);
    bool showOnlineUserList(const std::string& token = "");
    bool showSpecifiedUserInfoByID(int32_t userid);

private:
    std::weak_ptr<TcpConnection>       m_tmpConn;
};


#endif //!__MONITOR_SESSION_H__