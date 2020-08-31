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
		std::unique_lock<std::mutex> guard(m_mutex);//上锁(保护条件变量和list组)

		while (packet_str_vec.empty())              //防止线程虚假唤醒
		{
			//如果获得了互斥锁，但是条件不合适的话，pthread_cond_wait会释放锁，不往下执行。
			//当发生变化后，条件合适，pthread_cond_wait将直接获得锁。
			m_cvItems.wait(guard);                  //条件变量
		}

		string cur_str = packet_str_vec.front();  
		packet_str_vec.pop_front();               //删除栈首
		//进一步解包。。。。
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
	std::unique_lock<std::mutex> guard(m_mutex);//上锁
	   packet_str_vec.push_back(packet_str);
	   m_cvItems.notify_all();
}

bool Net::NetTastHandle::HandlePacket(string&packet_str)
{
	net::BinaryStreamReader readStream(packet_str.c_str(), packet_str.length());
	
	//命令号
	int32_t cmd;
	if (!readStream.ReadInt32(cmd))
	{
		return false;
	}

	//序列号
	int m_seq;
	if (!readStream.ReadInt32(m_seq))
	{
		return false;
	}

	//数据体
	std::string data;
	size_t datalength;
	if (!readStream.ReadString(&data, 0, datalength))
	{
		return false;
	}

	switch (cmd)
	{
		//心跳包不处理
	case msg_type_heartbeat:
		break;
		//注册
	case msg_type_register:
		this->HandleRegist(data);
		break;
		//登录
	case msg_type_login:
		//HandleLoginMessage(data);
		break;
		//用户信息
	case msg_type_getofriendlist:
		//HandleFriendListInfo(data);
		break;
		//获取群成员信息
	case msg_type_getgroupmembers:
		//HandleGroupBasicInfo(data);
		break;
		//查找用户结果
	case msg_type_finduser:
		//HandleFindFriendMessage(data);
		break;
		//加好友申请或删除好友或加群或退群
	case msg_type_operatefriend:
		//CacheNotifyMsg(data);
		break;
		//用户信息发生变更
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

	//更新个人信息
	case msg_type_updateuserinfo:
		//HandleUpdateLogonUserInfoMessage(data);
		break;

		//修改密码
	case msg_type_modifypassword:
		//HandleModifyPasswordResult(data);
		break;

		//创建群
	case msg_type_creategroup:
		//HandleCreateNewGroupResult(data);
		break;

		//聊天消息
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

	//被踢下线
	case msg_type_kickuser:
		//::PostMessage(m_hProxyWnd, FMG_MSG_SELF_STATUS_CHANGE, 0, 0);
		break;

		//截屏数据
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
	//由于PostMessage是异步执行的，所以对象要用new，然后在接收方释放

	CRegisterResult* pRegisterResult = new CRegisterResult();
	if (nRetCode == 0)
		pRegisterResult->m_nResultCode = REGISTER_SUCCESS;
	else if (nRetCode == 101)
		pRegisterResult->m_nResultCode = REGISTER_EXIST;
	else
		pRegisterResult->m_nResultCode = REGISTER_FAILED;

	::PostMessage(proxy_hwnd, FMG_MSG_REGISTER, 0, (LPARAM)pRegisterResult);
	return TRUE;


	//如果PostMessage传递的是一些在栈区的变量，PostMessage后把这些数据传到另一个线程的消息队列里但
	//由于是栈区上的变量在PostMessage所在函数返回时由系统回收，那么另一线程就会访问不了这些数据。
	//解决办法：在传送线程PostMessage消息前对栈区上的数据new一份放到堆区上，堆区的数据是程序员手创
//建和手动回收的，然后把这些数据传送到另一线程，由另一线程去手动释放delete。
}
