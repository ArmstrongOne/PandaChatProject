/** 
 *  �������е��û���Ϣ����ʼ��Ϣ�����ݿ��м���, UserManager.h
 *  zhangyl 2017.03.15
 **/

#pragma once
#include <stdint.h>
#include <string>
#include <list>
#include <mutex>
#include <set>
#include <atomic>

using namespace std;

#define GROUPID_BOUBDARY   0x0FFFFFFF 

#define DEFAULT_TEAMNAME  "My Friends"

enum FRIEND_OPERATION
{
    FRIEND_OPERATION_ADD,
    FRIEND_OPERATION_DELETE
};

struct FriendInfo
{
    int32_t friendid;
    string  markname;
    string  teamname;
};

//�û�����Ⱥ
struct User
{
    int32_t        userid;      //0x0FFFFFFF������Ⱥ�ţ���������ͨ�û�
    string         username;    //Ⱥ�˻���usernameҲ��Ⱥ��userid���ַ�����ʽ
    string         password;
    string         nickname;    //Ⱥ�˺�ΪȺ����
    int32_t        facetype;
    string         customface;
    string         customfacefmt;//�Զ���ͷ���ʽ
    int32_t        gender;
    int32_t        birthday;
    string         signature;
    string         address;
    string         phonenumber;
    string         mail;
    /*
    �����û����ѷ�����Ϣ������Ⱥ�˻���Ϊ�գ�����:
    [{"teamname": "�ҵĺ���"}, {"teamname": "�ҵ�ͬ��"}, {"teamname": "��ҵ�ͻ�"}]
    */
    string             teaminfo;       //������ͨ�û���Ϊ������Ϣ������Ⱥ����Ϊ��
    int32_t            ownerid;        //����Ⱥ�˺ţ�ΪȺ��userid
    list<FriendInfo>   friends;        
};

class UserManager final
{
public:
    UserManager();
    ~UserManager();

    bool init(const char* dbServer, const char* dbUserName, const char* dbPassword, const char* dbName);

    UserManager(const UserManager& rhs) = delete;
    UserManager& operator=(const UserManager& rhs) = delete;

    bool addUser(User& u);
    bool makeFriendRelationshipInDB(int32_t smallUserid, int32_t greaterUserid);
    bool releaseFriendRelationshipInDBAndMemory(int32_t smallUserid, int32_t greaterUserid);
    bool updateUserRelationshipInMemory(int32_t userid, int32_t target, FRIEND_OPERATION operation);
    bool addFriendToUser(int32_t userid, int32_t friendid);
    bool deleteFriendToUser(int32_t userid, int32_t friendid);

    bool isFriend(int32_t userid, int32_t friendid);
    
    //TODO: ���඼�Ǹ����û���Ϣ�Ľӿڣ��������Կ���ͳһ����
    bool updateUserInfoInDb(int32_t userid, const User& newuserinfo);
    bool modifyUserPassword(int32_t userid, const std::string& newpassword);
    //���ڴ�����ݿ��е�ĳ���û��ķ�����Ϣ�ĳ��µ�newteaminfo
    bool updateUserTeamInfoInDbAndMemory(int32_t userid, const std::string& newteaminfo);
    bool deleteTeam(int32_t userid, const std::string& deletedteamname);
    bool modifyTeamName(int32_t userid, const std::string& newteamname, const std::string& oldteamname);
    
    //�����û����ѱ�ע��
    bool updateMarknameInDb(int32_t userid, int32_t friendid, const std::string& newmarkname);
    //�ƶ���������������
    bool moveFriendToOtherTeam(int32_t userid, int32_t friendid, const std::string& newteamname);

    bool addGroup(const char* groupname, int32_t ownerid, int32_t& groupid);

    //������Ϣ���
    bool saveChatMsgToDb(int32_t senderid, int32_t targetid, const std::string& chatmsg);

    //TODO: ���û�Խ��Խ�࣬������Խ��Խ���ʱ�����ϵ�еĺ���Ч�ʸ���
    bool getUserInfoByUsername(const std::string& username, User& u);
    bool getUserInfoByUserId(int32_t userid, User& u);
    bool getUserInfoByUserId(int32_t userid, User*& u);
    bool getFriendInfoByUserId(int32_t userid, std::list<User>& friends);
    //��ȡ���ѵı�ע��
    bool getFriendMarknameByUserId(int32_t userid1, int32_t friendid, std::string& markname);
    bool getTeamInfoByUserId(int32_t userid, std::string& teaminfo);

private:
    bool loadUsersFromDb();
    bool loadRelationshipFromDb(int32_t userid, std::list<FriendInfo>& r);

private:
    std::atomic_int     m_baseUserId{ 0 };        //m_baseUserId, ȡ���ݿ�����userid���ֵ�������û�����������ϵ���
    std::atomic<int>    m_baseGroupId{0x0FFFFFFF};
    list<User>          m_allCachedUsers;
    mutex               m_mutex;

    string              m_strDbServer;
    string              m_strDbUserName;
    string              m_strDbPassword;
    string              m_strDbName;
};