#include "EventLoopThread.h"
#include <functional>
#include "EventLoop.h"

using namespace net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb,
								 const std::string& name/* = ""*/)
								 : loop_(NULL),
								 exiting_(false),
								 callback_(cb)//callback_��Ҫ���ڸ�IOThreadִ��ѭ��֮ǰ��Ҫִ�е��Զ��庯���������ɲ���ֵ��
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
* threadFunc()���еľ���ʵ����һ��EventLoop���������EventLoop���뵽loop״̬��
* ����EventLoopThread�ڵĻص�callback_
* ���ն���õ�loop��������ص�
* ��������ص��϶���EventLoopThread��ӵ����ע���ȥ��
* Ȼ��EventLoopThreadҲ������һ��EventLoop
* ʹ��loop_ָ������������߳������EventLoopһֱ����loop()״̬
*/
void EventLoopThread::threadFunc()
{
	EventLoop loop;

	if (callback_)   //???����ص���ʲô��˼??? ��������Ҫ�Լ������һЩ����
	{
		callback_(&loop);// ����г�ʼ�����������ȵ��ó�ʼ������
	}

	{
		//һ��һ�����̴߳���
        std::unique_lock<std::mutex> lock(mutex_);
		loop_ = &loop;
		cond_.notify_all();
	}

	loop.loop();
	//assert(exiting_);
	loop_ = NULL;
}

