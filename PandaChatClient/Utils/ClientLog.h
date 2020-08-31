//���ݷ�-�ͻ���ͨ���첽��־-2020.5.8
#pragma once
#include <string>
#include <thread>
#include <memory>
#include <mutex>  
#include <list>
using namespace std;

//��־�ȼ� 
enum LEVEL_LOG
{
	LEVEL_DEBUG_LOG, 
	LEVEL_INFO_LOG,
	LEVEL_WARN_LOG,
	LEVEL_ERROR_LOG,
	LEVEL_FATAL_LOG
};

#define LOG_DEBUG(...)    ClientLog::GetLogObj()->Output(LEVEL_DEBUG_LOG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    ClientLog::GetLogObj()->Output(LEVEL_INFO_LOG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)    ClientLog::GetLogObj()->Output(LEVEL_WARN_LOG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)    ClientLog::GetLogObj()->Output(LEVEL_ERROR_LOG, __FILE__, __LINE__,__VA_ARGS__)
#define LOG_FATAL(...)    ClientLog::GetLogObj()->Output(LEVEL_FATAL_LOG, __FILE__, __LINE__, __VA_ARGS__)


class ClientLog
{
public:
	    static ClientLog*GetLogObj();
		void SetSaveFolderPath(string save_fold_path);//���ñ����ļ���·��
		bool InitLog();
		bool UnitLog();


		//����߳�ID�ź����ں���ǩ�����к�	
		bool Output(long nLevel, const char* pszFileName, int nLineNo, const char* pszFmt, ...);

	   ~ClientLog();
private:
		ClientLog();
		ClientLog(ClientLog&) = delete;
		ClientLog&operator=(ClientLog&) = delete;
		static ClientLog*log_obj;

		void AppendInfo(long level, std::string&str);//ƴ����־��Ϣ
		void GetNowTime(char* pszTime, int nTimeStrLength);//��ȡ��ǰʱ��
		void Crash();//Fatal����ʹ�������
		void WritFileThreadFun();
		bool WriteFile(string);

		string                        save_folder_path;//������ļ���·��
		FILE*                         save_file;//ָ��txt·��ָ��
		shared_ptr<std::thread>       m_write_thread;
		mutex                         m_writr_mutex;
		condition_variable            m_writr_cv;
		list<string>                  string_list;//д����
};

