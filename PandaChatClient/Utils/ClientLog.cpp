#include"stdafx.h"
#include "ClientLog.h"
#include <ctime>
#include <time.h>
#include <sys/timeb.h>
#include<iostream>
#include <sstream>
#include <stdarg.h>
#include"..\Utils\FileOperation.h"
ClientLog*ClientLog::log_obj = nullptr;


#define IS_SUB_LONG_LOG    0  //是否截断长日志
#define MAX_LINE_LENGTH   256 //超过这个长度日志截断后面超过的部分

void ClientLog::AppendInfo(long nLevel, std::string&str)
{
	//级别
	str = "[INFO]"; //默认Debug
	if (nLevel == LEVEL_DEBUG_LOG)
		str = "[DEBUG]";
	else if (nLevel == LEVEL_INFO_LOG)
		str = "[INFO]";
	else if (nLevel == LEVEL_WARN_LOG)
		str = "[WARN]";
	else if (nLevel == LEVEL_ERROR_LOG)
		str = "[ERROR]";
	else if (nLevel == LEVEL_FATAL_LOG)
		str = "[FATAL]";

	//拼接时间
	char szTime[64] = { 0 };
	GetNowTime(szTime, sizeof(szTime));

	str += "[";
	str += szTime;
	str += "]";

	//拼接当前线程信息
	char szThreadID[32] = { 0 };
	std::ostringstream osThreadID;
	osThreadID<<std::this_thread::get_id();
	snprintf(szThreadID, sizeof(szThreadID), "[%s]", osThreadID.str().c_str());
	str += szThreadID;
}

void ClientLog::GetNowTime(char* pszTime, int nTimeStrLength)
{
	struct timeb tp;
	ftime(&tp);
	time_t now = tp.time;
	tm time;
#ifdef _WIN32
	localtime_s(&time, &now);
#else
	localtime_r(&now, &time);
#endif
	snprintf(pszTime, nTimeStrLength, "[%04d-%02d-%02d %02d:%02d:%02d:%03d]", time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, tp.millitm);
}

void ClientLog::Crash()
{
	char* p = nullptr;
	*p = 0;
}

void ClientLog::WritFileThreadFun()
{
	while(true)
	{
		std::unique_lock<std::mutex> guard(m_writr_mutex);
		if (0 == string_list.size())
		{
			m_writr_cv.wait(guard);
		}

		string need_write_str = string_list.front(); //返回list第一个元素引用
		string_list.pop_front();//删除list删除链表头的一元素


		if (!WriteFile(need_write_str))
		{
			//.......
		}
	}

}

bool ClientLog::WriteFile(string data)
{
	//为了防止长文件一次性写不完,放在一个循环里面分批写
	std::string strLocal(data);
	int ret = 0;
	while (true)
	{
		ret = fwrite(strLocal.c_str(), 1, strLocal.length(), save_file);
		if (ret <= 0)
		{
			return false;
		}
		else if (ret <= (int)strLocal.length())
		{
			strLocal.erase(0, ret);
		}

		if (strLocal.empty())
		{
			break;
		}
	}

	//::OutputDebugStringA(strDebugInfo.c_str());
	fflush(save_file);
	return true;
}

ClientLog* ClientLog::GetLogObj()
{
	if (log_obj == nullptr)
	{
		log_obj = new ClientLog;
	}

	return log_obj;
}

void ClientLog::SetSaveFolderPath(string save_folder_path)
{
	this->save_folder_path = save_folder_path;
}

bool ClientLog::InitLog()
{
	//判断文件夹存不存在
	bool is_folder_exist = FileOperation::GetFileOperation()->IsFolderExist(save_folder_path);
	if (!is_folder_exist)
	{
		//不存在直接return;
		return false;
	}

	//获取今天日期
	char szTime[64] = { 0 };
	struct timeb tp;
	ftime(&tp);
	time_t now = tp.time;
	tm time;
	localtime_s(&time, &now);
	string cur_year= to_string(time.tm_year+1900);
	string cur_month= to_string(time.tm_mon+1);
	string cur_day = to_string(time.tm_mday);
	string save_file_name = cur_year + "-" + cur_month + "-"+cur_day+".txt";

	//完整文件路径 顺便判断有没有文件
	string whole_file_path = save_folder_path + "\\" + save_file_name;
	bool is_file_exist= FileOperation::GetFileOperation()->IsFileExist(whole_file_path);
	if (!is_file_exist)
	{
		//不存在 则创建 这步骤文件夹是一定有的
		bool create_ret=FileOperation::GetFileOperation()->CreateFile("",whole_file_path);
		if (!create_ret)
		{
			//创建失败直接return;
			return false;
		}
	}

	//文件指针
	save_file = fopen(whole_file_path.c_str(), "w+");


	//开启线程
	m_write_thread = shared_ptr<std::thread>(new thread(bind(&ClientLog::WritFileThreadFun, this)));

	return true;
}

bool ClientLog::UnitLog()
{
	m_writr_cv.notify_one();

	if (m_write_thread->joinable())
	{
		//join是等待被创建线程的结束,回收线程资源
		m_write_thread->join();
	}

	if (save_file != nullptr)
	{
		fclose(save_file);
		save_file = nullptr;
	}

	return true;
}

bool ClientLog::Output(long nLevel, const char* pszFileName, int nLineNo, const char* pszFmt, ...)
{
	std::string strLine;
	AppendInfo(nLevel, strLine);  //拼接日志等级+时间+线程信息


	//写入行号和文件名 预设512字符
	char szFileName[512] = { 0 };
	snprintf(szFileName, sizeof(szFileName), "[%s:%d]", pszFileName, nLineNo);//将信息写入字符串
	strLine += szFileName;

	//日志正文
	std::string strLogMsg;

	//先计算一下不定参数的长度，以便于分配空间-gyf
	va_list ap;
	va_start(ap, pszFmt);
	int nLogMsgLength = vsnprintf(NULL, 0, pszFmt, ap);
	va_end(ap);

	//容量必须算上最后一个\0
	if ((int)strLogMsg.capacity() < nLogMsgLength + 1)//capacity返回string目前分配的最大存储容量
	{
		strLogMsg.resize(nLogMsgLength + 1);
	}

	va_list aq;
	va_start(aq, pszFmt);
	vsnprintf((char*)strLogMsg.data(), strLogMsg.capacity(), pszFmt, aq);
	va_end(aq);

	//string内容正确但length不对，恢复一下其length
	std::string strMsgFormal;
	strMsgFormal.append(strLogMsg.c_str(), nLogMsgLength);

	//日志太长开启截断，长日志只取前MAX_LINE_LENGTH个字符
	if (IS_SUB_LONG_LOG)
	{
		strMsgFormal = strMsgFormal.substr(0, MAX_LINE_LENGTH);
	}

	strLine += strMsgFormal;

	//在最后一行末尾加一个换行符
	strLine += "\n";

	//LEVEL_FATAL_LOG日志同步写日志然后崩溃程序 其他等级正常写日志
	if (nLevel != LEVEL_FATAL_LOG)
	{
		std::lock_guard<std::mutex> lock_guard(m_writr_mutex);
		string_list.push_back(strLine);
		m_writr_cv.notify_one();
	}
	else
	{
		//让FATAL级别同步写日志的,然后立即crash程序
#ifdef _WIN32
		OutputDebugStringA(strLine.c_str());
#endif
		WriteFile(strLine);
		Crash();
	}
	return true;
}


ClientLog::~ClientLog()
{
	this->UnitLog();
}

ClientLog::ClientLog()
{
	string_list.clear();
	save_file = nullptr;
}



