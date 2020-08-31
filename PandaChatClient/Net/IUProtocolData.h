//Э�鼰״̬����

#pragma once
#include <stdint.h>
#include <set>
#include <list>
#include <map>
#include <string>

#define DEFAULT_TEAMNAME _T("My Friends")

//�ͻ�������
enum CLIENT_TYPE
{
	CLIENT_TYPE_UNKNOWN = 0,
	CLIENT_TYPE_PC      = 1,
	CLIENT_TYPE_IOS		= 2,
	CLIENT_TYPE_ANDROID = 3
};

//����״̬
enum ONLINE_STATUS
{
	STATUS_OFFLINE		  = 0,			//����
	STATUS_ONLINE		  = 1,			//����
	STATUS_INVISIBLE	  = 2,			//����
	STATUS_BUSY			  = 3,			//æµ
	STATUS_AWAY			  = 4,			//�뿪
	STATUS_MOBILE_ONLINE  = 5,			//�ƶ�������
	STATUS_MOBILE_OFFLINE = 6,			//�ƶ�������
	STATUS_BOTH			  = 7			//�ֻ��͵���ͬʱ����
};

enum ONLINE_CLIENT_TYPE
{	
    ONLINE_CLIENT_UNKONWN,
    ONLINE_CLIENT_PC,
	ONLINE_CLIENT_MOBILE,
	ONLINE_CLIENT_BOTH,
	OFFLINE_CLIENT_BOTH
};

enum LOGIN_TYPE
{
	LOGIN_USE_MOBILE_NUMBER = 0,	//ʹ���ֻ��ŵ�¼
	LOGIN_USE_ACCOUNT		= 1,	//ʹ���˺ŵ�¼
};

enum LOGIN_RESULT_CODE	// ��¼�������
{
	LOGIN_SUCCESS,				// ��¼�ɹ�
	LOGIN_FAILED,				// ��¼ʧ��
	LOGIN_UNREGISTERED,			// �û�δע��
	LOGIN_PASSWORD_ERROR,		// �������
	LOGIN_USER_CANCEL_LOGIN,	// �û�ȡ����¼
	LOGIN_SERVER_REFUSED,		// �������ܾ�
	LOGIN_SERVER_NOT_RESPONSE	// ������δ��Ӧ
};

enum REGISTER_RESULT_CODE	//ע��������
{
	REGISTER_SUCCESS,				//ע��ɹ�
	REGISTER_EXIST,					//�˺��Ѿ�����
	REGISTER_FAILED,				//ע��ʧ��
	REGISTER_SERVER_NOT_RESPONSE	//������δ��Ӧ
};

enum HEARTBEAT_RESULT_CODE
{
	HEARTBEAT_ALIVE,				//��Ȼ��������
	HEARTBEAT_DEAD,					//�Ѿ��Ͽ�����
};

//�Ӻ���״̬
enum OPERATE_FRIEND
{
    Apply,
    Agree,
    Refuse,
    Delete
};

enum ADD_FRIEND_RESULT	//��Ӻ��ѽ��
{
	FIND_FRIEND_NOT_FOUND,
	FIND_FRIEND_FOUND,
	FIND_FRIEND_FAILED,
	
	ADD_FRIEND_SUCCESS,
	ADD_FRIEND_FAILED
};

enum DELETE_FRIEND_RESULT
{
	DELETE_FRIEND_SUCCESS,
	DELETE_FRIEND_FAILED
};

enum SEND_CHAT_MSG_RESULT		//����������Ϣ���
{
	SEND_MSG_SUCESS,
	SEND_WHOLE_MSG_FAILED,		//����������Ϣʧ��
	SEND_IMGAE_FAILED,			//����ͼƬʧ��
	//SEND_FILE_FAILED			//�����ļ�ʧ��
};

enum UPLOAD_USER_THUMB_RESULT
{
	UPLOAD_USER_THUMB_RESULT_SUCCESS,
	UPLOAD_USER_THUMB_RESULT_FAILED
};

enum SEND_FILE_RESULT
{
	SEND_FILE_FAILED,
	SEND_FILE_SUCCESS
};

enum RECV_FILE_RESULT
{
	RECV_FILE_FAILED,
	RECV_FILE_SUCCESS
};

enum GET_USER_INFO_TYPE
{
	USER_INFO_TYPE_SELF,
	USER_INFO_TYPE_FRIENDS,
	USER_INFO_TYPE_GROUP,
	USER_INFO_TYPE_GROUP_MEMBER
};

struct FindUserOrGroupResult
{
	long		nType;			//���˻���Ⱥ��
	UINT		uAccountID;
	TCHAR		szAccountName[32];
	TCHAR		szNickName[32];
};

//��¼��ǰ��Ӻ��ѵ�״̬��Ϣ
struct AddFriendInfo
{
	UCHAR	nCmd;				//�������ͣ�Apply/Agree/Refuse��
	UINT	uAccountID;			//�˻�ID
	TCHAR	szAccountName[32];	//�˻���
	TCHAR	szNickName[32];		//�ǳ�
};

enum CHAT_MSG_TYPE
{
	FMG_MSG_TYPE_UNKNOWN,
	FMG_MSG_TYPE_BUDDY,
	FMG_MSG_TYPE_GROUP,
	FMG_MSG_TYPE_MULTI,		//Ⱥ����Ϣ
	FMG_MSG_TYPE_SESS
};

struct UserBasicInfo
{
	UINT  uAccountID;
	char  szAccountName[32];
	char  szNickName[64];
    char  szMarkName[64];
    char  szSignature[256];
    char  szAddress[51225];
	UINT  uFaceID;
    int   nStatus;              //����״̬
    int   clientType;           //�ͻ�������
    char  customFace[64];
    int   nGender;
    int   nBirthday;
    char  szPhoneNumber[64];
    char  szMail[64];
};

struct UserExtendInfo
{
	UINT	uAccountID;
	UINT	uGender;
	UINT	uShengXiao;
	UINT	uConstel;
	UINT    uBlood;
	UINT	uBirthday;

	char	szCountry[32];			// ����
	char	szProvince[32];			// ʡ��
	char	szCity[32];				// ����
	char	szPhone[32];			// �绰
	char	szMobile[32];			// �ֻ�
	char	szOccupation[32];		// ְҵ
	char	szCollege[32];			// ��ҵԺУ
	char	szEmail[48];			// ����
	char	szHomepage[64];			// ������ҳ
	char	szSignature[256];		// ����ǩ��
	char	szPersonal[512];		// ����˵��	
	char    szCustomFace[MAX_PATH];	// �Զ���ͷ����
};

//�ļ��ϴ���������
enum FILE_ITEM_TYPE
{
	FILE_ITEM_UNKNOWN,
	FILE_ITEM_UPLOAD_CHAT_IMAGE,
	FILE_ITEM_UPLOAD_CHAT_OFFLINE_FILE,
	FILE_ITEM_UPLOAD_USER_THUMB,

	FILE_ITEM_DOWNLOAD_CHAT_IMAGE,
	FILE_ITEM_DOWNLOAD_CHAT_OFFLINE_FILE,
	FILE_ITEM_DOWNLOAD_USER_THUMB,
};

//�ļ����ط��ؽ����
enum FILE_DOWNLOAD_RETCODE
{
	FILE_DOWNLOAD_FAILED,
	FILE_DOWNLOAD_SUCCESS,
	FILE_DOWNLOAD_USERCANCEL	//�û�ȡ������
};

//�ļ��ϴ����ؽ����
enum FILE_UPLOAD_RETCODE
{
	FILE_UPLOAD_FAILED,
	FILE_UPLOAD_SUCCESS,
	FILE_UPLOAD_USERCANCEL		//�û�ȡ���ϴ�
};

//��ȡ�ļ�md5ֵ�����
enum GET_FILE_MD5_RETCODE
{
	GET_FILE_MD5_FAILED,
	GET_FILE_MD5_SUCESS,
	GET_FILE_MD5_USERCANCEL
};

//�޸ĸ�����Ϣ����
enum MODIFY_USER_INFO_TYPE
{
	MODIFY_NICKNAME = 0x0001,
	MODIFY_GENDER   = 0x0002,
	MODIFY_BIRTHDAY = 0x0004,
	MODIFY_SIGNATRUE= 0x0008,
	MODIFY_ADDRESS  = 0x0010,
	MODIFY_PHONE    = 0x0020,
	MODIFY_EMAIL    = 0x0040,
	MODIFY_THUMB    = 0x0080
};

enum CHAT_CONFIRM_TYPE
{
	CHAT_CONFIRM_TYPE_SINGLE,		//���ģ�����Ⱥ�飩ȷ����Ϣ����
	CHAT_CONFIRM_TYPE_MULTI			//Ⱥ��ȷ����Ϣ����
};

enum NET_DATA_TYPE
{
	NET_DATA_UNKNOWN,
	NET_DATA_REGISTER,
	NET_DATA_LOGIN,
	NET_DATA_USER_BASIC_INFO,
    NET_DATA_CHANGE_STATUS,
    NET_DATA_GROUP_BASIC_INFO,
	NET_DATA_USER_EXTEND_INFO,
	NET_DATA_FRIENDS_ID,
	NET_DATA_FRIENDS_STATUS,
	NET_DATA_CHAT_MSG,
	NET_DATA_CHAT_CONFIRM_IMAGE_MSG,	//���췢����ͼƬ��׷�ӵ�ȷ����Ϣ
	NET_DATA_FIND_FRIEND,
	NET_DATA_OPERATE_FRIEND,
	NET_DATA_HEARTBEAT,
	NET_DATA_UPDATE_LOGON_USER_INFO,
	NET_DATA_TARGET_INFO_CHANGE,
	NET_DATA_MODIFY_PASSWORD,
	NET_DATA_CREATE_NEW_GROUP,
    NET_DATA_OPERATE_TEAM,              //����µĺ��ѷ���
    NET_DATA_MODIFY_FRIEND_MARKNAME,    //�޸ĺ��ѱ�ע��
    NET_DATA_MOVE_FRIEND,               //�ƶ���������������

	NET_DATA_FILE
};

class CNetData
{
public:
	CNetData();
	virtual ~CNetData();
public:
	UINT		m_uType;		//��������
	long		m_nRetryTimes;	//����Ϊ����������ʱ���Դ�����
};

class CRegisterRequest : public CNetData
{
public:
	CRegisterRequest();
	~CRegisterRequest();

public:
	char m_szAccountName[64];
	char m_szNickName[64];
	char m_szPassword[64];
	
};

class CRegisterResult : public CNetData
{
public:
	CRegisterResult();
	~CRegisterResult();

public:
	long m_nResultCode;
	char m_szMobile[64];
	char m_szAccount[64];
};

class CLoginRequest : public CNetData
{
public:
	CLoginRequest();
	~CLoginRequest();
public:
	char		m_szAccountName[64];
	char		m_szPassword[64];
	long		m_nStatus;
	LOGIN_TYPE	m_nLoginType;	//��¼���ͣ�0��ʾʹ���ֻ��ŵ��룬1��ʾʹ���˺ŵ���
};

class CLoginResult : public CNetData
{
public:
	CLoginResult();
	~CLoginResult();
public:
	long m_LoginResultCode;
	UINT m_uAccountID;
	char m_szAccountName[32];
	char m_szNickName[32];
	char m_szSignature[256];
	long m_nFace;
    char m_szCustomFace[64];
    int  m_nGender;
    int  m_nBirthday;
    char m_szAddress[512];
    char m_szPhoneNumber[64];
    char m_szMail[256];
	long m_nStatus;
};

class CUserBasicInfoRequest : public CNetData
{
public:
	CUserBasicInfoRequest();
	~CUserBasicInfoRequest();

public:
	std::set<UINT> m_setAccountID;
};

class CUserBasicInfoResult : public CNetData
{
public:
	CUserBasicInfoResult();
	~CUserBasicInfoResult();

public:
    //key�Ƿ�������֣�value�Ǹ�����ѵļ���
	std::map<std::string, std::list<UserBasicInfo*>> m_mapUserBasicInfo;
};

class CChangeUserStatusRequest : public CNetData
{
public:
    CChangeUserStatusRequest();
    ~CChangeUserStatusRequest();

public:
    int32_t m_nNewStatus;
};

class CGroupBasicInfoRequest : public CNetData
{
public:
    CGroupBasicInfoRequest();
    ~CGroupBasicInfoRequest();

public:
    int  m_groupid;
};

class CGroupBasicInfoResult : public CNetData
{
public:
    CGroupBasicInfoResult();
    ~CGroupBasicInfoResult();

public:
    int                       m_groupid;
    std::list<UserBasicInfo*> m_listUserBasicInfo;
};

class CUserExtendInfoRequest : public CNetData
{
public:
	CUserExtendInfoRequest();
	~CUserExtendInfoRequest();

public:
	std::set<UINT> m_setAccountID;
};

class CUserExtendInfoResult : public CNetData
{
public:
	CUserExtendInfoResult();
	~CUserExtendInfoResult();
public:
	std::list<UserExtendInfo*> m_listUserExtendInfo;
};

class CLoginUserFriendsIDRequest : public CNetData
{
public:
	CLoginUserFriendsIDRequest();
	~CLoginUserFriendsIDRequest();

public:
	UINT m_uAccountID;
};

class CLoginUserFriendsIDResult : public CNetData
{
public:
	CLoginUserFriendsIDResult();
	~CLoginUserFriendsIDResult();

public:
	UINT				m_uAccountID;
	std::set<UINT>		m_setFriendIDs;
	std::set<UINT>		m_setGroupIDs;
};

class CFriendStatus : public CNetData
{
public:
	CFriendStatus();
	~CFriendStatus();

public:
	UINT	m_uAccountID;
	long	m_nStatus;
    long    m_nClientType;
    int     m_type;
};

class CFindFriendRequest : public CNetData
{
public:
	CFindFriendRequest();
	~CFindFriendRequest();

public:
	char m_szAccountName[64];
	long m_nType;				//��������
};

class CFindFriendResult : public CNetData
{
public:
	CFindFriendResult();
	~CFindFriendResult();

public:
	long m_nResultCode;
	UINT m_uAccountID;
	char m_szAccountName[64];
	char m_szNickName[64];
};

class COperateFriendRequest : public CNetData
{
public:
	COperateFriendRequest();
	~COperateFriendRequest();

public:
	UINT m_uAccountID;
	UINT m_uCmd;
};

class COperateFriendResult : public CNetData
{
public:
	COperateFriendResult();
	~COperateFriendResult();

public:
	UINT m_uAccountID;
	UINT m_uCmd;
	char m_szAccountName[64];
	char m_szNickName[64];
};

class CAddTeamInfoRequest : public CNetData
{
public:
    CAddTeamInfoRequest();
    ~CAddTeamInfoRequest();

public:
    int          m_opType;          //��������
    std::wstring m_strNewTeamName;
    std::wstring m_strOldTeamName;  
};

class CAddTeamInfoResult : public CNetData
{
public:
    CAddTeamInfoResult();
    ~CAddTeamInfoResult();

public:
    int          m_opType;          //��������  
};

class CMoveFriendRequest : public CNetData
{
public:
    CMoveFriendRequest();
    ~CMoveFriendRequest();

public:
    int          m_nFriendID;          //��������
    std::wstring m_strNewTeamName;
    std::wstring m_strOldTeamName;
};

class CMsgItem;

class CSentChatMessage : public CNetData
{
public:
	CSentChatMessage();
	~CSentChatMessage();

public:
	CMsgItem* m_pMsgItem;
	HWND	  m_hwndChat;
};

class CSentChatConfirmImageMessage : public CNetData
{
public:
	CSentChatConfirmImageMessage();
	~CSentChatConfirmImageMessage();

public:
	CHAT_CONFIRM_TYPE	m_nType;
	HWND				m_hwndChat;
	char*				m_pszConfirmBody;
	UINT				m_uConfirmBodySize;		//��Ϣ��ĳ��ȣ��������ĩβ������\0
	UINT				m_uSenderID;
	std::set<UINT>		m_setTargetIDs;
	//UINT	  m_uTargetID;

};

class CRecvChatMessage : public CNetData
{
public:
	CRecvChatMessage();
	~CRecvChatMessage();

public:
	UINT  m_uMsgID;			//��ϢID
	UINT  m_uSenderID;		//������ID
	UINT  m_uTargetID;		//������ID
	UINT  m_uMsgBodySize;	//��Ϣ�������ݳ���
	char* m_pszMsgBody;		//��Ϣ��������
};

class CModifyPasswordRequest : public CNetData
{
public:
	CModifyPasswordRequest();
	~CModifyPasswordRequest();

public:
	char m_szOldPassword[64];
	char m_szNewPassword[64];
};

class CModifyPasswordResult : public CNetData
{
public:
	CModifyPasswordResult();
	~CModifyPasswordResult();

public:
	long	m_nResultCode;
};

//class CCreateNewGroupRequest
class CCreateNewGroupRequest : public CNetData
{
public:
	CCreateNewGroupRequest();
	~CCreateNewGroupRequest();

public:
	char m_szGroupName[64];
};

//class CCreateGroupResult
class CCreateNewGroupResult : public CNetData
{
public:
	CCreateNewGroupResult();
	~CCreateNewGroupResult();

public:
	UINT	m_uError;
	UINT	m_uAccountID;
	char	m_szGroupName[64];
	char	m_szAccount[32];
};

//class CModifyFriendMakeNameRequest
class CModifyFriendMakeNameRequest : public CNetData
{
public:
    CModifyFriendMakeNameRequest();
    ~CModifyFriendMakeNameRequest();

public:
    UINT	m_uFriendID;
    TCHAR   m_szNewMarkName[64];
};

//class CModifyFriendMakeNameResult
class CModifyFriendMakeNameResult : public CNetData
{
public:
    CModifyFriendMakeNameResult();
    ~CModifyFriendMakeNameResult();

public:
    UINT	m_uFriendID;
    //char	m_szNewMarkName[64];
};

class CHeartbeatMessageRequest : public CNetData
{
public:
	CHeartbeatMessageRequest();
	~CHeartbeatMessageRequest();
};

class CHeartbeatMessageResult : public CNetData
{
public:
	~CHeartbeatMessageResult();
	CHeartbeatMessageResult();
};

//class CUpdateLogonUserInfoRequest
class CUpdateLogonUserInfoRequest : public CNetData
{
public:
	CUpdateLogonUserInfoRequest();
	~CUpdateLogonUserInfoRequest();

public:
	char	m_szNickName[64];
	char    m_szSignature[512];
	UINT	m_uGender;
	long	m_nBirthday;
	char    m_szAddress[64];
	char    m_szPhone[32];
	char    m_szMail[48];
	UINT	m_uFaceID;
	TCHAR   m_szCustomFace[MAX_PATH];
	BOOL    m_bUseCustomThumb;				//�Ƿ�ʹ���Զ���ͷ��
};

//class CUpdateLogonUserInfoResult
class CUpdateLogonUserInfoResult : public CNetData
{
public:
	CUpdateLogonUserInfoResult();
	~CUpdateLogonUserInfoResult();

public:
    char	m_szNickName[64];
    char    m_szSignature[512];
    UINT	m_uGender;
    long	m_nBirthday;
    char    m_szAddress[64];
    char    m_szPhone[32];
    char    m_szMail[48];
    UINT	m_uFaceID;
    char    m_szCustomFace[MAX_PATH];
    BOOL    m_bUseCustomThumb;
};

//class CTargetInfoChangeResult
class CTargetInfoChangeResult : public CNetData
{
public:
	CTargetInfoChangeResult();
	~CTargetInfoChangeResult();

public:
	UINT m_uAccountID;

};

class CScreenshotInfo
{
public:
    CScreenshotInfo();
    ~CScreenshotInfo();

public:
    std::string m_strBmpHeader;
    std::string m_strBmpData;
    UINT        m_targetId;
};


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// �ļ���������
/////////////////////////////////////////////////////
//class CBuddyMessage;
//class CFileItemRequest : public CNetData
//{
//public:
//	CFileItemRequest();
//	~CFileItemRequest();
//
//public:
//	long			m_nID;
//	char			m_szUtfFilePath[MAX_PATH];	//�ļ�utf8·�����������أ�����ʱ���
//	TCHAR			m_szFilePath[MAX_PATH];		//�ļ�ȫ��·���������ϴ����ϴ�ʱ����, ����ʱ��ΪĿ���ļ�·����
//	HWND			m_hwndReflection;			//							 (�Ǳ����ֶ�)
//	HANDLE			m_hCancelEvent;				// ȡ���¼�					 (�Ǳ����ֶ�)
//	UINT			m_uSenderID;
//	std::set<UINT>	m_setTargetIDs;
//	UINT			m_uAccountID;				//�˻�id����������ͷ��
//	long			m_nFileType;				//Ŀǰ������ͼƬ�������ļ����Զ���ͷ����������
//	CBuddyMessage*  m_pBuddyMsg;
//
//	BOOL			m_bPending;					//�������Ѿ������ػ����ϴ�ΪFALSE����֮ΪTRUE
//};
//
//class CUploadFileResult
//{
//public:
//	CUploadFileResult();
//	~CUploadFileResult();
//
//public:
//	void Clone(const CUploadFileResult* pSource);
//
//public:
//	long			m_nFileType;
//	BOOL			m_bSuccessful;					//�Ƿ��ϴ��ɹ�
//	int64_t			m_nFileSize;					//�ļ���С
//	TCHAR			m_szLocalName[MAX_PATH];		//�����ļ���
//						
//	char			m_szMd5[64];					//�ļ���md5ֵ
//	char			m_szRemoteName[MAX_PATH];		//�ϴ��ɹ��Ժ��ļ��ڷ������ϵ�·����
//
//	UINT			m_uSenderID;
//	std::set<UINT>	m_setTargetIDs;
//	HWND			m_hwndReflection;
//};
//
//struct FileProgress
//{
//	TCHAR szDestPath[MAX_PATH];
//	long  nPercent;				//�ٷֱȣ�0��100֮��(ֵΪ-1ʱ��Ϊ��ȡ�ļ�md5ֵ)
//	long  nVerificationPercent;	//��ȡmd5ֵ���ȣ�0��100��
//};






