//网络事件处理者,内部维持一个线程永远跑,不断解包,通过窗口句柄发送给相应的对象 
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
		void Start();//运行一个线程
		void Stop(); //停止线程
		void Run();  //线程执行函数

		bool HandlePacket(std::string&);//解包

    private:
	    std::list<std::string>       packet_str_vec; //解完包后的string_vec
		bool                         stop_flag;
		std::shared_ptr<std::thread> handle_thread;  //处理事件线程
		std::mutex                   m_mutex;        //锁    
		std::condition_variable      m_cvItems;      //条件变量
    
		HWND                         proxy_hwnd;     //代理窗口句柄

		//业务逻辑部分
	private:
		bool                         HandleRegist(std::string data);  //注册返回
	};
}

