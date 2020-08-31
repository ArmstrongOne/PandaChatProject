#include "EventLoopThread.h"
#include <functional>
#include "EventLoop.h"

using namespace net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
								 const std::string& name/* = ""*/)
								 : loop_(NULL),
								 exiting_(false),
								 callback_(cb)//callback_主要是在该IOThread执行循环之前需要执行的自定义函数操作（可不赋值）
{

}

EventLoopThread::~EventLoopThread()
{
	exiting_ = true;
	if (loop_ != NULL) // not 100% race-free, eg. threadFunc could be running callback_.
	{
		// still a tiny chance to call destructed object, if threadFunc exits just now.
		// but when EventLoopThread destructs, usually programming is exiting anyway.
		loop_->quit();
		thread_->join();
	}
}

EventLoop* EventLoopThread::startLoop()
{
	//assert(!thread_.started());
	//thread_.start();
	thread_.reset(new std::thread(std::bind(&EventLoopThread::threadFunc, this)));
	{
		std::unique_lock<std::mutex> lock(mutex_);
		while (loop_ == NULL)
		{
			cond_.wait(lock);
		}
	}
	return loop_;
}

void EventLoopThread::stopLoop()
{
    if (loop_ != NULL)
        loop_->quit();

    thread_->join();
}


/*
* threadFunc()运行的就是实例化一个EventLoop，并让这个EventLoop进入到loop状态。
* 运行EventLoopThread内的回调callback_
* 将刚定义好的loop传入这个回调
* 现在这个回调肯定是EventLoopThread的拥有者注册进去的
* 然后EventLoopThread也就有了一个EventLoop
* 使用loop_指向他，在这个线程中这个EventLoop一直出于loop()状态
*/
void EventLoopThread::threadFunc()
{
	EventLoop loop;

	if (callback_)   //???这个回调是什么意思??? 可能是需要自己定义的一些操作
	{
		callback_(&loop);// 如果有初始化函数，就先调用初始化函数
	}

	{
		//一个一个的线程创建
        std::unique_lock<std::mutex> lock(mutex_);
		loop_ = &loop;
		cond_.notify_all();
	}

	loop.loop();
	//assert(exiting_);
	loop_ = NULL;
}

