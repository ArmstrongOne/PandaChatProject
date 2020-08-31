//GYF封装
//用c++标准库和windowsapi封装的所有能想得到的文件操作 保证各个函数基本没有任何耦合并且内存不受影响
//基本概念file --文件    folder--目录/文件夹  文件操作后缀要完整
//中文路径测试无影响  识别不了的话请注意编码转换的问题(根据winapi接口传入gbk的窄字符和u8的宽字符)
//txt要读取显示要转换成utf8显示
//wstring专门承载u16 一个字符用2字节表示
//注意宽字符和窄字符的问题
//2019.9.26测试所有没问题
//ANSI和GBK编码所有数字英文都一样  u16比较特殊 单独考虑
//2019.11.21基本都改成了传入ut8的函数参数 有些可能漏掉 需要测试
//ifstream目前只接受gbk

#pragma once
#include<string>
#include <vector>
#include <windows.h>

using namespace std;
class FileOperation
{
public:
	~FileOperation();
	static FileOperation*GetFileOperation();
private:
	FileOperation();
	static FileOperation*file_operation;

public:
    LPCWSTR StringToLPCWSTR(std::string orig);//将string转化为winapi需要的LPCWSTR,这里string必须为gbk类型

//判断文件是否存在操作:
	bool IsFileExist(const string& file_path);//只能判断文件是否存在(文件显示有无后缀都适用,有后缀就必须加)
	bool IsFolderExist(const string& file_path);//只能判断文件夹是否存在 传u8

//判断路径目标是文件夹还是文件(用上面两个函数联合判断)
	//返回 1:文件 2:文件夹 3:路径不存在任何东西
	int IsFileOrFolder(const string& file_path);

//创建操作:
	bool CreateFile(string write_str,const string& file_path);//创建并写入文件(必须要有上一级目录)
	bool CreateFolder(const string& file_path);//创建文件夹(必须有上一级目录)
	bool CreateFolderTree(const string& file_path);//创建文件夹(不必须要上一级目录,直接创建完整，前提是路径正确)

//读取/写入操作(使用前先用上面函数判断存不存在该函数)
	bool ReadStringFromFile(const string& file_path,string&read_str);//默认读取保存成utf8的txt
	bool ReadStringFromFile16(const string& file_path, wstring&read_str);//默认读取保存成utf8的txt

	bool WriteStringToFile(string str, const string& file_path);

//删除操作:
	bool DeleteFile(const string& file_path);//删除文件
	bool DeleteEmptyFolder(const string& file_path);//删除一个空的文件夹(必须是空的)
	bool DeleteFolder(const string& file_path);//删除文件夹
	bool ClearFolder(const string& file_path);//清空文件夹(本身不删) 传u8

//获取文件信息:
	vector<std::string> GetAllFilePath(const std::string & path);//获取文件夹下所有子文件的路径
	vector<std::string> GetAllFileName(const std::string & path);//获取文件夹下所有子文件的路径
	vector<std::string> GetNextFolderPath(const std::string & path);//获取文件夹下一级所有子文件夹的路径
	vector<std::string> GetLowestFolderPath(const std::string & path);//获取文件夹最下级文件夹的路径(没有子文件夹的文件夹)

//文件复制
	bool CopyFile(string file_path, string target_path); //只限txt和ini可以复制 要加文件后缀,两个都是文件路径,要确保有文件夹存在
	bool CopyFolder(string folder_path, string target_path);//将folder_path文件夹复制到target_path目录里去,使用时确保folder_path和target_path都存在
    bool CopyAllFile(string file_path, string target_pat);//复制所有类型文件 目前还是用不了 用QFile::copy()可以
	 
//编码及字符串转换
	string UTF8ToGB(const char* str);//utf8转码成gbk
	string GbkToUtf8(const char *src_str);//gbk转成utf8
	
//宽窄字符转换
	wstring stringTowstring(string str);//string转wstring 要不然string直接转LPSWTR会出问题
	string wstringTostring(wstring wstr);//wstring转string

	//将UTF16(Unicode)编码文件的字节流转换为宽字节流(亲测有效,配合上面读取txt内容直接读取正确的宽字节字符)
	std::wstring UTF16StringLineToWstring(std::string utf16line);


//判断文件保存编码(有bom的utf8和ansi和u16能区分了  但是没有bom的u8默认是ansi)
	string FileSaveCode(string file_path);//判断文件保存编码


	//以上组合基本实现项目中对文件操作的需要
};

