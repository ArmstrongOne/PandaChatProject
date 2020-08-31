#pragma once
#include <stdint.h>

//���Ƿ�ѹ����
enum
{
	PACKAGE_UNCOMPRESSED,
	PACKAGE_COMPRESSED
};

struct msg
{
	char     compressflag; //ѹ����־�����Ϊ1��������ѹ������֮������ѹ��
	int32_t  originsize;   //����ѹ��ǰ��С
	int32_t  compresssize; //����ѹ�����С
	char     reserved[16];
};

enum Regist_Return_Msg
{
	RegisteredAlready=101, //�Ѿ�ע��
	RegisteredFailed=100,  //ע��ʧ��
	RegisteredSuccess=0    //ע��ɹ�
};

enum msg_type
{
	msg_type_unknown,
	msg_type_heartbeat = 1000,      //������
	msg_type_register,              //ע��
	msg_type_login,                 //��½
	msg_type_getofriendlist,        //��ȡ�����б�
	msg_type_finduser,              //�����û�
	msg_type_operatefriend,         //��ӡ�ɾ���Ⱥ��Ѳ���
	msg_type_userstatuschange,      //�û���Ϣ�ı�֪ͨ
	msg_type_updateuserinfo,        //�����û���Ϣ
	msg_type_modifypassword,        //�޸ĵ�½����
	msg_type_creategroup,           //����Ⱥ��
	msg_type_getgroupmembers,       //��ȡȺ���Ա�б�
	msg_type_chat = 1100,           //������Ϣ
	msg_type_multichat,             //Ⱥ����Ϣ
	msg_type_kickuser,              //��������
	msg_type_remotedesktop,         //Զ������
	msg_type_updateteaminfo,        //�����û����ѷ�����Ϣ
	msg_type_modifyfriendmarkname,  //���º��ѱ�ע��Ϣ
	msg_type_movefriendtootherteam, //�ƶ�������

#ifdef FXN_VERSION
//����Э��
	msg_type_uploaddeviceinfo = 2000, //�ϴ��豸��Ϣ
	msg_type_downloaddeviceinfo = 2001  //��ȡ�豸��Ϣ
#endif
};

//��ʽ��ȫ
/**
*  ������Э��
**/
/*
cmd = 1000, seq = 0
cmd = 1000, seq = 0
**/


/*ע��Э��

����:
cmd = 1001, seq = 0,  {"username": "13917043329", "nickname": "balloon", "password": "123"}

����:
cmd = 1001, seq = 0,  {"code": 0, "msg": "ok"}                    ע��ɹ�
cmd = 1001, seq = 0,  {"code": 101, "msg": "registered already"}  �Ѿ�ע��
cmd = 1001, seq = 0,  {"code": 100, "msg": "registered failed"}   ע��ʧ��

*/

/**
*  ��¼Э��
**/
/*
//status: ����״̬ 0���� 1���� 2æµ 3�뿪 4����
//clienttype: �ͻ�������,pc=1, android=2, ios=3
cmd = 1002, seq = 0, {"username": "13917043329", "password": "123", "clienttype": 1, "status": 1}
cmd = 1002, seq = 0, {"code": 0, "msg": "ok", "userid": 8, "username": "13917043320", "nickname": "zhangyl",
"facetype": 0, "customface":"�ļ�md5", "gender":0, "birthday":19891208, "signature":"���������ڳɹ���",
"address":"�Ϻ��ж���·3261��", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
**/

/**
* ��ȡ�û��б�
**/
/*
cmd = 1003, seq = 0
cmd = 1003, seq = 0,
{
"code": 0,
"msg": "ok",
"userinfo": [
{
"teamname": "�ҵĺ���",
"members": [
{
"userid": 2,
"username": "qqq",
"nickname": "qqq123",
"facetype": 0,
"customface": "466649b507cdf7443c4e88ba44611f0c",
"gender": 1,
"birthday": 19900101,
"signature": "������Ҫ�ܶ������ѽ��xx",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 1,
"markname": "qq���Ժ�"
},
{
"userid": 3,
"username": "hujing",
"nickname": "hujingx",
"facetype": 0,
"customface": "",
"gender": 0,
"birthday": 19900101,
"signature": "",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 0
}
]
},
{
"teamname": "�ҵ�ͬѧ",
"members": [
{
"userid": 4,
"username": "qqq",
"nickname": "qqq123",
"facetype": 0,
"customface": "466649b507cdf7443c4e88ba44611f0c",
"gender": 1,
"birthday": 19900101,
"signature": "������Ҫ�ܶ������ѽ��xx",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 1
},
{
"userid": 5,
"username": "hujing",
"nickname": "hujingx",
"facetype": 0,
"customface": "",
"gender": 0,
"birthday": 19900101,
"signature": "",
"address": "",
"phonenumber": "",
"mail": "",
"clienttype": 1,
"status": 0,
"markname": "qq���Ժ�"
}
]
}
]
}

**/

/**
* �����û�
**/
/*
//type�������� 0���У� 1�����û� 2����Ⱥ
cmd = 1004, seq = 0, {"type": 1, "username": "zhangyl"}
cmd = 1004, seq = 0, { "code": 0, "msg": "ok", "userinfo": [{"userid": 2, "username": "qqq", "nickname": "qqq123", "facetype":0}] }
**/

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

/**
*  �û�״̬�ı�
**/
/*
//type 1�û�����״̬�ı� 2���� 3�û�ǩ����ͷ���ǳƷ����仯
//onlinestatusȡֵ�� 0���� 1���� 2���� 3æµ 4�뿪 5�ƶ������� 6�ƶ������� 7�ֻ��͵���ͬʱ����,
cmd = 1006, seq = 0, {"type": 1, "onlinestatus": 1, "clienttype": 1},  targetid(״̬�ı���û���Ϣ)
cmd = 1006, seq = 0, {"type": 3}, targetid(״̬�ı���û���Ϣ)
**/

/**
*  �����û���Ϣ
**/
/*
cmd = 1007, seq = 0, �û���Ϣ: {"nickname": "xx", "facetype": 0,"customface":"�ļ�md5", "gender":0, "birthday":19891208, "signature":"���������ڳɹ���",
"address":"�Ϻ��ж���·3261��", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
cmd = 1007, seq = 0, {"code": 0, "msg": "ok", "userid": 9, "username": "xxxx", "nickname": "xx", "facetype": 0,
"customface":"�ļ�md5", "gender":0, "birthday":19891208, "signature":"���������ڳɹ���",
"address":"�Ϻ��ж���·3261��", "phonenumber":"021-389456", "mail":"balloonwj@qq.com"}
**/

/**
*  �޸�����
**/
/*
cmd = 1008, seq = 0, {"oldpassword": "xxx", "newpassword": "yyy"}
cmd = 1008, seq = 0, {"code":0, "msg": "ok"}
**/

/**
*  ����Ⱥ
**/
/*
cmd = 1009, seq = 0, {"groupname": "�ҵ�Ⱥ����", "type": 0}
cmd = 1009, seq = 0, {"code":0, "msg": "ok", "groupid": 12345678, "groupname": "�ҵ�Ⱥ����"}, �û�id��Ⱥid����4�ֽ����ͣ�32λ��Ⱥid�ĸ�λ����λΪ1
**/

/**
*  ��ȡȺ��Ա
**/
/*
cmd = 1010, seq = 0, {"groupid": Ⱥid}
cmd = 1010, seq = 0, {"code":0, "msg": "ok", "groupid": 12345678,
"members": [{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1},
{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1},
{"userid": 1, "username": xxxx, "nickname": yyyy, "facetype": 1, "customface": "ddd", "status": 1, "clienttype": 1}]}
**/

/**
*  ����Э��
**/
/*
cmd = 1100, seq = 0, data:�������� targetid(��Ϣ������)
cmd = 1100, seq = 0, data:�������� senderid(��Ϣ������), targetid(��Ϣ������)
**/

/**
*  Ⱥ��Э��
**/
/*
cmd = 1101, seq = 0, data:�������� targetid(��Ϣ������)
cmd = 1101, seq = 0, data:��������, {"targets": [1, 2, 3]}(��Ϣ������)
**/

/**
*  ��������
**/
/*
cmd = 1102, seq = 0, data:
**/

/**
* ��Ļ��������
**/
/*
cmd = 1103, seq = 0, string: λͼͷ����Ϣ�� λͼ��Ϣ��targetId
**/

/**
* �����û����ѷ�����Ϣ
**/
/*
�ͻ������� cmd = 1104, seq = 0, data(������ַ���), int(�������ͣ�0�� 1ɾ 2��), string(�µķ�������), string(�ɵķ�����)
������Ӧ�� cmd = 1003, seq = 0,
**/

/**
*  ���º��ѱ�ע��Ϣ
*/
/*
�ͻ������� cmd = 1105, seq = 0, data: ��װ���json, friendid, newmarkname
������Ӧ�� cmd = 1003, seq = 0, data: {"code": 0, "msg": ok}
*/

/**
*   �ƶ���������������
*/
/*
�ͻ������� cmd = 1105, seq = 0, data(������ַ���), friendid(int32, �����ĺ���id), newteamname(string, �·�����), oldteamname(string, �ɷ�����)
������Ӧ�� cmd = 1003, seq = 0, data: {"code": 0, "msg": ok}
*/



////////////////////////
//������Ϣ
////////////////////////
/* �ϴ��豸��Ϣ
cmd = 2000, seq = 0, data(�豸��Ϣjson), �豸id(int32)����Ϣ����classtype(int32), �ϴ�ʱ��(int64�� UTCʱ��)
cmd = 2000, seq = 0, data: {"code": 0, "msg": "ok"}
**/

/*
cmd = 2001, seq = 0, data(��)���豸id(int32)����Ϣ����classtype(int32), �ϴ�ʱ��(int64, UTCʱ��)
cmd = 2001, seq = 0, data: {������豸��Ϣjson}
**/












