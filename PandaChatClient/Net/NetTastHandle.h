//�����¼�������,�ڲ�ά��һ���߳���Զ��,���Ͻ��,ͨ�����ھ�����͸���Ӧ�Ķ��� 
#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <list>
namespace Net
{
	class NetTastHandle
	{
	public: 
		void AddPacketVec(std::string packet_str);


    public:
		NetTastHandle();
		~NetTastHandle();
		NetTastHandle(NetTastHandle&) = delete;
		NetTastHandle&operator=(NetTastHandle&) = delete;

    private:
		void Start();//����һ���߳�
		void Stop(); //ֹͣ�߳�
		void Run();  //�߳�ִ�к���

		bool HandlePacket(std::string&);//���

    private:
	    std::list<std::string>       packet_str_vec; //��������string_vec
		bool                         stop_flag;
		std::shared_ptr<std::thread> handle_thread;  //�����¼��߳�
		std::mutex                   m_mutex;        //��    
		std::condition_variable      m_cvItems;      //��������
    
		HWND                         proxy_hwnd;     //�����ھ��

		//ҵ���߼�����
	private:
		bool                         HandleRegist(std::string data);  //ע�᷵��
	};
}

