//Э����ƺ�����ͨ�Ų��ֲο������е�Flamingo
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

		//����ip��ַ�Ͷ˿ں�
		void    SetIpAndPort(std::string ip, short port);
		//����-�첽�ӿ�
		void    Send(const std::string& strBuffer);

		bool    Init();                  //�����߳�,����ͽ���2���̺߳���
		void    Uninit();                //�ͷ��߳�
		bool    Connect(int timeout = 3);//socket����
		void    Close();                 //socket�ر�
		bool    IsClosed();              //��������״̬
		void    Join();                  //�̵߳ȴ�����

	private:
		void    SendThreadProc();
		void    RecvThreadProc();

		bool	Send();
		bool	Recv();

		//����������
		void    SendHeartbeatPackage();

		/** �ж���ͨSocket���Ƿ��յ�����
		* @return -1���� 0������ 1������
		*/
		int    CheckReceivedData();

		//���
		bool   DecodePackages();

	private:
		SOCKET	                        m_socket;	          //SOCKET	
		bool                            m_connected;          //����״̬��־λ
		std::string	                    m_strServer;          //��������ַ
		short					        m_port;               //�˿ں�
		std::unique_ptr<std::thread>    m_spSendThread;       //�����߳�
		std::unique_ptr<std::thread>    m_spRecvThread;       //�����߳�
		std::string                     m_strSendBuf;         //�����ַ���
		std::string                     m_strRecvBuf;         //�����ַ���
		std::mutex                      m_mtSendBuf;          //������
		std::condition_variable         m_cvSendBuf;	      
		std::condition_variable         m_cvRecvBuf;	      
		bool                            m_bStop;              //���ͱ�־λ

		long                            m_nLastDataTime;        //���һ���շ����ݵ�ʱ��
		std::mutex                      m_mutexLastDataTime;    //����m_nLastDataTime�Ļ�����
		long                            m_nHeartbeatInterval;   //������ʱ����
		int32_t                         m_nHeartbeatSeq;        //���������к�

		NetTastHandle*                  handle_obj;           //�������
	};
}

