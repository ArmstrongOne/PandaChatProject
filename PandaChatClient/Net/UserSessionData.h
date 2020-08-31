#pragma once
#include"..\jsoncpp1.9.0\json.h"
#include <string>
#include <vector>

#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring tstring;
#else
typedef std::string tstring;
#endif

extern TCHAR g_szHomePath[MAX_PATH];
extern char g_szHomePathAscii[MAX_PATH];
extern const UINT USER_THUMB_COUNT;

extern std::wstring g_strAppTitle;

#define     FMG_MSG_NET_ERROR               WM_USER + 50            // �������
#define		FMG_MSG_LOGIN_RESULT			WM_USER + 100		    // ��¼������Ϣ
#define		FMG_MSG_LOGOUT_RESULT			WM_USER + 101			// ע��������Ϣ
#define		FMG_MSG_UPDATE_BUDDY_LIST		WM_USER + 103			// ���º����б���Ϣ
#define		FMG_MSG_UPDATE_GROUP_LIST		WM_USER + 104			// ����Ⱥ�б���Ϣ
#define		FMG_MSG_UPDATE_RECENT_LIST		WM_USER + 105		    // ���������ϵ���б���Ϣ
#define		FMG_MSG_BUDDY_MSG				WM_USER + 106			// ������Ϣ
#define		FMG_MSG_GROUP_MSG				WM_USER + 107			// Ⱥ��Ϣ
#define		FMG_MSG_SESS_MSG				WM_USER + 108		    // ��ʱ�Ự��Ϣ
#define		FMG_MSG_STATUS_CHANGE_MSG		WM_USER + 109			// ����״̬�ı���Ϣ
#define		FMG_MSG_KICK_MSG				WM_USER + 110		    // ����������Ϣ
#define		FMG_MSG_SYS_GROUP_MSG			WM_USER + 111			// Ⱥϵͳ��Ϣ
#define		FMG_MSG_UPDATE_BUDDY_NUMBER		WM_USER + 112		    // ���º��Ѻ���
#define		FMG_MSG_UPDATE_GMEMBER_NUMBER	WM_USER + 113			// ����Ⱥ��Ա����
#define		FMG_MSG_UPDATE_GROUP_NUMBER		WM_USER + 114		    // ����Ⱥ����
#define		FMG_MSG_UPDATE_BUDDY_SIGN		WM_USER + 115			// ���º��Ѹ���ǩ��
#define		FMG_MSG_UPDATE_GMEMBER_SIGN		WM_USER + 116		    // ����Ⱥ��Ա����ǩ��
#define		FMG_MSG_UPDATE_BUDDY_INFO		WM_USER + 117			// ���º�����Ϣ
#define		FMG_MSG_UPDATE_GMEMBER_INFO		WM_USER + 118		    // ����Ⱥ��Ա��Ϣ
#define		FMG_MSG_UPDATE_GROUP_INFO		WM_USER + 119			// ����Ⱥ��Ϣ
#define		FMG_MSG_UPDATE_C2CMSGSIG		WM_USER + 120		    // ������ʱ�Ự����
#define		FMG_MSG_UPDATE_GROUPFACESIG		WM_USER + 121		    // ����Ⱥ��������
#define		FMG_MSG_UPDATE_BUDDY_HEADPIC	WM_USER + 122		    // ���º���ͷ��
#define		FMG_MSG_UPDATE_GMEMBER_HEADPIC	WM_USER + 123		    // ����Ⱥ��Աͷ��
#define		FMG_MSG_UPDATE_GROUP_HEADPIC	WM_USER + 124		    // ����Ⱥͷ��
#define		FMG_MSG_CHANGE_STATUS_RESULT	WM_USER + 125		    // �ı�����״̬������Ϣ

#define		FMG_MSG_INTERNAL_GETBUDDYDATA	WM_USER + 400
#define		FMG_MSG_INTERNAL_GETGROUPDATA	WM_USER + 401
#define		FMG_MSG_INTERNAL_GETGMEMBERDATA	WM_USER + 402
#define		FMG_MSG_INTERNAL_GROUPID2CODE	WM_USER + 403

#define		FMG_MSG_REGISTER				WM_USER + 404			//ע�����û�
#define		FMG_MSG_FINDFREIND			    WM_USER + 405
#define     FMG_MSG_ADDFREIND				WM_USER + 406			//�������ͼӺ�������
#define		FMG_MSG_RECVADDFRIENDREQUSET	WM_USER + 407			//�յ��Ӻ�������
#define		FMG_MSG_DELETEFRIEND			WM_USER + 408
#define		FMG_MSG_SENDCHATMSG_RESULT	    WM_USER + 409			//�����ѷ���Ϣ�ķ������
//#define		FMG_MSG_UPLOADFILE_RESULT		WM_USER + 410		//�ϴ��ļ��������
#define		FMG_MSG_RECVFILE_REQUEST		WM_USER + 411			//�����ļ�����
#define		FMG_MSG_RECVFILE_RESULT		    WM_USER + 412			//�����ļ��������
#define     FMG_MSG_RECVADDFRIENDANS		WM_USER + 413			//�յ�������Ӻ��ѵĶԷ�����(�ܾ�����ͬ��)
#define		FMG_MSG_HEARTBEAT				WM_USER + 414			//������Ϣ
#define		FMG_MSG_GOOFFLINE				WM_USER + 415			//���߻��ߵ���
#define		FMG_MSG_UPDATE_USER_BASIC_INFO  WM_USER + 416			//����ĳ���û�������Ϣ
#define		FMG_MSG_UPDATE_GROUP_BASIC_INFO WM_USER + 417		    //����ĳ��Ⱥ�������Ϣ

#define		FMG_MSG_UPDATE_FRIEND_IDS		WM_USER + 418			//�������к���ID
#define		FMG_MSG_MODIFY_USER_INFO		WM_USER + 421			//�޸��û���Ϣ�ɹ�
#define		FMG_MSG_RECV_USER_STATUS_CHANGE_DATA	WM_USER + 422	//�յ��û�����״̬�ı�����
#define		FMG_MSG_USER_STATUS_CHANGE	    WM_USER + 423			//�û�����״̬�ı�
#define		FMG_MSG_SELF_STATUS_CHANGE	    WM_USER + 424			//�Լ���������ն�������֪ͨ
#define     FMG_MSG_UPDATE_USER_CHAT_MSG_ID	WM_USER + 425		    //�����û���������ϢID
#define		FMG_MSG_RECV_CHAT_MSG			WM_USER + 426			//�յ�������Ϣ
#define		FMG_MSG_UPDATE_CHATDLG_USERINFO WM_USER + 427			//�������촰�����û�״̬
#define		FMG_MSG_SEND_FILE_PROGRESS	    WM_USER + 428			//�ļ����ͽ�����Ϣ
#define		FMG_MSG_RECV_FILE_PROGRESS	    WM_USER + 429			//�����ļ�������Ϣ
#define		FMG_MSG_SEND_FILE_RESULT		WM_USER + 430			//�����ļ����
#define		FMG_MSG_RECV_FILE_RESULT		WM_USER + 431			//�����ļ����
#define		FMG_MSG_NETWORK_STATUS_CHANGE   WM_USER + 432			//��������״̬���ͱ仯
#define		FMG_MSG_CUSTOMFACE_AVAILABLE	WM_USER + 433			//�Զ���ͷ���Ϊ����
#define		FMG_MSG_UPLOAD_USER_THUMB		WM_USER + 434			//�Զ���ͷ����Ϣ
#define		FMG_MSG_TARGET_INFO_CHANGE	    WM_USER + 435			//���������߿ͻ����û���Ϣ�����仯����Ҫ�û�ȥȡ�µ��û���Ϣ
#define		FMG_MSG_MODIFY_PASSWORD_RESULT  WM_USER + 436			//�޸�������
#define		FMG_MSG_CREATE_NEW_GROUP_RESULT WM_USER + 437			//����Ⱥ����
#define     FMG_MSG_SCREENSHOT              WM_USER + 438			//��Ļ��ͼ��Ϣ


#define		FMG_MSG_FIRST					WM_USER + 001
#define		FMG_MSG_LAST					WM_USER + 500

// ��Ϣ�����ַ���
#define		FMG_MSG_TYPE_STR_BUDDY			_T("message")		        // ������Ϣ����
#define		FMG_MSG_TYPE_STR_GROUP			_T("group_message")	        // Ⱥ��Ϣ����
#define		FMG_MSG_TYPE_STR_SESS			_T("sess_message")		    // ��ʱ�Ự��Ϣ����
#define		FMG_MSG_TYPE_STR_STATUSCHANGE	_T("buddies_status_change")	// ����״̬�ı���Ϣ����
#define		FMG_MSG_TYPE_STR_KICK			_T("kick_message")		    // ����������Ϣ����
#define		FMG_MSG_TYPE_STR_SYSGROUP		_T("sys_g_msg")			    // Ⱥϵͳ��Ϣ����

//typedef NS_ENUM(int, IUKMessageInsideType) {// ������Ϣ����
//    IUKMessageInsideTypeUndefined   = 0,// δ֪��Ϣ����
//    IUKMessageInsideTypeText        = 1,// �ı�����
//    IUKMessageInsideTypeImage       = 2,// ͼƬ����  pcר�� ��pc���ϴ�ͼƬ�ɹ��� ��������Ϣ ����ȥ����  pc��app�˶�Ҫ����
//    IUKMessageInsideTypeFile        = 3,// �ļ�����
//    IUKMessageInsideTypeShake       = 4,// ���ڶ���  pcר��
//    IUKMessageinsideTypeApp         = 5 // app�˷�����ͼƬ  ��app�˷��͵�ͼƬ��Ϣ���Ѿ��ϴ��òŻᷢ�͵���һ��  pc�ӵ�������Ϳ���ֱ����ʾ��
//};

enum CONTENT_TYPE
{
    CONTENT_TYPE_UNKNOWN,			    // δ֪��Ϣ����
    CONTENT_TYPE_TEXT,				    // �ı����ͣ�����ͼ�Ļ������ͣ�
    CONTENT_TYPE_IMAGE_CONFIRM,			// ͼƬ�ϴ��ɹ����ȷ����Ϣ����  pcר�� ��pc���ϴ�ͼƬ�ɹ��� ��������Ϣ ����ȥ����  pc��app�˶�Ҫ����
    CONTENT_TYPE_FILE,				    // �յ����ѷ��͹������ļ�
    CONTENT_TYPE_SHAKE_WINDOW,			// ���ڶ���  pcר��
    CONTENT_TYPE_MOBILE_IMAGE,			// app�˷�����ͼƬ  ��app�˷��͵�ͼƬ��Ϣ���Ѿ��ϴ��òŻᷢ�͵���һ��  pc�ӵ�������Ϳ���ֱ����ʾ��

    CONTENT_TYPE_FONT_INFO,				//pcר�ã��ƶ��˺���
    CONTENT_TYPE_FACE,					//pcר�ã��ƶ��˺���
    CONTENT_TYPE_CHAT_IMAGE,			//pcר�ã��ƶ��˺���

    CONTENT_TYPE_REMOTE_DESKTOP         //Զ������
};

class CBuddyTeamInfo;
class CBuddyInfo;
class COnlineBuddyInfo;
class CGroupInfo;
class CRecentInfo;
class CBuffer;

struct BuddyInfo
{
    UINT	uAccountID;
    TCHAR	szAccountName[32];
    TCHAR	szMarkName[32];
    long	nTeamIndex;
    long	nGroupCount;		//Ⱥ����
    UINT	GroupID[64];		//Ⱥ��ID
};

class CBuddyInfo			// ������Ϣ
{
public:
    CBuddyInfo(void);
    ~CBuddyInfo(void);

public:
    void Reset();
    void FillFakeData();	//TODO: ���������ݣ���Ϊ�˲��ԣ�ʵ�ʵ�����Ӧ�õȷ������������Ժ�ӷ�������ȡ
    void SetBuddyInfo(CBuddyInfo* lpBuddyInfo);

public:
    UINT		m_uUserID;			//�û�ID��Ψһ��ʶ���û�
    tstring		m_strAccount;		//�û��˻���
    tstring		m_strPassword;		// ����
    tstring		m_strNickName;		// �ǳ�
    tstring		m_strMarkName;		// ��ע
    long		m_nTeamIndex;		// ��������
    UINT		m_nFace;			// ͷ��
    long		m_nStatus;			// ����״̬
    long        m_nClientType;      // �ͻ�������
    tstring		m_strSign;			// ����ǩ��
    long		m_nGender;			// �Ա�
    long		m_nBirthday;		// ����
    tstring		m_strMobile;		// �ֻ�
    tstring		m_strAddress;		// �û���ַ��Ϣ
    tstring		m_strEmail;
    BOOL		m_bUseCustomFace;		//�Ƿ�ʹ���Զ���ͷ��
    BOOL		m_bCustomFaceAvailable;	//�Զ���ͷ���Ƿ����
    tstring		m_strCustomFace;	    // �Զ���ͷ�������ļ�������·�������ز�������Ҫ�ӷ��������أ�
    tstring		m_strRawCustomFace;		//ԭʼ�Զ���ͷ������

    //CStringA    m_strCustomFaceName;

    UINT		m_uMsgID;			//�Ự��ϢID����ʼֵΪ0��ֻ�������ڴ��в��浵
};

struct BuddyTeamInfo
{
    long	nTeamIndex;
    TCHAR	szTeamName[16];
};

class CBuddyTeamInfo		// ���ѷ�����Ϣ
{
public:
    CBuddyTeamInfo(void);
    ~CBuddyTeamInfo(void);

public:
    void Reset();
    int GetBuddyCount();
    int GetOnlineBuddyCount();
    CBuddyInfo* GetBuddy(int nIndex);
    void Sort();

public:
    int m_nIndex;			// ����
    int m_nSort;			// ����˳��
    tstring m_strName;		// ��������
    std::vector<CBuddyInfo*> m_arrBuddyInfo;
};

class COnlineBuddyInfo		// ���ߺ�����Ϣ
{
public:
    COnlineBuddyInfo(void);
    ~COnlineBuddyInfo(void);

public:
    void Reset();

public:
    UINT m_nUin;
    long m_nStatus;	// ����״̬
    int m_nClientType;		// �ͻ�������
};

class CBuddyList	// �����б�
{
public:
    CBuddyList(void);
    ~CBuddyList(void);

public:
    void Reset();
    int GetBuddyTeamCount();
    CBuddyTeamInfo* GetBuddyTeam(int nTeamIndex);
    CBuddyTeamInfo* GetBuddyTeamByIndex(int nIndex);
    int GetBuddyTotalCount();
    int GetBuddyCount(int nTeamIndex);
    int GetOnlineBuddyCount(int nTeamIndex);
    CBuddyInfo* GetBuddy(int nTeamIndex, int nIndex);
    CBuddyInfo* GetBuddy(UINT nUTalkUin);
    void SortBuddyTeam();
    void SortBuddy();
    BOOL AddBuddyTeam(CBuddyTeamInfo* lpBuddyTeamInfo);
    BOOL IsTeamNameExist(PCTSTR pszTeamName);

public:
    int m_nRetCode;
    std::vector<CBuddyTeamInfo*> m_arrBuddyTeamInfo;
};

class CGroupInfo			// Ⱥ��Ϣ
{
public:
    CGroupInfo(void);
    ~CGroupInfo(void);

public:
    void Reset();
    int GetMemberCount();						// ��ȡȺ��Ա������
    int GetOnlineMemberCount();					// ��ȡȺ��Ա��������
    CBuddyInfo* GetMember(int nIndex);			// ����������ȡȺ��Ա��Ϣ
    CBuddyInfo* GetMemberByUin(UINT nUTalkUin);	// �����ڲ�ID��ȡȺ��Ա��Ϣ
    CBuddyInfo* GetMemberByAccount(PCTSTR pszAccountName);	// �����˺Ż�ȡȺ��Ա��Ϣ
    void Sort();								// ��Ⱥ��Ա�б�����״̬��������
    BOOL AddMember(CBuddyInfo* lpBuddyInfo);	// ���Ⱥ��Ա
    BOOL DelAllMember();						// ɾ������Ⱥ��Ա
    BOOL IsHasGroupNumber();
    BOOL IsHasGroupInfo();
    BOOL IsMember(UINT uAccountID);

public:
    UINT m_nGroupCode;		// Ⱥ�˺�ID
    UINT m_nGroupId;		// ȺID
    UINT m_nGroupNumber;	// Ⱥ����
    tstring m_strAccount;	// Ⱥ�˺�
    tstring m_strName;		// Ⱥ����
    tstring m_strMemo;		// Ⱥ����
    tstring m_strFingerMemo;// Ⱥ���
    UINT m_nOwnerUin;		// Ⱥӵ����Uin
    UINT m_nCreateTime;		// Ⱥ����ʱ��
    int m_nFace;			// Ⱥͷ��
    int m_nLevel;			// Ⱥ�ȼ�
    int m_nClass;			// Ⱥ��������
    UINT m_nOption;
    UINT m_nFlag;
    BOOL m_bHasGroupNumber;
    BOOL m_bHasGroupInfo;
    std::vector<CBuddyInfo*> m_arrMember;	// Ⱥ��Ա
};

class CGroupList	// Ⱥ�б�
{
public:
    CGroupList(void);
    ~CGroupList(void);

public:
    void Reset();
    int GetGroupCount();							// ��ȡȺ����
    CGroupInfo* GetGroup(int nIndex);				// ��ȡȺ��Ϣ(��������)
    CGroupInfo* GetGroupByCode(UINT nGroupCode);	// ��ȡȺ��Ϣ(����Ⱥ����)
    CGroupInfo* GetGroupById(UINT nGroupId);		// ��ȡȺ��Ϣ(����ȺId)
    CBuddyInfo* GetGroupMemberByCode(UINT nGroupCode, UINT nUTalkUin);// ����Ⱥ�����Ⱥ��ԱUTalkUin��ȡȺ��Ա��Ϣ
    CBuddyInfo* GetGroupMemberById(UINT nGroupId, UINT nUTalkUin);	// ����ȺId��Ⱥ��ԱUTalkUin��ȡȺ��Ա��Ϣ
    BOOL AddGroup(CGroupInfo* lpGroupInfo);		// ���Ⱥ
    UINT GetGroupCodeById(UINT nGroupId);			// ��ȺId��ȡȺ����
    UINT GetGroupIdByCode(UINT nGroupCode);			// ��Ⱥ�����ȡȺId

public:
    std::vector<CGroupInfo*> m_arrGroupInfo;
};

#define MAX_LAST_MSG_TEXT_LENGTH 18
#define MAX_RECENT_NAME_LENGTH	 32

struct RecentInfo
{
    int		nType;		//��Ϣ���ͣ�����������Ϣ��Ⱥ�顢�����飩
    UINT	uUserID;
    UINT	uFaceID;	//ͷ������
    UINT64  uMsgTime;
    TCHAR   szName[MAX_RECENT_NAME_LENGTH];
    TCHAR   szLastMsgText[MAX_LAST_MSG_TEXT_LENGTH];
};

class CRecentInfo	// �����ϵ����Ϣ
{
public:
    CRecentInfo(void);
    ~CRecentInfo(void);

public:
    void Reset();

public:
    int		m_nType;		//��Ϣ���ͣ�����������Ϣ��Ⱥ�顢�����飩
    UINT	m_uUserID;
    UINT	m_uFaceID;		//ͷ������
    UINT64  m_MsgTime;
    TCHAR   m_szName[MAX_RECENT_NAME_LENGTH];	//�����ǳƻ�Ⱥ����
    TCHAR	m_szLastMsgText[MAX_LAST_MSG_TEXT_LENGTH];

};

class CRecentList			// �����ϵ���б�
{
public:
    CRecentList(void);
    ~CRecentList(void);

public:
    void Reset();
    BOOL DeleteRecentItem(UINT uAccountID);
    BOOL AddRecent(CRecentInfo* lpRecentInfo);
    int GetRecentCount();
    CRecentInfo* GetRecent(int nIndex);

public:
    int m_nRetCode;
    std::vector<CRecentInfo*> m_arrRecentInfo;
};

class CFontInfo				// ������Ϣ
{
public:
    CFontInfo(void) : m_nSize(9), m_clrText(RGB(0, 0, 0)), m_strName(_T("΢���ź�")),
        m_bBold(FALSE), m_bItalic(FALSE), m_bUnderLine(FALSE){}
    ~CFontInfo(void){}

public:
    int m_nSize;			// �����С
    COLORREF m_clrText;		// ������ɫ
    tstring m_strName;		// ��������
    BOOL m_bBold;			// �Ƿ�Ӵ�
    BOOL m_bItalic;			// �Ƿ���б
    BOOL m_bUnderLine;		// �Ƿ���»���
};

class CCustomFaceInfo		// �Զ��������Ϣ
{
public:
    CCustomFaceInfo(void) : m_nFileId(0), m_dwFileSize(0), m_bOnline(TRUE){}
    ~CCustomFaceInfo(void){}

public:
    tstring m_strName;			// �����Զ������ʹ�ò���(TODO: ����ֶ��ȱ�������Ҫ�ã������Ժ���չ)
    UINT    m_nFileId;
    tstring m_strKey;
    tstring m_strServer;

    DWORD   m_dwFileSize;		// �����Զ������ʹ�ò���
    tstring m_strFileName;
    tstring m_strFilePath;
    BOOL	m_bOnline;			//�����ļ�����ʱ�����ֶδ����ļ��������ļ����������ļ�
};

class CContent				// ��Ϣ����
{
public:
    CContent(void) : m_nType(CONTENT_TYPE_UNKNOWN), m_nFaceId(0), m_nShakeTimes(0){}
    ~CContent(void){}

public:
    CONTENT_TYPE	m_nType;			// ��������
    CFontInfo		m_FontInfo;			// ������Ϣ
    tstring			m_strText;			// �ı���Ϣ
    int				m_nFaceId;			// ϵͳ����Id
    int				m_nShakeTimes;		// ���ڶ�������
    CCustomFaceInfo m_CFaceInfo;		// �Զ��������Ϣ�������͵���m_nType��m_CFaceInfo���ļ����й���Ϣ��
};


class CBuddyMessage
{
public:
    CBuddyMessage(void);
    ~CBuddyMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void		Reset();
    BOOL		Parse(Json::Value& JsonValue);
    BOOL		IsShakeWindowMsg();

public:
    CONTENT_TYPE			m_nMsgType;
    UINT					m_nMsgId;
    UINT					m_nMsgId2;
    UINT					m_nFromUin;
    UINT					m_nToUin;
    UINT					m_nReplyIp;
    UINT64					m_nTime;
    HWND					m_hwndFrom;
    tstring					m_strNickName;					//�����ֶ�
    std::vector<CContent*> m_arrContent;
};

class CGroupMessage
{
public:
    CGroupMessage(void);
    ~CGroupMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT	m_nMsgId;
    UINT	m_nMsgId2;
    UINT	m_nFromUin;
    UINT	m_nToUin;
    UINT	m_nMsgType;
    UINT	m_nReplyIp;
    UINT	m_nGroupCode;
    UINT	m_nSendUin;
    UINT	m_nSeq;
    UINT	m_nInfoSeq;
    UINT64	m_nTime;
    HWND	m_hwndFrom;
    std::vector<CContent*> m_arrContent;
};

class CSessMessage		// ��ʱ��Ϣ
{
public:
    CSessMessage(void);
    ~CSessMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT m_nMsgId;
    UINT m_nMsgId2;
    UINT m_nFromUin;
    UINT m_nToUin;
    UINT m_nMsgType;
    UINT m_nReplyIp;
    UINT m_nTime;
    UINT m_nGroupId;
    UINT m_nUTalkNum;
    UINT m_nServiceType;
    UINT m_nFlags;
    std::vector<CContent*> m_arrContent;
};

class CStatusChangeMessage	// ״̬�ı�֪ͨ��Ϣ
{
public:
    CStatusChangeMessage(void);
    ~CStatusChangeMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT m_nUTalkUin;
    long m_nStatus;	// ����״̬
    int m_nClientType;		// �ͻ�������
};

class CKickMessage	// ��������֪ͨ��Ϣ(�����������һ�ص��¼)
{
public:
    CKickMessage(void);
    ~CKickMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT m_nMsgId;
    UINT m_nMsgId2;
    UINT m_nFromUin;
    UINT m_nToUin;
    UINT m_nMsgType;
    UINT m_nReplyIp;
    BOOL m_bShowReason;			// �Ƿ���ʾ��������ԭ��
    tstring m_strReason;		// ��������ԭ��
};

class CSysGroupMessage	// Ⱥϵͳ��Ϣ
{
public:
    CSysGroupMessage(void);
    ~CSysGroupMessage(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT m_nMsgId;
    UINT m_nMsgId2;
    UINT m_nFromUin;
    UINT m_nToUin;
    UINT m_nMsgType;
    UINT m_nReplyIp;
    tstring m_strSubType;
    UINT m_nGroupCode;
    UINT m_nGroupNumber;
    UINT m_nAdminUin;
    tstring m_strMsg;
    UINT m_nOpType;
    UINT m_nOldMember;
    tstring m_strOldMember;
    tstring m_strAdminUin;
    tstring m_strAdminNickName;
};

class CUpdateFriendInfoMsg	//���º�����Ϣ
{
public:
    CUpdateFriendInfoMsg(void);
    ~CUpdateFriendInfoMsg(void);

public:
    static BOOL IsType(LPCTSTR lpType);
    void Reset();
    BOOL Parse(Json::Value& JsonValue);

public:
    UINT m_uMsgType;
    UINT m_uMsgId;
    UINT m_uMsgId2;
    UINT m_uFromUin;
    UINT m_uToUin;
    UINT m_uClientType;
    UINT64 m_uTime;
};

class CMessageSender	// ��Ϣ������
{
public:
    CMessageSender(long nType, UINT nSenderId, UINT nGroupCode);
    ~CMessageSender(void);

public:
    BOOL AddMsg(void* lpMsg);						// �����Ϣ
    BOOL DelMsg(int nIndex);						// ɾ����Ϣ(��������)
    BOOL DelMsgById(UINT nMsgId);					// ɾ����Ϣ(������ϢId)
    BOOL DelAllMsg();								// ɾ��������Ϣ

    long GetMsgType();					// ��ȡ��Ϣ����
    UINT GetSenderId();								// ��ȡ������Id
    int GetMsgCount();								// ��ȡ��Ϣ����
    int GetDisplayMsgCount();						// ��ȥͼƬ�ϴ���ɵ�ȷ����Ϣ������
    UINT GetGroupCode();							// ��ȡȺ����

    CBuddyMessage* GetBuddyMsg(int nIndex);		// ��ȡ������Ϣ(��������)
    CBuddyMessage* GetGroupMsg(int nIndex);		// ��ȡȺ��Ϣ(��������)
    CSessMessage* GetSessMsg(int nIndex);			// ��ȡ��ʱ�Ự��Ϣ(��������)

    CBuddyMessage* GetBuddyMsgById(UINT nMsgId);	// ��ȡ������Ϣ(������ϢId)
    CBuddyMessage* GetGroupMsgById(UINT nMsgId);	// ��ȡȺ��Ϣ(������ϢId)
    CSessMessage* GetSessMsgById(UINT nMsgId);		// ��ȡ��ʱ�Ự��Ϣ(������ϢId)
    CSysGroupMessage* GetSysGroupMsgById(UINT nMsgId);// ��ȡȺϵͳ��Ϣ(������ϢId)

private:
    long m_nType;
    UINT m_nSenderId;
    UINT m_nGroupCode;
    std::vector<void*> m_arrMsg;
};

class CMessageList	// ��Ϣ�б�
{
public:
    CMessageList(void);
    ~CMessageList(void);

public:
    BOOL AddMsg(long nType, UINT nSenderId, UINT nGroupCode, void* lpMsg);		// �����Ϣ
    BOOL DelMsg(long nType, UINT nMsgId);										// ɾ����Ϣ

    BOOL DelMsgSender(int nIndex);												// ɾ����Ϣ������(��������)
    BOOL DelMsgSender(long nType, UINT nSenderId);								// ɾ����Ϣ������(������Ϣ���ͺͷ�����Id)
    BOOL DelAllMsgSender();									// ɾ��������Ϣ������

    int GetMsgSenderCount();								// ��ȡ��Ϣ����������
    CMessageSender* GetMsgSender(int nIndex);				// ��ȡ��Ϣ������(��������)
    CMessageSender* GetMsgSender(long nType, UINT nSenderId);// ��ȡ��Ϣ������(������Ϣ���ͺͷ�����Id)
    CMessageSender* GetLastMsgSender();					// ��ȡ���һ��������Ϣ�����ķ�����

    CBuddyMessage* GetBuddyMsg(UINT nMsgId);				// ��ȡ������Ϣ(������ϢId)
    CBuddyMessage* GetGroupMsg(UINT nMsgId);				// ��ȡȺ��Ϣ(������ϢId)
    CSessMessage* GetSessMsg(UINT nMsgId);					// ��ȡ��ʱ�Ự��Ϣ(������ϢId)

    CBuddyMessage* GetBuddyMsg(UINT nUTalkUin, UINT nMsgId);	// ��ȡ������Ϣ(���ݷ�����Id����ϢId)
    CBuddyMessage* GetGroupMsg(UINT nGroupCode, UINT nMsgId);	// ��ȡȺ��Ϣ(���ݷ�����Id����ϢId)
    CSessMessage* GetSessMsg(UINT nUTalkUin, UINT nMsgId);	// ��ȡ��ʱ�Ự��Ϣ(���ݷ�����Id����ϢId)

    struct MSG_IDENTITY
    {
        long nType;
        UINT nSenderId;
    };

private:
    void AddMsgIdentity(long nType, UINT nSenderId);
    void DelMsgIdentity(long nType, UINT nSenderId);

private:
    std::vector<CMessageSender*> m_arrMsgSender;
    std::vector<MSG_IDENTITY> m_arrMsgIdentity;
};

