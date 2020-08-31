#include "stdafx.h"
#include "NetTastHandle.h"
#include"..\Net\protocolstream.h"
#include "..\Net\NetProtocol.h"
#include"IUProtocolData.h"
#include "..\jsoncpp1.9.0\json.h"
#include "UserSessionData.h"
using namespace std;
using namespace Net;
NetTastHandle::NetTastHandle():stop_flag(true)
{
	packet_str_vec.clear();
}

void NetTastHandle::Run()
{
	while (!stop_flag)
	{
		std::unique_lock<std::mutex> guard(m_mutex);//����(��������������list��)

		while (packet_str_vec.empty())              //��ֹ�߳���ٻ���
		{
			//�������˻��������������������ʵĻ���pthread_cond_wait���ͷ�����������ִ�С�
			//�������仯���������ʣ�pthread_cond_wait��ֱ�ӻ������
			m_cvItems.wait(guard);                  //��������
		}

		string cur_str = packet_str_vec.front();  
		packet_str_vec.pop_front();               //ɾ��ջ��
		//��һ�������������
		HandlePacket(cur_str);
	}
}

void NetTastHandle::Start()
{
	handle_thread.reset(new std::thread(std::bind(&NetTastHandle::Run, this)));
	stop_flag = false;
}

void NetTastHandle::Stop()
{


}

NetTastHandle::~NetTastHandle()
{


}

void Net::NetTastHandle::AddPacketVec(std::string packet_str)
{
	std::unique_lock<std::mutex> guard(m_mutex);//����
	   packet_str_vec.push_back(packet_str);
	   m_cvItems.notify_all();
}

bool Net::NetTastHandle::HandlePacket(string&packet_str)
{
	net::BinaryStreamReader readStream(packet_str.c_str(), packet_str.length());
	
	//�����
	int32_t cmd;
	if (!readStream.ReadInt32(cmd))
	{
		return false;
	}

	//���к�
	int m_seq;
	if (!readStream.ReadInt32(m_seq))
	{
		return false;
	}

	//������
	std::string data;
	size_t datalength;
	if (!readStream.ReadString(&data, 0, datalength))
	{
		return false;
	}

	switch (cmd)
	{
		//������������
	case msg_type_heartbeat:
		break;
		//ע��
	case msg_type_register:
		this->HandleRegist(data);
		break;
		//��¼
	case msg_type_login:
		//HandleLoginMessage(data);
		break;
		//�û���Ϣ
	case msg_type_getofriendlist:
		//HandleFriendListInfo(data);
		break;
		//��ȡȺ��Ա��Ϣ
	case msg_type_getgroupmembers:
		//HandleGroupBasicInfo(data);
		break;
		//�����û����
	case msg_type_finduser:
		//HandleFindFriendMessage(data);
		break;
		//�Ӻ��������ɾ�����ѻ��Ⱥ����Ⱥ
	case msg_type_operatefriend:
		//CacheNotifyMsg(data);
		break;
		//�û���Ϣ�������
	case msg_type_userstatuschange:
	{
		int32_t targetId;
		//int seq;
		if (!readStream.ReadInt32(targetId))
		{
			break;
		}
		//HandleUserStatusNotifyMessage(targetId, data);
	}

	break;

	//���¸�����Ϣ
	case msg_type_updateuserinfo:
		//HandleUpdateLogonUserInfoMessage(data);
		break;

		//�޸�����
	case msg_type_modifypassword:
		//HandleModifyPasswordResult(data);
		break;

		//����Ⱥ
	case msg_type_creategroup:
		//HandleCreateNewGroupResult(data);
		break;

		//������Ϣ
	case msg_type_chat:
	{
		int32_t senderId;
		//int seq;
		if (!readStream.ReadInt32(senderId))
		{
			break;
		}

		int32_t targetid;
		if (!readStream.ReadInt32(targetid))
		{
			break;
		}

		//CacheChatMessage(senderId, targetid, data);
	}
	break;

	//��������
	case msg_type_kickuser:
		//::PostMessage(m_hProxyWnd, FMG_MSG_SELF_STATUS_CHANGE, 0, 0);
		break;

		//��������
	case msg_type_remotedesktop:
	{
		std::string bmpHeader;
		size_t bmpHeaderlength;
		if (!readStream.ReadString(&bmpHeader, 0, bmpHeaderlength))
			break;

		std::string bmpData;
		size_t bmpDatalength;
		if (!readStream.ReadString(&bmpData, 0, bmpDatalength))
			break;

		int32_t target;
		if (!readStream.ReadInt32(target))
			break;

		//HandleScreenshotMessage(target, bmpHeader, bmpData);
	}
	break;


	//case NET_DATA_TARGET_INFO_CHANGE:
		//HandleTargetInfoChangeMessage((CTargetInfoChangeResult*)pNetData);
		//break;
	default:
		break;
	}
	return true;
}

bool Net::NetTastHandle::HandleRegist(std::string data)
{
	//{"code": 0, "msg": "ok", "userid": 8}
	Json::Reader JsonReader;
	Json::Value JsonRoot;
	if (!JsonReader.parse(data, JsonRoot))
	{
		return FALSE;
	}

	if (JsonRoot["code"].isNull())
		return FALSE;

	int nRetCode = JsonRoot["code"].asInt();
	//����PostMessage���첽ִ�еģ����Զ���Ҫ��new��Ȼ���ڽ��շ��ͷ�

	CRegisterResult* pRegisterResult = new CRegisterResult();
	if (nRetCode == 0)
		pRegisterResult->m_nResultCode = REGISTER_SUCCESS;
	else if (nRetCode == 101)
		pRegisterResult->m_nResultCode = REGISTER_EXIST;
	else
		pRegisterResult->m_nResultCode = REGISTER_FAILED;

	::PostMessage(proxy_hwnd, FMG_MSG_REGISTER, 0, (LPARAM)pRegisterResult);
	return TRUE;


	//���PostMessage���ݵ���һЩ��ջ���ı�����PostMessage�����Щ���ݴ�����һ���̵߳���Ϣ�����ﵫ
	//������ջ���ϵı�����PostMessage���ں�������ʱ��ϵͳ���գ���ô��һ�߳̾ͻ���ʲ�����Щ���ݡ�
	//����취���ڴ����߳�PostMessage��Ϣǰ��ջ���ϵ�����newһ�ݷŵ������ϣ������������ǳ���Ա�ִ�
//�����ֶ����յģ�Ȼ�����Щ���ݴ��͵���һ�̣߳�����һ�߳�ȥ�ֶ��ͷ�delete��
}
