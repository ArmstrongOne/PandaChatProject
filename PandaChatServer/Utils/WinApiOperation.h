//封装一些常见的winapi和shellapi的操作
#pragma once
#include<string>
#include"windows.h"
#include<vector>
using namespace std;


class WinApiOperation
{
public:
	~WinApiOperation();
	static WinApiOperation*GetWinApiOperation();

private:
	WinApiOperation();
	static WinApiOperation*api_operation;
	LPCWSTR StringToLPCWSTR(std::string orig);//将string转化为winapi需要的LPCWSTR

public:
//进程操作
	bool StartProcess(const string file_path);//打开进程
	bool KillProcess(string szProcessName);//结束进程
	bool KillProcess(DWORD dwPid);
	vector<DWORD> GetProcessIDByName(string szProcessName);//根据进程名字获取进程号
	 //杀进程也可以用 	string exe = "taskkill /f /t /im AcroRd32.exe";
	//WinExec(exe.c_str(), SW_HIDE);


//服务操作
    //用sc命令 用2天多简单学习了下dos
	bool StartService(const string exe_path);
	bool StopService(const string exe_path);

//编码转换
	string UTF8ToGB(const char* str);//utf8转码成gbk
	string GbkToUtf8(const char *src_str);//gbk转成utf8


//获取本机网卡mac地址
	bool GetMacByGetAdaptersAddresses(std::string& macOUT);//返回的第一块网卡
	bool GetAllMacAdapterAddresses(vector<string>&mac_address_vector);//所有网卡

//获取系统的当前时间
	void GetSystemTime(SYSTEMTIME&systime);
	string WordToString(WORD);
};

