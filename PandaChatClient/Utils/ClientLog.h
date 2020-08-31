//顾逸凡-客户端通用异步日志-2020.5.8
#pragma once
#include <string>
#include <thread>
#include <memory>
#include <mutex>  
#include <list>
using namespace std;

//日志等级 
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
		void SetSaveFolderPath(string save_fold_path);//设置保存文件夹路径
		bool InitLog();
		bool UnitLog();


		//输出线程ID号和所在函数签名、行号	
		bool Output(long nLevel, const char* pszFileName, int nLineNo, const char* pszFmt, ...);

	   ~ClientLog();
private:
		ClientLog();
		ClientLog(ClientLog&) = delete;
		ClientLog&operator=(ClientLog&) = delete;
		static ClientLog*log_obj;

		void AppendInfo(long level, std::string&str);//拼接日志信息
		void GetNowTime(char* pszTime, int nTimeStrLength);//获取当前时间
		void Crash();//Fatal级别使程序崩溃
		void WritFileThreadFun();
		bool WriteFile(string);

		string                        save_folder_path;//保存的文件夹路径
		FILE*                         save_file;//指向txt路径指针
		shared_ptr<std::thread>       m_write_thread;
		mutex                         m_writr_mutex;
		condition_variable            m_writr_cv;
		list<string>                  string_list;//写入项
};

