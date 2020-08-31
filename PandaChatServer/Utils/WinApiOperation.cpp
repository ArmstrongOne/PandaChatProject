#include <winsock2.h>
#include "WinApiOperation.h"
#include<iostream>
#include<TlHelp32.h>
#include <shellapi.h>
#include <iphlpapi.h>
#include <algorithm>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

WinApiOperation*WinApiOperation::api_operation =NULL;

LPCWSTR WinApiOperation::StringToLPCWSTR(std::string orig)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, orig.c_str(), orig.size(), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, orig.c_str(), orig.size(), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	return buffer;  //这个需要在外部delete掉
}

bool WinApiOperation::StartProcess(const string file_path)
{
	STARTUPINFO si; //参数设置
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	PROCESS_INFORMATION pi; //参数结束

	/*printf("Please enter the name of process to start:");
	std::string name;
	cin >> name;*/

	if (!CreateProcess(NULL,(LPWSTR)StringToLPCWSTR(file_path),NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		return false;
	}
	else 
	{

	}
	return true;
}

bool WinApiOperation::KillProcess(string szProcessName)
{
	string gbk_string = UTF8ToGB(szProcessName.c_str());
	vector<DWORD>process_name_vector = GetProcessIDByName(gbk_string);
	for (int process_num = 0; process_num < process_name_vector.size(); process_num++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, (DWORD)process_name_vector.at(process_num));
		if (hProcess == NULL)
		{
			return false;
		}
		if (!TerminateProcess(hProcess, 0))
		{
			return false;
		}
	}
	return true;
}


WinApiOperation::WinApiOperation()
{

}

WinApiOperation::~WinApiOperation()
{

}

WinApiOperation* WinApiOperation::GetWinApiOperation()
{
	if (api_operation == NULL)
	{
		api_operation = new WinApiOperation();
	}
	return api_operation;
}

vector<DWORD> WinApiOperation::GetProcessIDByName(string szProcessName)
{
	LPCWSTR process_name =StringToLPCWSTR(szProcessName);
	STARTUPINFO st;
	PROCESS_INFORMATION pi;
	PROCESSENTRY32 ps;
	HANDLE hSnapshot;
	vector<DWORD> dwPID;
	ZeroMemory(&st, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	st.cb = sizeof(STARTUPINFO);
	ZeroMemory(&ps, sizeof(PROCESSENTRY32));
	ps.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//拍摄进程快照
	if (hSnapshot == INVALID_HANDLE_VALUE)//快照拍摄失败
	{
		return dwPID;
	}
	if (!Process32First(hSnapshot, &ps))
	{
		return dwPID;
	}
	do
	{
		if (lstrcmpi(ps.szExeFile, process_name) == 0)//遍历进程快照，比较进程名
		{
			dwPID.push_back(ps.th32ProcessID);//将进程id放到容器中
		}
	} while (Process32Next(hSnapshot, &ps));
	// 没有找到  
	CloseHandle(hSnapshot);
	return dwPID;//返回容器
}

bool WinApiOperation::StartService(const string exe_path)
{
	int index=exe_path.find_last_of("/");
	
	string temp_str1 = exe_path;
	string front_str= temp_str1.substr(0,index); //截去最后一个 /XXX.后缀
	
	string temp_str2 = exe_path;                  //截取XXX.后缀
	string behind_str = temp_str2.substr(index+1);
	
	string temp_str3= exe_path;                   //截取 .后缀
	string temp_str4 = temp_str3.substr(index + 1);
	string behind_str_with_no_exe = temp_str4.substr(0, temp_str4.length()-4);
		
	string install_cmd ="sc create \"" + behind_str_with_no_exe + "\"" + " " + "binpath= " + "\"" + exe_path + "\"";
	string start_cmd = "sc start " + behind_str_with_no_exe;
	WinExec(LPCSTR(install_cmd.c_str()),0);
	WinExec(LPCSTR(start_cmd.c_str()), 0);
	
	 return true;
}

bool WinApiOperation::StopService(const string exe_path)
{
	int index = exe_path.find_last_of("/");

	string temp_str1 = exe_path;
	string front_str = temp_str1.substr(0, index); //截去最后一个 /XXX.后缀

	string temp_str2 = exe_path;                  //截取XXX.后缀
	string behind_str = temp_str2.substr(index + 1);

	string temp_str3 = exe_path;                   //截取 .后缀
	string temp_str4 = temp_str3.substr(index + 1);
	string behind_str_with_no_exe = temp_str4.substr(0, temp_str4.length() - 4);

	string delete_sever_cmd = "sc delete " + behind_str_with_no_exe;
	string stop_process_cmd = "taskkill /f /t /im " + behind_str;

	WinExec(LPCSTR(delete_sever_cmd.c_str()), 0);
	WinExec(LPCSTR(stop_process_cmd.c_str()), 0);

	return true;
}


std::string WinApiOperation::UTF8ToGB(const char* str)
{
	string result;
	WCHAR *strSrc;
	LPSTR szRes;

	//获得临时变量的大小
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//获得临时变量的大小
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);
	result = szRes;
	delete[]strSrc;
	delete[]szRes;
	return result;
}

std::string WinApiOperation::GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}


bool WinApiOperation::KillProcess(DWORD dwPid)
{
	//关闭进程
	HANDLE killHandle = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION |   // Required by Alpha
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx
		PROCESS_VM_WRITE,             // For WriteProcessMemory);
		FALSE, dwPid);
	if (killHandle == NULL)
		return false;
	TerminateProcess(killHandle, 0);
	return true;
}


bool WinApiOperation::GetMacByGetAdaptersAddresses(std::string& macOUT)
{
	bool ret = false;
	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	if (pAddresses == NULL)
		return false;
	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variables
	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAddresses);
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (pAddresses == NULL)
			return false;
	}
	if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
	{
		// If successful, output some information from the data we received
		for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if (pCurrAddresses->PhysicalAddressLength != 6)
				continue;
			char acMAC[32];
			sprintf_s(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int(pCurrAddresses->PhysicalAddress[0]),
				int(pCurrAddresses->PhysicalAddress[1]),
				int(pCurrAddresses->PhysicalAddress[2]),
				int(pCurrAddresses->PhysicalAddress[3]),
				int(pCurrAddresses->PhysicalAddress[4]),
				int(pCurrAddresses->PhysicalAddress[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	}
	free(pAddresses);
	return ret;
}

bool WinApiOperation::GetAllMacAdapterAddresses(vector<string>&mac_address_vector)
{
	mac_address_vector.clear();

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL)
	{
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return -1;
	}
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL)
		{
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return -1;    //    error data return
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		while (pAdapter)
		{
			//MyAdpterInfo info;
			//info.Name = std::string(pAdapter->AdapterName);
			//info.Description = std::string(pAdapter->Description);
			//info.Type = pAdapter->Type;
			char buffer[20];
			sprintf_s(buffer, "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x", pAdapter->Address[0],
				pAdapter->Address[1], pAdapter->Address[2], pAdapter->Address[3],
				pAdapter->Address[4], pAdapter->Address[5]);
			
			string cur_mac_str = std::string(buffer);
			transform(cur_mac_str.begin(), cur_mac_str.end(), cur_mac_str.begin(), ::toupper);
			mac_address_vector.push_back(cur_mac_str);
			//info.MacAddress = std::string(buffer);
		/*	IP_ADDR_STRING *pIpAddrString = &(pAdapter->IpAddressList);
			do
			{
				info.Ip.push_back(std::string(pIpAddrString->IpAddress.String));
				pIpAddrString = pIpAddrString->Next;
			} while (pIpAddrString);*/
			pAdapter = pAdapter->Next;
			//adpterInfo.push_back(info);
		}
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return 0;    // normal return
	}
	else
	{
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
		return 1;    //    null data return
	}
}

void WinApiOperation::GetSystemTime(SYSTEMTIME&systime)
{
	GetLocalTime(&systime);

	cout << systime.wYear << "年" /*<< setw(2) << setfill('0') */<< systime.wMonth << "月" << /*setw(2) << setfill('0') << */systime.wDay << "日  " << systime.wHour << "时" << systime.wMinute << "分" << systime.wSecond << "秒" << systime.wMilliseconds << "毫秒" << "  星期" << systime.wDayOfWeek;
}

std::string WinApiOperation::WordToString(WORD w)
{
	char tmpbuff[16];
	sprintf_s(tmpbuff, "%d", w);
	string res = tmpbuff; 
	return res;
}

