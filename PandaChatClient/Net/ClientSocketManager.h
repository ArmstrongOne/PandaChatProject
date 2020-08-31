//协议设计和网络通信部分参考范蠡大佬的Flamingo
#pragma once
#include <thread>
#include <mutex>
#include <string>
#include <memory>
#include <condition_variable>
#include <stdint.h>
#include <WinSock2.h>

namespace Net
{
	class NetTastHandle;
}

namespace Net
{
	class ClientSocketManager
	{
	public:
		ClientSocketManager();
		~ClientSocketManager();
		ClientSocketManager(ClientSocketManager&) = delete;
		ClientSocketManager&operator=(ClientSocketManager&) = delete;

		//设置ip地址和端口号
		void    SetIpAndPort(std::string ip, short port);
		//发送-异步接口
		void    Send(const std::string& strBuffer);

		bool    Init();                  //开启线程,激活发送接受2个线程函数
		void    Uninit();                //释放线程
		bool    Connect(int timeout = 3);//socket连接
		void    Close();                 //socket关闭
		bool    IsClosed();              //返回连接状态
		void    Join();                  //线程等待销毁

	private:
		void    SendThreadProc();
		void    RecvThreadProc();

		bool	Send();
		bool	Recv();

		//心跳包发送
		void    SendHeartbeatPackage();

		/** 判断普通Socket上是否收到数据
		* @return -1出错， 0无数据 1有数据
		*/
		int    CheckReceivedData();

		//解包
		bool   DecodePackages();

	private:
		SOCKET	                        m_socket;	          //SOCKET	
		bool                            m_connected;          //连接状态标志位
		std::string	                    m_strServer;          //服务器地址
		short					        m_port;               //端口号
		std::unique_ptr<std::thread>    m_spSendThread;       //发送线程
		std::unique_ptr<std::thread>    m_spRecvThread;       //接受线程
		std::string                     m_strSendBuf;         //发送字符串
		std::string                     m_strRecvBuf;         //接受字符串
		std::mutex                      m_mtSendBuf;          //互斥锁
		std::condition_variable         m_cvSendBuf;	      
		std::condition_variable         m_cvRecvBuf;	      
		bool                            m_bStop;              //发送标志位

		long                            m_nLastDataTime;        //最近一次收发数据的时间
		std::mutex                      m_mutexLastDataTime;    //保护m_nLastDataTime的互斥体
		long                            m_nHeartbeatInterval;   //心跳包时间间隔
		int32_t                         m_nHeartbeatSeq;        //心跳包序列号

		NetTastHandle*                  handle_obj;           //解包对象
	};
}

