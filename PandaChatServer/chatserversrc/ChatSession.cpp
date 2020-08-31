/**
 * ChatSession.cpp
 * zhangyl, 2017.03.10
 **/
#include "ChatSession.h"
#include <string.h>
#include <sstream>
#include <list>
#include "../net/TcpConnection.h"
#include "../net/ProtocolStream.h"
#include "../Utils/AsyncLog.h"
#include "../Utils/Singleton.h"
#include "../jsoncpp1.9.0/json.h"
#include "Msg.h"
#include "UserManager.h"
#include "ChatServer.h"
#include "MsgCacheManager.h"
#include "../zlib1.2.11/ZlibUtil.h"
#include "BussinessLogic.h"

//������ֽ�������Ϊ10M
#define MAX_PACKAGE_SIZE    10 * 1024 * 1024

using namespace std;
using namespace net;

//��������ʱ���ݰ�����������������ó�30��
#define MAX_NO_PACKAGE_INTERVAL  30

ChatSession::ChatSession(const std::shared_ptr<TcpConnection>& conn, int sessionid) :
TcpSession(conn), 
m_id(sessionid),
m_seq(0),
m_isLogin(false)
{
	m_userinfo.userid = 0;
    m_lastPackageTime = time(NULL);

//#ifndef _DEBUG
    //����ע�͵��������ڵ���
    //EnableHearbeatCheck();
//#endif
}

ChatSession::~ChatSession()
{
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        LOGI("remove check online timerId, userid: %d, clientType: %d, client address: %s", m_userinfo.userid, m_userinfo.clienttype, conn->peerAddress().toIpPort().c_str());
        conn->getLoop()->remove(m_checkOnlineTimerId);
    }
}

void ChatSession::onRead(const std::shared_ptr<TcpConnection>& conn, Buffer* pBuffer, Timestamp receivTime)
{
    while (true)
    {
        //����һ����ͷ��С
        if (pBuffer->readableBytes() < (size_t)sizeof(chat_msg_header))
        {
            //LOGI << "buffer is not enough for a package header, pBuffer->readableBytes()=" << pBuffer->readableBytes() << ", sizeof(msg)=" << sizeof(msg);
            return;
        }

        //ȡ��ͷ��Ϣ
        chat_msg_header header;
        memcpy(&header, pBuffer->peek(), sizeof(chat_msg_header));
        //���ݰ�ѹ����
        if (header.compressflag == PACKAGE_COMPRESSED)
        {
            //��ͷ�д��������ر�����
            if (header.compresssize <= 0 || header.compresssize > MAX_PACKAGE_SIZE ||
                header.originsize <= 0 || header.originsize > MAX_PACKAGE_SIZE)
            {
                //�ͻ��˷��Ƿ����ݰ��������������ر�֮
                LOGE("Illegal package, compresssize: %lld, originsize: %lld, close TcpConnection, client: %s",  header.compresssize, header.originsize, conn->peerAddress().toIpPort().c_str());
                conn->forceClose();
                return;
            }

            //�յ������ݲ���һ�������İ�
            if (pBuffer->readableBytes() < (size_t)header.compresssize + sizeof(chat_msg_header))
                return;

            pBuffer->retrieve(sizeof(chat_msg_header));
            std::string inbuf;
            inbuf.append(pBuffer->peek(), header.compresssize);
            pBuffer->retrieve(header.compresssize);
            std::string destbuf;
            if (!ZlibUtil::uncompressBuf(inbuf, destbuf, header.originsize))
            {
                LOGE("uncompress error, client: %s", conn->peerAddress().toIpPort().c_str());
                conn->forceClose();
                return;
            }

            if (!process(conn, destbuf.c_str(), destbuf.length()))
            {
                //�ͻ��˷��Ƿ����ݰ��������������ر�֮
                LOGE("Process error, close TcpConnection, client: %s", conn->peerAddress().toIpPort().c_str());
                conn->forceClose();
                return;
            }
            m_lastPackageTime = time(NULL);
        }
        //���ݰ�δѹ��
        else
        {
            //��ͷ�д��������ر�����
            if (header.originsize <= 0 || header.originsize > MAX_PACKAGE_SIZE)
            {
                //�ͻ��˷��Ƿ����ݰ��������������ر�֮
                LOGE("Illegal package, compresssize: %lld, originsize: %lld, close TcpConnection, client: %s", header.compresssize, header.originsize, conn->peerAddress().toIpPort().c_str());
                conn->forceClose();
                return;
            }

            //�յ������ݲ���һ�������İ�
            if (pBuffer->readableBytes() < (size_t)header.originsize + sizeof(chat_msg_header))
                return;

            pBuffer->retrieve(sizeof(chat_msg_header));
            std::string inbuf;
            inbuf.append(pBuffer->peek(), header.originsize);
            pBuffer->retrieve(header.originsize);
            if (!process(conn, inbuf.c_str(), inbuf.length()))
            {
                //�ͻ��˷��Ƿ����ݰ��������������ر�֮
                LOGE("Process error, close TcpConnection, client: %s", conn->peerAddress().toIpPort().c_str());
                conn->forceClose();
                return;
            }
            m_lastPackageTime = time(NULL);
        }// end else
    }// end while-loop
}

bool ChatSession::process(const std::shared_ptr<TcpConnection>& conn, const char* inbuf, size_t buflength)
{   
    BinaryStreamReader readStream(inbuf, buflength);
    int32_t cmd;
    if (!readStream.ReadInt32(cmd))
    {
        LOGE("read cmd error, client: %s", conn->peerAddress().toIpPort().c_str());
        return false;
    }

    //int seq;
    if (!readStream.ReadInt32(m_seq))
    {
        LOGE("read seq error, client: %s", conn->peerAddress().toIpPort().c_str());
        return false;
    }

    std::string data;
    size_t datalength;
    if (!readStream.ReadString(&data, 0, datalength))
    {
        LOGE("read data error, client: %s", conn->peerAddress().toIpPort().c_str());
        return false;
    }
   
    //������̫Ƶ��������ӡ
    if (cmd != msg_type_heartbeat)
        LOGI("Request from client: userid=%d, cmd=%d, seq=%d, data=%s, datalength=%d, buflength=%d", m_userinfo.userid, cmd, m_seq, data.c_str(), datalength, buflength);
    
    if (Singleton<ChatServer>::Instance().isLogPackageBinaryEnabled())
    {
        LOGI("body stream, buflength: %d, client: %s", buflength, conn->peerAddress().toIpPort().c_str());
        //LOG_DEBUG_BIN((unsigned char*)inbuf, buflength);
    }
        
    switch (cmd)
    {
        //������
        case msg_type_heartbeat:
            onHeartbeatResponse(conn);
            break;

        //ע��
        case msg_type_register:
            onRegisterResponse(data, conn);
            break;
        
        //��¼
        case msg_type_login:                          
            onLoginResponse(data, conn);
            break;
        
        //��������������Ѿ���¼��ǰ���²��ܽ��в���
        default:
        {
            if (m_isLogin)
            {
                switch (cmd)
                {
                    //��ȡ�����б�
                    case msg_type_getofriendlist:
                        onGetFriendListResponse(conn);
                        break;

                    //�����û�
                    case msg_type_finduser:
                        onFindUserResponse(data, conn);
                        break;

                    //�Ӻ���
                    case msg_type_operatefriend:    
                        onOperateFriendResponse(data, conn);
                        break;

                    //�û����������Լ�����״̬
                    case msg_type_userstatuschange:
        	            onChangeUserStatusResponse(data, conn);
                        break;

                    //�����û���Ϣ
                    case msg_type_updateuserinfo:
                        onUpdateUserInfoResponse(data, conn);
                        break;
        
                    //�޸�����
                    case msg_type_modifypassword:
                        onModifyPasswordResponse(data, conn);
                        break;
        
                    //����Ⱥ
                    case msg_type_creategroup:
                        onCreateGroupResponse(data, conn);
                        break;

                    //��ȡָ��Ⱥ��Ա��Ϣ
                    case msg_type_getgroupmembers:
                        onGetGroupMembersResponse(data, conn);
                        break;

                    //������Ϣ
                    case msg_type_chat:
                    {
                        int32_t target;
                        if (!readStream.ReadInt32(target))
                        {
                            LOGE("read target error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }
                        onChatResponse(target, data, conn);
                    }
                        break;
        
                    //Ⱥ����Ϣ
                    case msg_type_multichat:
                    {
                        std::string targets;
                        size_t targetslength;
                        if (!readStream.ReadString(&targets, 0, targetslength))
                        {
                            LOGE("read targets error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        onMultiChatResponse(targets, data, conn);
                    }

                        break;

                    //��Ļ��ͼ
                    case msg_type_remotedesktop:
                    {
                        string bmpHeader;
                        size_t bmpHeaderlength;
                        if (!readStream.ReadString(&bmpHeader, 0, bmpHeaderlength))
                        {
                            LOGE("read bmpheader error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string bmpData;
                        size_t bmpDatalength;
                        if (!readStream.ReadString(&bmpData, 0, bmpDatalength))
                        {
                            LOGE("read bmpdata error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }
                                   
                        int32_t target;
                        if (!readStream.ReadInt32(target))
                        {
                            LOGE("read target error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }
                        onScreenshotResponse(target, bmpHeader, bmpData, conn);
                    }
                        break;

                    //�����û�������Ϣ
                    case msg_type_updateteaminfo:
                    {
                        int32_t operationType;
                        if (!readStream.ReadInt32(operationType))
                        {
                            LOGE("read operationType error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string newTeamName;
                        size_t newTeamNameLength;
                        if (!readStream.ReadString(&newTeamName, 0, newTeamNameLength))
                        {
                            LOGE("read newTeamName error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string oldTeamName;
                        size_t oldTeamNameLength;
                        if (!readStream.ReadString(&oldTeamName, 0, oldTeamNameLength))
                        {
                            LOGE("read newTeamName error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }
                        
                        onUpdateTeamInfoResponse(operationType, newTeamName, oldTeamName, conn);
                        break;
                    }
                        
                    //�޸ĺ��ѱ�ע��Ϣ
                    case msg_type_modifyfriendmarkname:
                    {
                        int32_t friendid;
                        if (!readStream.ReadInt32(friendid))
                        {
                            LOGE("read friendid error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string newmarkname;
                        size_t newmarknamelength;
                        if (!readStream.ReadString(&newmarkname, 0, newmarknamelength))
                        {
                            LOGE("read newmarkname error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        onModifyMarknameResponse(friendid, newmarkname, conn);
                        break;
                    }
                    
                    //�ƶ���������������
                    case msg_type_movefriendtootherteam:
                    {
                        int32_t friendid;
                        if (!readStream.ReadInt32(friendid))
                        {
                            LOGE("read friendid error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string newteamname;
                        size_t newteamnamelength;
                        if (!readStream.ReadString(&newteamname, 0, newteamnamelength))
                        {
                            LOGE("read newteamname error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        string oldteamname;
                        size_t oldteamnamelength;
                        if (!readStream.ReadString(&oldteamname, 0, oldteamnamelength))
                        {
                            LOGE("read oldteamname error, client: %s", conn->peerAddress().toIpPort().c_str());
                            return false;
                        }

                        onMoveFriendToOtherTeamResponse(friendid, newteamname, oldteamname, conn);
                    }
                        break;                      

                    default:
                        //pBuffer->retrieveAll();
                        LOGE("unsupport cmd, cmd: %d, data=%s, connection name:", cmd, data.c_str(), conn->peerAddress().toIpPort().c_str());
                        //conn->forceClose();
                        return false;
                }// end inner-switch
            }
            else
            {
                //�û�δ��¼�����߿ͻ��˲��ܽ��в�����ʾ��δ��¼��
                std::string data = "{\"code\": 2, \"msg\": \"not login, please login first!\"}";
                send(cmd, m_seq, data);
                LOGI("Response to client: cmd=%d, , data=%s, , sessionId=%d", cmd, data.c_str(), m_id);                
            }// end if
         }// end default
    }// end outer-switch

    ++ m_seq;

    return true;
}

void ChatSession::onHeartbeatResponse(const std::shared_ptr<TcpConnection>& conn)
{
    std::string dummydata;    
    send(msg_type_heartbeat, m_seq, dummydata);

    //��������־�Ͳ�Ҫ��ӡ�ˣ�������д����־
    //LOGI << "Response to client: cmd=1000" << ", sessionId=" << m_id;
}

void ChatSession::onRegisterResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    string retData;
    BussinessLogic::registerUser(data, conn, true, retData);

    if (!retData.empty())
    {
        send(msg_type_register, m_seq, retData);
        LOGI("Response to client: cmd=msg_type_register, data: %s. client: %s", retData.c_str(), conn->peerAddress().toIpPort().c_str());
    }
}

void ChatSession::onLoginResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    //{"username": "13917043329", "password": "123", "clienttype": 1, "status": 1}
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("onLoginResponse failed, invalid json: %s, sessionId: %d, client: %s", data.c_str(), m_id, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot["username"].isString() || !jsonRoot["password"].isString() || !jsonRoot["clienttype"].isInt() || !jsonRoot["status"].isInt())
    {
        LOGE("invalid json: %s, sessionId: %d, client: %s", data.c_str(), m_id, conn->peerAddress().toIpPort().c_str());
        return;
    }

    string username = jsonRoot["username"].asString();
    string password = jsonRoot["password"].asString();
    int clientType = jsonRoot["clienttype"].asInt();
    std::ostringstream os;
    User cachedUser;
    cachedUser.userid = 0;
    Singleton<UserManager>::Instance().getUserInfoByUsername(username, cachedUser);
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    if (cachedUser.userid == 0)
    {
        //TODO: ��ЩӲ������ַ�Ӧ��ͳһ�ŵ�ĳ���ط�ͳһ����
        os << "{\"code\": 102, \"msg\": \"not registered\"}";
    }
    else
    {
        if (cachedUser.password != password)
            os << "{\"code\": 103, \"msg\": \"incorrect password\"}";
        else
        {
            //������˺��Ѿ���¼����ǰһ���˺�������
            std::shared_ptr<ChatSession> targetSession;
            //���ڷ�������֧�ֶ������ն˵�¼������ֻ��ͬһ���͵��ն���ͬһ�ͻ������Ͳ���Ϊ��ͬһ��session
            imserver.getSessionByUserIdAndClientType(targetSession, cachedUser.userid, clientType);
            if (targetSession)
            {                              
                string dummydata;
                targetSession->send(msg_type_kickuser, m_seq, dummydata);
                //�������ߵ�Session���Ϊ��Ч��
                targetSession->makeSessionInvalid();

                LOGI("Response to client, userid: %d, cmd=msg_type_kickuser", targetSession->getUserId());

                //�ر�����
                //targetSession->GetConnectionPtr()->forceClose();
            }           
            
            //��¼�û���Ϣ
            m_userinfo.userid = cachedUser.userid;
            m_userinfo.username = username;
            m_userinfo.nickname = cachedUser.nickname;
            m_userinfo.password = password;
            m_userinfo.clienttype = jsonRoot["clienttype"].asInt();
            m_userinfo.status = jsonRoot["status"].asInt();

            os << "{\"code\": 0, \"msg\": \"ok\", \"userid\": " << m_userinfo.userid << ",\"username\":\"" << cachedUser.username << "\", \"nickname\":\"" 
               << cachedUser.nickname << "\", \"facetype\": " << cachedUser.facetype << ", \"customface\":\"" << cachedUser.customface << "\", \"gender\":" << cachedUser.gender
               << ", \"birthday\":" << cachedUser.birthday << ", \"signature\":\"" << cachedUser.signature << "\", \"address\": \"" << cachedUser.address
               << "\", \"phonenumber\": \"" << cachedUser.phonenumber << "\", \"mail\":\"" << cachedUser.mail << "\"}";            
        }
    }
   
    //��¼��ϢӦ��
    send(msg_type_login, m_seq, os.str());

    LOGI("Response to client: cmd=msg_type_login, data=%s, userid=", os.str().c_str(), m_userinfo.userid);

    //�����Ѿ���¼�ı�־
    m_isLogin = true;

    //��������֪ͨ��Ϣ
    std::list<NotifyMsgCache> listNotifyCache;
    Singleton<MsgCacheManager>::Instance().getNotifyMsgCache(m_userinfo.userid, listNotifyCache);
    for (const auto &iter : listNotifyCache)
    {
        send(iter.notifymsg);
    }

    //��������������Ϣ
    std::list<ChatMsgCache> listChatCache;
    Singleton<MsgCacheManager>::Instance().getChatMsgCache(m_userinfo.userid, listChatCache);
    for (const auto &iter : listChatCache)
    {
        send(iter.chatmsg);
    }

    //�������û�����������Ϣ
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(m_userinfo.userid, friends);
    for (const auto& iter : friends)
    {
        //��Ϊ����һ���û�id������նˣ����ԣ�ͬһ��userid���ܶ�Ӧ���session
        std::list<std::shared_ptr<ChatSession>> sessions;
        imserver.getSessionsByUserId(sessions, iter.userid);
        for (auto& iter2 : sessions)
        {
            if (iter2)
            {
                iter2->sendUserStatusChangeMsg(m_userinfo.userid, 1, m_userinfo.status);
                LOGI("sendUserStatusChangeMsg to user(userid: %d): user go online, online userid: %d, status: %d", iter2->getUserId(), m_userinfo.userid, m_userinfo.status);
            }
        }
    }  
}

void ChatSession::onGetFriendListResponse(const std::shared_ptr<TcpConnection>& conn)
{
    std::string friendlist;
    makeUpFriendListInfo(friendlist, conn);
    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendlist << "}";
    send(msg_type_getofriendlist, m_seq, os.str());

    LOGI("Response to client: userid: %d, cmd=msg_type_getofriendlist, data: %s", m_userinfo.userid, os.str().c_str());    
}

void ChatSession::onChangeUserStatusResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    //{"type": 1, "onlinestatus" : 1}
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot["type"].isInt() || !jsonRoot["onlinestatus"].isInt())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    int newstatus = jsonRoot["onlinestatus"].asInt();
    if (m_userinfo.status == newstatus)
        return;

    //�����µ�ǰ�û���״̬
    m_userinfo.status = newstatus;

    //TODO: Ӧ�����Լ����߿ͻ����޸ĳɹ�

    ChatServer& imserver = Singleton<ChatServer>::Instance();
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(m_userinfo.userid, friends);
    for (const auto& iter : friends)
    {
        //��Ϊ����һ���û�id������նˣ����ԣ�ͬһ��userid���ܶ�Ӧ���session
        std::list<std::shared_ptr<ChatSession>> sessions;
        imserver.getSessionsByUserId(sessions, iter.userid);
        for (auto& iter2 : sessions)
        {
            if (iter2)
                iter2->sendUserStatusChangeMsg(m_userinfo.userid, 1, newstatus);
        }
    }
}

void ChatSession::onFindUserResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    //{ "type": 1, "username" : "zhangyl" }
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;
   
    if (!jsonRoot["type"].isInt() || !jsonRoot["username"].isString())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());       
        return;
    }

    string retData;
    //TODO: Ŀǰֻ֧�ֲ��ҵ����û�
    string username = jsonRoot["username"].asString();
    User cachedUser;
    if (!Singleton<UserManager>::Instance().getUserInfoByUsername(username, cachedUser))
        retData = "{ \"code\": 0, \"msg\": \"ok\", \"userinfo\": [] }";
    else
    {
        //TODO: �û��Ƚ϶��ʱ��Ӧ��ʹ�ö�̬string
        char szUserInfo[256] = { 0 };
        snprintf(szUserInfo, 256, "{ \"code\": 0, \"msg\": \"ok\", \"userinfo\": [{\"userid\": %d, \"username\": \"%s\", \"nickname\": \"%s\", \"facetype\":%d}] }", cachedUser.userid, cachedUser.username.c_str(), cachedUser.nickname.c_str(), cachedUser.facetype);
        retData = szUserInfo;
    } 

    send(msg_type_finduser, m_seq, retData);

    LOGI("Response to client: userid: %d, cmd=msg_type_finduser, data: %s", m_userinfo.userid, retData.c_str());
}

void ChatSession::onOperateFriendResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot["type"].isInt() || !jsonRoot["userid"].isInt())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    int type = jsonRoot["type"].asInt();
    int32_t targetUserid = jsonRoot["userid"].asInt();
    if (targetUserid >= GROUPID_BOUBDARY)
    {
        if (type == 4)
        {
            //��Ⱥ
            deleteFriend(conn, targetUserid);
            return;
        }

        if (Singleton<UserManager>::Instance().isFriend(m_userinfo.userid, targetUserid))
        {            
            LOGE("In group already, unable to join in group, groupid: %d, , userid: %d, , client: %s",  targetUserid, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
            //TODO: ֪ͨ�¿ͻ���
            return;
        }

        //��Ⱥֱ��ͬ��
        onAddGroupResponse(targetUserid, conn);
        return;
    }

    char szData[256] = { 0 };
    //ɾ������
    if (type == 4)
    {
        deleteFriend(conn, targetUserid);
        return;
    }
    //�����Ӻ�������
    if (type == 1)
    {
        if (Singleton<UserManager>::Instance().isFriend(m_userinfo.userid, targetUserid))
        {
            LOGE("Friendship already, unable to add friend, friendid: %d, userid: %d, client: %s", targetUserid, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
            //TODO: ֪ͨ�¿ͻ���
            return;
        }
        
        //{"userid": 9, "type": 1, }        
        snprintf(szData, 256, "{\"userid\":%d, \"type\":2, \"username\": \"%s\"}", m_userinfo.userid, m_userinfo.username.c_str());
    }
    //Ӧ��Ӻ���
    else if (type == 3)
    {
        if (!jsonRoot["accept"].isInt())
        {
            LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
            return;
        }

        int accept = jsonRoot["accept"].asInt();
        //���ܼӺ�������󣬽������ѹ�ϵ
        if (accept == 1)
        {
            if (!Singleton<UserManager>::Instance().makeFriendRelationshipInDB(targetUserid, m_userinfo.userid))
            {
                LOGE("make relationship error: %s, userid: %d, client:  %s", data.c_str(), m_userinfo.userid,  conn->peerAddress().toIpPort().c_str());
                return;
            }

            if (!Singleton<UserManager>::Instance().updateUserRelationshipInMemory(m_userinfo.userid, targetUserid, FRIEND_OPERATION_ADD))
            {
                LOGE("UpdateUserTeamInfo error: %s, , userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
                return;
            }
        }

        //{ "userid": 9, "type" : 3, "userid" : 9, "username" : "xxx", "accept" : 1 }
        snprintf(szData, 256, "{\"userid\": %d, \"type\": 3, \"username\": \"%s\", \"accept\": %d}", m_userinfo.userid, m_userinfo.username.c_str(), accept);

        //��ʾ�Լ���ǰ�û��Ӻ��ѳɹ�
        User targetUser;
        if (!Singleton<UserManager>::Instance().getUserInfoByUserId(targetUserid, targetUser))
        {
            LOGE("Get Userinfo by id error, targetuserid: %d, userid: %d, data: %s, client: %s", targetUserid, m_userinfo.userid, data.c_str(), conn->peerAddress().toIpPort().c_str());
            return;
        }
        char szSelfData[256] = { 0 };
        snprintf(szSelfData, 256, "{\"userid\": %d, \"type\": 3, \"username\": \"%s\", \"accept\": %d}", targetUser.userid, targetUser.username.c_str(), accept);
        send(msg_type_operatefriend, m_seq, szSelfData, strlen(szSelfData));
        LOGI("Response to client: userid: %d, cmd=msg_type_addfriend, data: %s", m_userinfo.userid, szSelfData);
    }

    //��ʾ�Է��Ӻ��ѳɹ�
    std::string outbuf;
    BinaryStreamWriter writeStream(&outbuf);
    writeStream.WriteInt32(msg_type_operatefriend);
    writeStream.WriteInt32(m_seq);
    writeStream.WriteCString(szData, strlen(szData));
    writeStream.Flush();

    //�ȿ�Ŀ���û��Ƿ�����
    std::list<std::shared_ptr<ChatSession>> sessions;
    Singleton<ChatServer>::Instance().getSessionsByUserId(sessions, targetUserid);
    //Ŀ���û������ߣ����������Ϣ
    if (sessions.empty())
    {
        Singleton<MsgCacheManager>::Instance().addNotifyMsgCache(targetUserid, outbuf);
        LOGI("userid: %d, is not online, cache notify msg, msg: %s", targetUserid, outbuf.c_str());
        return;
    }

    for (auto& iter : sessions)
    {
        iter->send(outbuf);
    }

    LOGI("Response to client: userid: %d, cmd=msg_type_addfriend, data: %s", targetUserid, data.c_str());
}

void ChatSession::onAddGroupResponse(int32_t groupId, const std::shared_ptr<TcpConnection>& conn)
{
    if (!Singleton<UserManager>::Instance().makeFriendRelationshipInDB(m_userinfo.userid, groupId))
    {
        LOGE("make relationship error, groupId: %d, userid: %d, client: %s", groupId, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    User groupUser;
    if (!Singleton<UserManager>::Instance().getUserInfoByUserId(groupId, groupUser))
    {
        LOGE("Get group info by id error, targetuserid: %d, userid: %d, client: %s", groupId, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    char szSelfData[256] = { 0 };
    snprintf(szSelfData, 256, "{\"userid\": %d, \"type\": 3, \"username\": \"%s\", \"accept\": 3}", groupUser.userid, groupUser.username.c_str());
    send(msg_type_operatefriend, m_seq, szSelfData, strlen(szSelfData));
    LOGI("Response to client: cmd=msg_type_addfriend, data: %s, userid: %d", szSelfData, m_userinfo.userid);

    if (!Singleton<UserManager>::Instance().updateUserRelationshipInMemory(m_userinfo.userid, groupId, FRIEND_OPERATION_ADD))
    {
        LOGE("UpdateUserTeamInfo error, targetUserid: %d, userid: %d, client: %s", groupId, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    //����������Ⱥ��Ա����Ⱥ��Ϣ�����仯����Ϣ
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(groupId, friends);
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    for (const auto& iter : friends)
    {
        //�ȿ�Ŀ���û��Ƿ�����
        std::list< std::shared_ptr<ChatSession>> targetSessions;
        imserver.getSessionsByUserId(targetSessions, iter.userid);
        for (auto& iter2 : targetSessions)
        {
            if (iter2)
                iter2->sendUserStatusChangeMsg(groupId, 3);
        }
    }
}

void ChatSession::onUpdateUserInfoResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;
    
    if (!jsonRoot["nickname"].isString() || !jsonRoot["facetype"].isInt() || 
        !jsonRoot["customface"].isString() || !jsonRoot["gender"].isInt() || 
        !jsonRoot["birthday"].isInt() || !jsonRoot["signature"].isString() || 
        !jsonRoot["address"].isString() || !jsonRoot["phonenumber"].isString() || 
        !jsonRoot["mail"].isString())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    User newuserinfo;
    newuserinfo.nickname = jsonRoot["nickname"].asString();
    newuserinfo.facetype = jsonRoot["facetype"].asInt();
    newuserinfo.customface = jsonRoot["customface"].asString();
    newuserinfo.gender = jsonRoot["gender"].asInt();
    newuserinfo.birthday = jsonRoot["birthday"].asInt();
    newuserinfo.signature = jsonRoot["signature"].asString();
    newuserinfo.address = jsonRoot["address"].asString();
    newuserinfo.phonenumber = jsonRoot["phonenumber"].asString();
    newuserinfo.mail = jsonRoot["mail"].asString();
    
    ostringstream retdata;
    ostringstream currentuserinfo;
    if (!Singleton<UserManager>::Instance().updateUserInfoInDb(m_userinfo.userid, newuserinfo))
    {
        retdata << "{ \"code\": 104, \"msg\": \"update user info failed\" }";
    }
    else
    {
        /*
        { "code": 0, "msg" : "ok", "userid" : 2, "username" : "xxxx", 
         "nickname":"zzz", "facetype" : 26, "customface" : "", "gender" : 0, "birthday" : 19900101, 
         "signature" : "xxxx", "address": "", "phonenumber": "", "mail":""}
        */
        currentuserinfo << "\"userid\": " << m_userinfo.userid << ",\"username\":\"" << m_userinfo.username
                        << "\", \"nickname\":\"" << newuserinfo.nickname
                        << "\", \"facetype\": " << newuserinfo.facetype << ", \"customface\":\"" << newuserinfo.customface
                        << "\", \"gender\":" << newuserinfo.gender
                        << ", \"birthday\":" << newuserinfo.birthday << ", \"signature\":\"" << newuserinfo.signature
                        << "\", \"address\": \"" << newuserinfo.address
                        << "\", \"phonenumber\": \"" << newuserinfo.phonenumber << "\", \"mail\":\""
                        << newuserinfo.mail;
        retdata << "{\"code\": 0, \"msg\": \"ok\"," << currentuserinfo.str()  << "\"}";
    }

    //Ӧ��ͻ���
    send(msg_type_updateuserinfo, m_seq, retdata.str());

    LOGI("Response to client: userid: %d, cmd=msg_type_updateuserinfo, data: %s", m_userinfo.userid, retdata.str().c_str());

    //���������ߺ������͸�����Ϣ�����ı���Ϣ
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(m_userinfo.userid, friends);
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    for (const auto& iter : friends)
    {
        //�ȿ�Ŀ���û��Ƿ�����
        std::list<std::shared_ptr<ChatSession>> targetSessions;
        imserver.getSessionsByUserId(targetSessions, iter.userid);
        for (auto& iter2 : targetSessions)
        {
            if (iter2)
                iter2->sendUserStatusChangeMsg(m_userinfo.userid, 3);
        }
    }
}

void ChatSession::onModifyPasswordResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{    
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;
    
    if (!jsonRoot["oldpassword"].isString() || !jsonRoot["newpassword"].isString())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    string oldpass = jsonRoot["oldpassword"].asString();
    string newPass = jsonRoot["newpassword"].asString();

    string retdata;
    User cachedUser;
    if (!Singleton<UserManager>::Instance().getUserInfoByUserId(m_userinfo.userid, cachedUser))
    {
        LOGE("get userinfo error, userid: %d, data: %s, client: %s", m_userinfo.userid, data.c_str(), conn->peerAddress().toIpPort().c_str());
        return;
    }

    if (cachedUser.password != oldpass)
    {
        retdata = "{\"code\": 103, \"msg\": \"incorrect old password\"}";
    }
    else
    {       
        if (!Singleton<UserManager>::Instance().modifyUserPassword(m_userinfo.userid, newPass))
        {
            retdata = "{\"code\": 105, \"msg\": \"modify password error\"}";
            LOGE("modify password error, userid: %d, data:%s, client: %s", m_userinfo.userid, data.c_str(), conn->peerAddress().toIpPort().c_str());
        }
        else
            retdata = "{\"code\": 0, \"msg\": \"ok\"}";
    }

    //Ӧ��ͻ���
    send(msg_type_modifypassword, m_seq, retdata);

    LOGI("Response to client: userid: %d, cmd=msg_type_modifypassword, data: %s", m_userinfo.userid, data.c_str());
}

void ChatSession::onCreateGroupResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot["groupname"].isString())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    ostringstream retdata;
    string groupname = jsonRoot["groupname"].asString();
    int32_t groupid;
    if (!Singleton<UserManager>::Instance().addGroup(groupname.c_str(), m_userinfo.userid, groupid))
    {
        LOGE("Add group error, data: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        retdata << "{ \"code\": 106, \"msg\" : \"create group error\"}";
    }
    else
    {
        retdata << "{\"code\": 0, \"msg\": \"ok\", \"groupid\":" << groupid << ", \"groupname\": \"" << groupname << "\"}";
    }

    //TODO: �������1�ɹ��ˣ�����2ʧ������ô�죿
    //����1
    //�����ɹ��Ժ���û��Զ���Ⱥ
    if (!Singleton<UserManager>::Instance().makeFriendRelationshipInDB(m_userinfo.userid, groupid))
    {
        LOGE("join in group, errordata: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    //�����ڴ��еĺ��ѹ�ϵ
    //����2
    if (!Singleton<UserManager>::Instance().updateUserRelationshipInMemory(m_userinfo.userid, groupid, FRIEND_OPERATION_ADD))
    {
        LOGE("UpdateUserTeamInfo error, data: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    //if (!Singleton<UserManager>::Instance().UpdateUserTeamInfo(groupid, m_userinfo.userid, FRIEND_OPERATION_ADD))
    //{
    //    LOGE << "UpdateUserTeamInfo error, data: " << data << ", userid: " << m_userinfo.userid << ", client: " << conn->peerAddress().toIpPort();
    //    return;
    //}


    //Ӧ��ͻ��ˣ���Ⱥ�ɹ�
    send(msg_type_creategroup, m_seq, retdata.str());

    LOGI("Response to client: userid: %d, cmd=msg_type_creategroup, data: %s", m_userinfo.userid, retdata.str().c_str());

    //Ӧ��ͻ��ˣ��ɹ���Ⱥ
    {
        char szSelfData[256] = { 0 };
        snprintf(szSelfData, 256, "{\"userid\": %d, \"type\": 3, \"username\": \"%s\", \"accept\": 1}", groupid, groupname.c_str());
        send(msg_type_operatefriend, m_seq, szSelfData, strlen(szSelfData));
        LOGI("Response to client, userid: %d, cmd=msg_type_addfriend, data: %s", m_userinfo.userid, szSelfData);
    }
}

void ChatSession::onGetGroupMembersResponse(const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    //{"groupid": Ⱥid}
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(data.c_str(), data.c_str() + data.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;
    
    if (!jsonRoot["groupid"].isInt())
    {
        LOGE("invalid json: %s, userid: %d, client: %s", data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    int32_t groupid = jsonRoot["groupid"].asInt();
    
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(groupid, friends);
    std::string strUserInfo;
    int useronline = 0;
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    for (const auto& iter : friends)
    {
        useronline = imserver.getUserStatusByUserId(iter.userid);
        /*
        {"code": 0, "msg": "ok", "members":[{"userid": 1,"username":"qqq,
        "nickname":"qqq, "facetype": 0, "customface":"", "gender":0, "birthday":19900101,
        "signature":", "address": "", "phonenumber": "", "mail":", "clienttype": 1, "status":1"]}
        */
        ostringstream osSingleUserInfo;
        osSingleUserInfo << "{\"userid\": " << iter.userid << ", \"username\":\"" << iter.username << "\", \"nickname\":\"" << iter.nickname
            << "\", \"facetype\": " << iter.facetype << ", \"customface\":\"" << iter.customface << "\", \"gender\":" << iter.gender
            << ", \"birthday\":" << iter.birthday << ", \"signature\":\"" << iter.signature << "\", \"address\": \"" << iter.address
            << "\", \"phonenumber\": \"" << iter.phonenumber << "\", \"mail\":\"" << iter.mail << "\", \"clienttype\": 1, \"status\":"
            << useronline << "}";

        strUserInfo += osSingleUserInfo.str();
        strUserInfo += ",";
    }
    //ȥ��������Ķ���
    strUserInfo = strUserInfo.substr(0, strUserInfo.length() - 1);
    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"groupid\": " << groupid << ", \"members\":[" << strUserInfo << "]}";
    send(msg_type_getgroupmembers, m_seq, os.str());

    LOGI("Response to client: userid: %d, cmd=msg_type_getgroupmembers, data: %s", m_userinfo.userid, os.str().c_str());
}

void ChatSession::sendUserStatusChangeMsg(int32_t userid, int type, int status/* = 0*/)
{
    string data; 
    //�û�����
    if (type == 1)
    {
        int32_t clientType = Singleton<ChatServer>::Instance().getUserClientTypeByUserId(userid);
        char szData[64];
        memset(szData, 0, sizeof(szData));
        sprintf(szData, "{ \"type\": 1, \"onlinestatus\": %d, \"clienttype\": %d}", status, clientType);
        data = szData;
    }
    //�û�����
    else if (type == 2)
    {
        data = "{\"type\": 2, \"onlinestatus\": 0}";
    }
    //�����ǳơ�ͷ��ǩ������Ϣ����
    else if (type == 3)
    {
        data = "{\"type\": 3}";
    }

    std::string outbuf;
    BinaryStreamWriter writeStream(&outbuf);
    writeStream.WriteInt32(msg_type_userstatuschange);
    writeStream.WriteInt32(m_seq);
    writeStream.WriteString(data);
    writeStream.WriteInt32(userid);
    writeStream.Flush();

    send(outbuf);

    LOGI("send to client: userid: %d, cmd=msg_type_userstatuschange, data: %s", m_userinfo.userid, data.c_str());
}

void ChatSession::makeSessionInvalid()
{
    m_userinfo.userid = 0;
}

bool ChatSession::isSessionValid()
{
    return m_userinfo.userid > 0;
}

void ChatSession::onChatResponse(int32_t targetid, const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    std::string modifiedChatData;
    if (!modifyChatMsgLocalTimeToServerTime(data, modifiedChatData))
    {
        LOGE("invalid chat json, chatjson: %s, senderid: %d, targetid: %d, chatmsg: %s, client: %s", data.c_str(), m_userinfo.userid, targetid, data.c_str(), conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    std::string outbuf;
    BinaryStreamWriter writeStream(&outbuf);
    writeStream.WriteInt32(msg_type_chat);
    writeStream.WriteInt32(m_seq);
    writeStream.WriteString(modifiedChatData);
    //��Ϣ������
    writeStream.WriteInt32(m_userinfo.userid);
    //��Ϣ������
    writeStream.WriteInt32(targetid);
    writeStream.Flush();

    UserManager& userMgr = Singleton<UserManager>::Instance();
    //д����Ϣ��¼
    if (!userMgr.saveChatMsgToDb(m_userinfo.userid, targetid, data))
    {
        LOGE("Write chat msg to db error, senderid: %d, targetid: %d, chatmsg: %s, client: %s", m_userinfo.userid, targetid, data.c_str(), conn->peerAddress().toIpPort().c_str());
    }

    ChatServer& imserver = Singleton<ChatServer>::Instance();
    MsgCacheManager& msgCacheMgr = Singleton<MsgCacheManager>::Instance();
    //������Ϣ
    if (targetid < GROUPID_BOUBDARY)
    {
        //�ȿ�Ŀ���û��Ƿ�����
        std::list<std::shared_ptr<ChatSession>> targetSessions;
        imserver.getSessionsByUserId(targetSessions, targetid);
        //Ŀ���û������ߣ����������Ϣ
        if (targetSessions.empty())
        {
            msgCacheMgr.addChatMsgCache(targetid, outbuf);
        }
        else
        {
            for (auto& iter : targetSessions)
            {
                if (iter)
                    iter->send(outbuf);
            }
        }
    }
    //Ⱥ����Ϣ
    else
    {       
        std::list<User> friends;
        userMgr.getFriendInfoByUserId(targetid, friends);
        std::string strUserInfo;
        bool useronline = false;
        for (const auto& iter : friends)
        {
            //�ų�Ⱥ��Ա�е��Լ�
            if (iter.userid == m_userinfo.userid)
                continue;

            //�ȿ�Ŀ���û��Ƿ�����
            std::list<std::shared_ptr<ChatSession>> targetSessions;
            imserver.getSessionsByUserId(targetSessions, iter.userid);
            //Ŀ���û������ߣ����������Ϣ
            if (targetSessions.empty())
            {
                msgCacheMgr.addChatMsgCache(iter.userid, outbuf);
                continue;
            }
            else
            {
                for (auto& iter2 : targetSessions)
                {
                    if (iter2)
                        iter2->send(outbuf);
                }
            }
        }
    }   
}

void ChatSession::onMultiChatResponse(const std::string& targets, const std::string& data, const std::shared_ptr<TcpConnection>& conn)
{
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(targets.c_str(), targets.c_str() + targets.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("invalid targets string: targets: %s, data: %s, userid: %d, , client: %s", targets.c_str(), data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;    

    if (!jsonRoot["targets"].isArray())
    {
        LOGE("[targets] node is not array in targets string: targets: %s, data: %s, userid: %d, client: %s", targets.c_str(), data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    for (uint32_t i = 0; i < jsonRoot["targets"].size(); ++i)
    {
        onChatResponse(jsonRoot["targets"][i].asInt(), data, conn);
    }

    LOGI("send to client, cmd=msg_type_multichat, targets: %s, data: %s, from userid: %d, from client: %s", targets.c_str(), data.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
}

void ChatSession::onScreenshotResponse(int32_t targetid, const std::string& bmpHeader, const std::string& bmpData, const std::shared_ptr<TcpConnection>& conn)
{
    std::string outbuf;
    BinaryStreamWriter writeStream(&outbuf);
    writeStream.WriteInt32(msg_type_remotedesktop);
    writeStream.WriteInt32(m_seq);
    std::string dummy;
    writeStream.WriteString(dummy);
    writeStream.WriteString(bmpHeader);
    writeStream.WriteString(bmpData);
    //��Ϣ������
    writeStream.WriteInt32(targetid);
    writeStream.Flush();

    ChatServer& imserver = Singleton<ChatServer>::Instance();
    //������Ϣ
    if (targetid >= GROUPID_BOUBDARY)
        return;

    std::list<std::shared_ptr<ChatSession>> targetSessions;
    imserver.getSessionsByUserId(targetSessions, targetid);
    //�ȿ�Ŀ���û����߲�ת��
    if (!targetSessions.empty())
    {
        for (auto& iter : targetSessions)
        {
            if (iter)
                iter->send(outbuf);
        }
    }
}

void ChatSession::onUpdateTeamInfoResponse(int32_t operationType, const std::string& newTeamName, const std::string& oldTeamName, const std::shared_ptr<TcpConnection>& conn)
{
    if (operationType < updateteaminfo_operation_add || operationType > updateteaminfo_operation_modify)
    {
        LOGE("invalid teaminfo, userid: %d, , client: %s", m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    string teaminfo;
    if (!Singleton<UserManager>::Instance().getTeamInfoByUserId(m_userinfo.userid, teaminfo))
    {
        LOGE("GetTeamInfoByUserId failed, userid: %d, client: %s", m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        //TODO: Ӧ��Ӧ��һ�¿ͻ���
        return;
    }

    if (teaminfo.empty())
    {
        teaminfo = "[{\"teamname\": \"";
        teaminfo += DEFAULT_TEAMNAME;
        teaminfo += "\", \"members\":[]}]";
    }

    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(teaminfo.c_str(), teaminfo.c_str() + teaminfo.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        //TODO: Ӧ��Ӧ��һ�¿ͻ���
        LOGE("parse teaminfo json failed, userid: %d, client: %s", m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    string newTeamInfo;

    //��������
    if (operationType == updateteaminfo_operation_add)
    {
        uint32_t teamCount = jsonRoot.size();
        for (uint32_t i = 0; i < teamCount; ++i)
        {
            if (!jsonRoot[i]["teamname"].isNull() && jsonRoot[i]["teamname"].asString() == newTeamName)
            {
                //TODO: ��ʾ�ͻ��˷����Ѿ�����
                LOGE("teamname not exist, userid: %d, client: %s", m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
                return;
            }
        }
        
        jsonRoot[teamCount]["teamname"] = newTeamName;
        Json::Value emptyArrayValue(Json::arrayValue);
        jsonRoot[teamCount]["members"] = emptyArrayValue;

        //Json::FastWriter writer;
        //newTeamInfo = writer.write(JsonRoot);

        Json::StreamWriterBuilder streamWriterBuilder;
        //����json�е�\t��\n����
        streamWriterBuilder.settings_["indentation"] = "";
        newTeamInfo = Json::writeString(streamWriterBuilder, jsonRoot);      
    }
    else if (operationType == updateteaminfo_operation_delete)
    {
        if (oldTeamName == DEFAULT_TEAMNAME)
        {
            //Ĭ�Ϸ��鲻����ɾ��
            //TODO: ��ʾ�ͻ���
            return;
        }
        
        bool found = false;
        uint32_t teamCount = jsonRoot.size();
        for (uint32_t i = 0; i < teamCount; ++i)
        {
            if (!jsonRoot[i]["teamname"].isNull() && jsonRoot[i]["teamname"].asString() == oldTeamName)
            {
                found = true;
                //TODO������������
                jsonRoot.removeIndex(i, &jsonRoot[i]["teamname"]);

                //�����ݿ��и���ĺ����ƶ���Ĭ�Ϸ���
                if (!Singleton<UserManager>::Instance().deleteTeam(m_userinfo.userid, oldTeamName))
                {
                    LOGE("Delete team error, oldTeamName: %s, userid: %s, client: %s", oldTeamName.c_str(), m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
                    return;
                }
                          
                break;
            }
        }

        if (!found)
        {
            //��ʾ�ͻ��˷�����������
            LOGE("teamname not exist, userid: %d, client: %s", m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        }

        //Json::FastWriter writer;
        //newTeamInfo = writer.write(JsonRoot);

        Json::StreamWriterBuilder streamWriterBuilder;
        //����json�е�\t��\n����
        streamWriterBuilder.settings_["indentation"] = "";
        newTeamInfo = Json::writeString(streamWriterBuilder, jsonRoot);
    }
    //�޸ķ�����
    else
    {
        if (oldTeamName == DEFAULT_TEAMNAME)
        {
            //Ĭ�Ϸ��鲻�����޸�
            //TODO: ��ʾ�ͻ���
            return;
        }
        
        //�޸ķ�����
        bool found = false;
        uint32_t teamCount = jsonRoot.size();
        for (uint32_t i = 0; i < teamCount; ++i)
        {
            if (!jsonRoot[i]["teamname"].isNull() && jsonRoot[i]["teamname"].asString() == oldTeamName)
            {
                found = true;
                jsonRoot[i]["teamname"] = newTeamName;
              
                break;
            }
        }

        if (!found)
        {
            //��ʾ�ͻ��˷�����������
        }

        if (!Singleton<UserManager>::Instance().modifyTeamName(m_userinfo.userid, newTeamName, oldTeamName))
        {
            LOGE("Update team info failed, userid: %d, newTeamInfo: %s, oldTeamInfo: %s, client: %s", m_userinfo.userid, newTeamInfo.c_str(), oldTeamName.c_str(), conn->peerAddress().toIpPort().c_str());
            return;
        }

        //Json::FastWriter writer;
        //newTeamInfo = writer.write(JsonRoot);

        Json::StreamWriterBuilder streamWriterBuilder;
        streamWriterBuilder.settings_["indentation"] = "";
        newTeamInfo = Json::writeString(streamWriterBuilder, jsonRoot);
    }
 
    //���浽���ݿ�����ȥ��������Ϣ���͸����ڴ��еķ�����Ϣ
    if (!Singleton<UserManager>::Instance().updateUserTeamInfoInDbAndMemory(m_userinfo.userid, newTeamInfo))
    {
        //TODO: ʧ��Ӧ��ͻ���
        LOGE("Update team info failed, userid: %d, , newTeamInfo: %s, , client: %s", m_userinfo.userid, newTeamInfo.c_str(), conn->peerAddress().toIpPort().c_str());
        return;
    }

    std::string friendinfo;
    makeUpFriendListInfo(friendinfo, conn);

    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendinfo << "}";
    send(msg_type_getofriendlist, m_seq, os.str());

    LOGI("Response to client, userid: %d, cmd=msg_type_getofriendlist, data: %s", m_userinfo.userid, os.str().c_str());
}

void ChatSession::onModifyMarknameResponse(int32_t friendid, const std::string& newmarkname, const std::shared_ptr<TcpConnection>& conn)
{
    if (!Singleton<UserManager>::Instance().updateMarknameInDb(m_userinfo.userid, friendid, newmarkname))
    {
        //TODO: ʧ��Ӧ��ͻ���
        LOGE("Update markname failed, userid: %d, friendid: %d, client: %s", m_userinfo.userid, friendid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    std::string friendinfo;
    makeUpFriendListInfo(friendinfo, conn);

    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendinfo << "}";
    send(msg_type_getofriendlist, m_seq, os.str());

    LOGI("Response to client, userid: %d, cmd=msg_type_getofriendlist, data: %s", m_userinfo.userid, os.str().c_str());
}

void ChatSession::onMoveFriendToOtherTeamResponse(int32_t friendid, const std::string& newteamname, const std::string& oldteamname, const std::shared_ptr<TcpConnection>& conn)
{
    if (newteamname.empty() || oldteamname.empty() || newteamname == oldteamname)
    {
        LOGE("Failed to move to other team, newteamname or oldteamname is invalid, userid: %d, friendid:%d, client: %s", m_userinfo.userid, friendid, conn->peerAddress().toIpPort().c_str());
        //TODO: ֪ͨ�ͻ���
        return;
    }
    
    //������ĺ��ѣ����ܲ���
    if (!Singleton<UserManager>::Instance().isFriend(m_userinfo.userid, friendid))
    {
        LOGE("Failed to move to other team, not your friend, userid: %d, friendid: %d, client: %s", m_userinfo.userid, friendid, conn->peerAddress().toIpPort().c_str());
        //TODO: ֪ͨ�ͻ���
        return;
    }

    User currentUser;
    if (!Singleton<UserManager>::Instance().getUserInfoByUserId(m_userinfo.userid, currentUser))
    {
        LOGE("User not exist in memory, userid: %d", m_userinfo.userid);
        //TODO: ֪ͨ�ͻ���
        return;
    }

    string teaminfo = currentUser.teaminfo;
    if (teaminfo.empty())
    {
        teaminfo = "[\"teamname\":\"";
        teaminfo += DEFAULT_TEAMNAME;
        teaminfo += "\"]";
    }

    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(teaminfo.c_str(), teaminfo.c_str() + teaminfo.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("parse teaminfo json failed, userid: %d, teaminfo: %s, client: %s", m_userinfo.userid, teaminfo.c_str(), conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot.isArray())
    {
        LOGE("parse teaminfo json failed, userid: %d, teaminfo: %s, client: %s",  m_userinfo.userid, teaminfo.c_str(), conn->peerAddress().toIpPort().c_str());
        return;
    }

    bool foundNewTeam = false;
    bool foundOldTeam = false;
    for (uint32_t i = 0; i < jsonRoot.size(); ++i)
    {
        if (jsonRoot[i]["teamname"].isString())
        {
            if (jsonRoot[i]["teamname"].asString() == newteamname)
            {
                foundNewTeam = true;
                continue;
            }
            else if (jsonRoot[i]["teamname"].asString() == oldteamname)
            {
                foundOldTeam = true;
                continue;
            }
        }
    }

    if (!foundNewTeam || !foundOldTeam)
    {
        LOGE("Failed to move to other team, oldTeamName or newTeamName not exist, userid: %d, friendid: %d, oldTeamName: %s, newTeamName: %s, client: %s", 
            m_userinfo.userid, friendid, oldteamname.c_str(), newteamname.c_str(), conn->peerAddress().toIpPort().c_str());       
        return;
    }

    if (!Singleton<UserManager>::Instance().moveFriendToOtherTeam(m_userinfo.userid, friendid, newteamname))
    {
        LOGE("Failed to MoveFriendToOtherTeam, db operation error, userid: %d, friendid: %d, client: %s", m_userinfo.userid, friendid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    std::string friendinfo;
    makeUpFriendListInfo(friendinfo, conn);

    std::ostringstream os;
    os << "{\"code\": 0, \"msg\": \"ok\", \"userinfo\":" << friendinfo << "}";
    send(msg_type_getofriendlist, m_seq, os.str());

    LOGI("Response to client: userid: %d, cmd=msg_type_getofriendlist, data: %s", m_userinfo.userid, os.str().c_str());
}

void ChatSession::deleteFriend(const std::shared_ptr<TcpConnection>& conn, int32_t friendid)
{
    /**
    *  �������ѣ������Ӻ��ѡ�ɾ������
    **/
    /*
    //typeΪ1�����Ӻ������� 2 �յ��Ӻ�������(���ͻ���ʹ��) 3Ӧ��Ӻ��� 4ɾ���������� 5Ӧ��ɾ������
    //��type=3ʱ��accept�Ǳ����ֶΣ�0�Է��ܾ���1�Է�����
    cmd = 1005, seq = 0, {"userid": 9, "type": 1}
    cmd = 1005, seq = 0, {"userid": 9, "type": 2, "username": "xxx"}
    cmd = 1005, seq = 0, {"userid": 9, "type": 3, "username": "xxx", "accept": 1}

    //����
    cmd = 1005, seq = 0, {"userid": 9, "type": 4}
    //Ӧ��
    cmd = 1005, seq = 0, {"userid": 9, "type": 5, "username": "xxx"}
    **/

    if (!Singleton<UserManager>::Instance().releaseFriendRelationshipInDBAndMemory(friendid, m_userinfo.userid))
    {
        LOGE("Delete friend error, friendid: %d, userid: %d, client: %d", friendid, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    //����һ�µ�ǰ�û��ķ�����Ϣ
    User cachedUser;
    if (!Singleton<UserManager>::Instance().getUserInfoByUserId(friendid, cachedUser))
    {
        LOGE("Delete friend - Get user error, friendid: %d, userid: %d, client: %s", friendid, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }

    if (!Singleton<UserManager>::Instance().updateUserRelationshipInMemory(m_userinfo.userid, friendid, FRIEND_OPERATION_DELETE))
    {
        LOGE("UpdateUserTeamInfo failed, friendid: %d, userid: %d, client: %s", friendid, m_userinfo.userid, conn->peerAddress().toIpPort().c_str());
        return;
    }
    
    char szData[256] = { 0 };
    //��������ɾ����һ��
    //{"userid": 9, "type": 1, }        
    snprintf(szData, 256, "{\"userid\":%d, \"type\":5, \"username\": \"%s\"}", friendid, cachedUser.username.c_str());
    send(msg_type_operatefriend, m_seq, szData, strlen(szData));

    LOGI("send to client: userid�� %d, cmd=msg_type_operatefriend, data: %s", m_userinfo.userid, szData);

    //������ɾ����һ��
    //ɾ��������Ϣ
    if (friendid < GROUPID_BOUBDARY)
    {
        //�ȿ�Ŀ���û��Ƿ�����
        std::list<std::shared_ptr<ChatSession>>targetSessions;
        Singleton<ChatServer>::Instance().getSessionsByUserId(targetSessions, friendid);
        //���������û����������Ϣ
        if (!targetSessions.empty())
        {
            memset(szData, 0, sizeof(szData));
            snprintf(szData, 256, "{\"userid\":%d, \"type\":5, \"username\": \"%s\"}", m_userinfo.userid, m_userinfo.username.c_str());
            for (auto& iter : targetSessions)
            {
                if (iter)
                    iter->send(msg_type_operatefriend, m_seq, szData, strlen(szData));
            }

            LOGI("send to client: userid: %d, cmd=msg_type_operatefriend, data: %s", friendid, szData);
        }

        return;
    }
    
    //��Ⱥ��Ϣ
    //����������Ⱥ��Ա����Ⱥ��Ϣ�����仯����Ϣ
    std::list<User> friends;
    Singleton<UserManager>::Instance().getFriendInfoByUserId(friendid, friends);
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    for (const auto& iter : friends)
    {
        //�ȿ�Ŀ���û��Ƿ�����
        std::list<std::shared_ptr<ChatSession>> targetSessions;
        imserver.getSessionsByUserId(targetSessions, iter.userid);
        if (!targetSessions.empty())
        {
            for (auto& iter2 : targetSessions)
            {
                if (iter2)
                    iter2->sendUserStatusChangeMsg(friendid, 3);
            }
        }
    }

}

void ChatSession::makeUpFriendListInfo(std::string& friendinfo, const std::shared_ptr<TcpConnection>& conn)
{
    std::string teaminfo;
    UserManager& userManager = Singleton<UserManager>::Instance();
    ChatServer& imserver = Singleton<ChatServer>::Instance();
    userManager.getTeamInfoByUserId(m_userinfo.userid, teaminfo);

    /*
    [
    {
    "teamindex": 0,
    "teamname": "�ҵĺ���",
    "members": [
    {
    "userid": 1,
    
    },
    {
    "userid": 2,
    "markname": "��xx"
    }
    ]
    }
    ]
    */

    string markname = "";
    if (teaminfo.empty())
    {
        teaminfo = "[{\"teamname\": \"";
        teaminfo += DEFAULT_TEAMNAME;
        teaminfo += "\", \"members\": []}]";
    }
           
    Json::Value emptyArrayValue(Json::arrayValue);

    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(teaminfo.c_str(), teaminfo.c_str() + teaminfo.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("parse teaminfo json failed, userid: %d, teaminfo: %s, client: %s", m_userinfo.userid, teaminfo.c_str(), conn->peerAddress().toIpPort().c_str());
        delete reader;
        return;
    }
    delete reader;

    if (!jsonRoot.isArray())
    {
        LOGE("parse teaminfo json failed, userid: %d, teaminfo: %s, client: %s", m_userinfo.userid, teaminfo.c_str(),  conn->peerAddress().toIpPort().c_str());
        return;
    }

    // ����������Ϣ����Ӻ���������Ϣ
    uint32_t teamCount = jsonRoot.size();
    int32_t userid = 0;

    //std::list<User> friends;
    User currentUserInfo;
    userManager.getUserInfoByUserId(m_userinfo.userid, currentUserInfo);
    User u;
    for (auto& friendinfo : currentUserInfo.friends)
    {
        for (uint32_t i = 0; i < teamCount; ++i)
        {
            if (jsonRoot[i]["members"].isNull() || !(jsonRoot[i]["members"]).isArray())
            {
                jsonRoot[i]["members"] = emptyArrayValue;
            }

            if (jsonRoot[i]["teamname"].isNull() || jsonRoot[i]["teamname"].asString() != friendinfo.teamname)
                continue;
            
            uint32_t memberCount = jsonRoot[i]["members"].size();
                                            
            if (!userManager.getUserInfoByUserId(friendinfo.friendid, u))
                continue;

            if (!userManager.getFriendMarknameByUserId(m_userinfo.userid, friendinfo.friendid, markname))
                continue;

            jsonRoot[i]["members"][memberCount]["userid"] = u.userid;
            jsonRoot[i]["members"][memberCount]["username"] = u.username;
            jsonRoot[i]["members"][memberCount]["nickname"] = u.nickname;
            jsonRoot[i]["members"][memberCount]["markname"] = markname;
            jsonRoot[i]["members"][memberCount]["facetype"] = u.facetype;
            jsonRoot[i]["members"][memberCount]["customface"] = u.customface;
            jsonRoot[i]["members"][memberCount]["gender"] = u.gender;
            jsonRoot[i]["members"][memberCount]["birthday"] = u.birthday;
            jsonRoot[i]["members"][memberCount]["signature"] = u.signature;
            jsonRoot[i]["members"][memberCount]["address"] = u.address;
            jsonRoot[i]["members"][memberCount]["phonenumber"] = u.phonenumber;
            jsonRoot[i]["members"][memberCount]["mail"] = u.mail;
            jsonRoot[i]["members"][memberCount]["clienttype"] = imserver.getUserClientTypeByUserId(friendinfo.friendid);
            jsonRoot[i]["members"][memberCount]["status"] = imserver.getUserStatusByUserId(friendinfo.friendid);;
       }// end inner for-loop
        
    }// end outer for - loop

    //JsonRoot.toStyledString()���ص��Ǹ�ʽ���õ�json����ʵ��
    //friendinfo = JsonRoot.toStyledString();
    //Json::FastWriter writer;
    //friendinfo = writer.write(JsonRoot); 

    Json::StreamWriterBuilder streamWriterBuilder;
    streamWriterBuilder.settings_["indentation"] = "";
    friendinfo = Json::writeString(streamWriterBuilder, jsonRoot);
}

bool ChatSession::modifyChatMsgLocalTimeToServerTime(const std::string& chatInputJson, std::string& chatOutputJson)
{
    /*
        ��Ϣ��ʽ��
        {
            "msgType": 1, //��Ϣ���� 0δ֪���� 1�ı� 2���ڶ��� 3�ļ�
            "time": 2434167,
            "clientType": 0,		//0δ֪ 1pc�� 2ƻ���� 3��׿��
            "font":["fontname", fontSize, fontColor, fontBold, fontItalic, fontUnderline],
            "content":
            [
                {"msgText": "text1"},
                {"msgText": "text2"},
                {"faceID": 101},
                {"faceID": 102},
                {"pic": ["name", "server_path", 400, w, h]},
                {"remotedesktop": 1},
                {"shake": 1},
                {"file":["name", "server_path", 400, onlineflag]}		//onlineflagΪ0�������ļ�����Ϊ0Ϊ�����ļ�
            ]
        }
    */
    if (chatInputJson.empty())
        return false;
    
    Json::CharReaderBuilder b;
    Json::CharReader* reader(b.newCharReader());
    Json::Value jsonRoot;
    JSONCPP_STRING errs;
    bool ok = reader->parse(chatInputJson.c_str(), chatInputJson.c_str() + chatInputJson.length(), &jsonRoot, &errs);
    if (!ok || errs.size() != 0)
    {
        LOGE("parse chatInputJson json failed, userid: %d, chatInputJson: %s", m_userinfo.userid, chatInputJson.c_str());
        delete reader;
        return false;
    }
    delete reader;

    unsigned int now = (unsigned int)time(NULL);
    //if (JsonRoot["time"].isNull())
    jsonRoot["time"] = now;

    //Json::FastWriter writer;
    //chatOutputJson = writer.write(JsonRoot);
    Json::StreamWriterBuilder streamWriterBuilder;
    //����json�е�\t��\n����
    streamWriterBuilder.settings_["indentation"] = "";
    chatOutputJson = Json::writeString(streamWriterBuilder, jsonRoot);

    return true;
}

void ChatSession::enableHearbeatCheck()
{
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {        
        //ÿ15���Ӽ��һ���Ƿ��е�������
        m_checkOnlineTimerId = conn->getLoop()->runEvery(15000000, std::bind(&ChatSession::checkHeartbeat, this, conn));
    }
}

void ChatSession::disableHeartbeatCheck()
{
    std::shared_ptr<TcpConnection> conn = getConnectionPtr();
    if (conn)
    {
        LOGI("remove check online timerId, userid: %d, clientType: %d, client address: %s", m_userinfo.userid, m_userinfo.clienttype, conn->peerAddress().toIpPort().c_str());        
        conn->getLoop()->cancel(m_checkOnlineTimerId, true);
    }
}

void ChatSession::checkHeartbeat(const std::shared_ptr<TcpConnection>& conn)
{   
    if (!conn)
        return;
    
    //LOGI("check heartbeat, userid: %d, clientType: %d, client address: %s", m_userinfo.userid, m_userinfo.clienttype, conn->peerAddress().toIpPort().c_str());

    if (time(NULL) - m_lastPackageTime < MAX_NO_PACKAGE_INTERVAL)
        return;
    
    conn->forceClose();
    //LOGI("in max no-package time, no package, close the connection, userid: %d, clientType: %d, client address: %s", m_userinfo.userid, m_userinfo.clienttype, conn->peerAddress().toIpPort().c_str());
}