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


#define IS_SUB_LONG_LOG    0  //�Ƿ�ضϳ���־
#define MAX_LINE_LENGTH   256 //�������������־�ضϺ��泬���Ĳ���

void ClientLog::AppendInfo(long nLevel, std::string&str)
{
	//����
	str = "[INFO]"; //Ĭ��Debug
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

	//ƴ��ʱ��
	char szTime[64] = { 0 };
	GetNowTime(szTime, sizeof(szTime));

	str += "[";
	str += szTime;
	str += "]";

	//ƴ�ӵ�ǰ�߳���Ϣ
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

		string need_write_str = string_list.front(); //����list��һ��Ԫ������
		string_list.pop_front();//ɾ��listɾ������ͷ��һԪ��


		if (!WriteFile(need_write_str))
		{
			//.......
		}
	}

}

bool ClientLog::WriteFile(string data)
{
	//Ϊ�˷�ֹ���ļ�һ����д����,����һ��ѭ���������д
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
	//�ж��ļ��д治����
	bool is_folder_exist = FileOperation::GetFileOperation()->IsFolderExist(save_folder_path);
	if (!is_folder_exist)
	{
		//������ֱ��return;
		return false;
	}

	//��ȡ��������
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

	//�����ļ�·�� ˳���ж���û���ļ�
	string whole_file_path = save_folder_path + "\\" + save_file_name;
	bool is_file_exist= FileOperation::GetFileOperation()->IsFileExist(whole_file_path);
	if (!is_file_exist)
	{
		//������ �򴴽� �ⲽ���ļ�����һ���е�
		bool create_ret=FileOperation::GetFileOperation()->CreateFile("",whole_file_path);
		if (!create_ret)
		{
			//����ʧ��ֱ��return;
			return false;
		}
	}

	//�ļ�ָ��
	save_file = fopen(whole_file_path.c_str(), "w+");


	//�����߳�
	m_write_thread = shared_ptr<std::thread>(new thread(bind(&ClientLog::WritFileThreadFun, this)));

	return true;
}

bool ClientLog::UnitLog()
{
	m_writr_cv.notify_one();

	if (m_write_thread->joinable())
	{
		//join�ǵȴ��������̵߳Ľ���,�����߳���Դ
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
	AppendInfo(nLevel, strLine);  //ƴ����־�ȼ�+ʱ��+�߳���Ϣ


	//д���кź��ļ��� Ԥ��512�ַ�
	char szFileName[512] = { 0 };
	snprintf(szFileName, sizeof(szFileName), "[%s:%d]", pszFileName, nLineNo);//����Ϣд���ַ���
	strLine += szFileName;

	//��־����
	std::string strLogMsg;

	//�ȼ���һ�²��������ĳ��ȣ��Ա��ڷ���ռ�-gyf
	va_list ap;
	va_start(ap, pszFmt);
	int nLogMsgLength = vsnprintf(NULL, 0, pszFmt, ap);
	va_end(ap);

	//���������������һ��\0
	if ((int)strLogMsg.capacity() < nLogMsgLength + 1)//capacity����stringĿǰ��������洢����
	{
		strLogMsg.resize(nLogMsgLength + 1);
	}

	va_list aq;
	va_start(aq, pszFmt);
	vsnprintf((char*)strLogMsg.data(), strLogMsg.capacity(), pszFmt, aq);
	va_end(aq);

	//string������ȷ��length���ԣ��ָ�һ����length
	std::string strMsgFormal;
	strMsgFormal.append(strLogMsg.c_str(), nLogMsgLength);

	//��־̫�������ضϣ�����־ֻȡǰMAX_LINE_LENGTH���ַ�
	if (IS_SUB_LONG_LOG)
	{
		strMsgFormal = strMsgFormal.substr(0, MAX_LINE_LENGTH);
	}

	strLine += strMsgFormal;

	//�����һ��ĩβ��һ�����з�
	strLine += "\n";

	//LEVEL_FATAL_LOG��־ͬ��д��־Ȼ��������� �����ȼ�����д��־
	if (nLevel != LEVEL_FATAL_LOG)
	{
		std::lock_guard<std::mutex> lock_guard(m_writr_mutex);
		string_list.push_back(strLine);
		m_writr_cv.notify_one();
	}
	else
	{
		//��FATAL����ͬ��д��־��,Ȼ������crash����
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



