#include "FileOperation.h"
#include <fstream>  //ofstream是从内存到硬盘，ifstream是从硬盘到内存。
#include<iostream>
#include<direct.h>
#include <io.h>
#include <shellapi.h>
#include <tchar.h>
#include <sstream>
#include <stdint.h>
#include <codecvt>
#include <locale>
using namespace std;

FileOperation*FileOperation::file_operation = NULL;
bool FileOperation::IsFileExist(const string&file_path)
{
	string gbk_file_path = UTF8ToGB(file_path.c_str());
	ifstream file(gbk_file_path);
	if (file)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//通过 findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
//可以判断 dwFileAttributes项的值中是否 FILE_ATTRIBUTE_DIRECTORY，即判断所找到的项目是不是文件夹
bool FileOperation::IsFolderExist(const string& file_path)
{
	string gbk_file_path = UTF8ToGB(file_path.c_str());

	WIN32_FIND_DATA  wfd;
	bool rValue = false;

	LPCWSTR lpc_wstr = StringToLPCWSTR(gbk_file_path);
	HANDLE hFind = FindFirstFile(lpc_wstr, &wfd);


	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);

	if (lpc_wstr!=NULL)
	{
		delete lpc_wstr;
	}
	return rValue;
}

int FileOperation::IsFileOrFolder(const string& file_path)
{
	bool is_file = IsFileExist(file_path);
	bool is_folder= IsFolderExist(file_path);

	if (is_file && !is_folder)  
	{
		return 1;
	}

	if (!is_file &&is_folder)
	{
		return 2;
	}

	if (!is_file && !is_folder)
	{
		return 3;
	}
	return 3;
}


FileOperation::FileOperation()
{
}

LPCWSTR FileOperation::StringToLPCWSTR(std::string orig)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
	int len = MultiByteToWideChar(CP_ACP, 0, orig.c_str(), int(orig.size()), NULL, 0);
	wchar_t* buffer = new wchar_t[len + 1];
	//多字节编码转换成宽字节编码  
	MultiByteToWideChar(CP_ACP, 0, orig.c_str(), int(orig.size()), buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
	return buffer;  //这个需要在外部delete掉!!!!!!!!!!!
}

bool FileOperation::CreateFile(string write_str,const string& file_path)
{
	string gbk_string = UTF8ToGB(file_path.c_str());
	ofstream fout(gbk_string);
	if (fout) 
	{ 
		// 如果创建成功
		fout <<write_str<<endl;
		fout.close();  // 执行完操作后关闭文件句柄
		return true;
	}
	else
	{
		return false;
	}
}

bool FileOperation::CreateFolder(const string& file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	BOOL ret = CreateDirectoryA(LPCSTR(gbk_str.c_str()), NULL);

	if (ret)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool FileOperation::CreateFolderTree(const string& file_path)
{
	//找到所有"\\"出现的位置
	vector<size_t>str_index_vec;

	size_t index = 0;
	//find找不到返回值会很大 
	while ((index = file_path.find("\\", index)) < file_path.length())
	{
		//找到一次传入index的数值
		str_index_vec.push_back(index);
		index++;
	}
	if (str_index_vec.size() == 0)
	{
		return false;
	}

	//开始一个一个\检测赋值
	for (size_t i = 0; i < str_index_vec.size(); i++)
	{
		//从\处开始截取
		string temp_path = file_path.substr(0, str_index_vec.at(i));
		if (!IsFileExist(temp_path))
		{
			this->CreateFolder(temp_path);
		}
	}

	//这里会漏掉最后一个完整的路径也需要判断
	if (!IsFileExist(file_path))
	{
		this->CreateFolder(file_path);
	}
	return true;
}

bool FileOperation::ReadStringFromFile(const string& file_path,string&read_str)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	string str;
	ifstream fin(gbk_str);
	if (fin.is_open())
	{
		std::stringstream buffer;
		buffer << fin.rdbuf();
		//转换成gbk用于显示
		//read_str = UTF8ToGB(buffer.str().data());
		read_str = buffer.str();
		return true;
	}
	return false;
}

bool FileOperation::ReadStringFromFile16(const string& file_path, wstring&read_str)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	wstring str;
	ifstream fin(gbk_str);
	if (fin.is_open())
	{
		std::wstringstream buffer;
		buffer << fin.rdbuf();
		//转换成gbk用于显示
		//read_str = UTF8ToGB(buffer.str().data());
		read_str = buffer.str();
		return true;
	}
	return false;
}

std::string FileOperation::UTF8ToGB(const char* str)
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

std::string FileOperation::GbkToUtf8(const char *src_str)
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

wstring FileOperation::stringTowstring(string str)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return w_str;
}

//将wstring转换成string  
string FileOperation::wstringTostring(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(),(int)wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring FileOperation::UTF16StringLineToWstring(std::string utf16line)
{
	std::wstring result = L"";
	for (int i = 0; i < utf16line.length() - 1; i += 2)
	{
		unsigned char c1 = utf16line[i];
		unsigned char c2 = utf16line[i + 1];
		unsigned short wc;
		if (c2 == 0)
		{
			wc = c1;
		}
		else
		{
			wc = c2;
			wc = wc << 8;
			wc += c1;
		}
		result += wc;
	}
	return result;
}

std::string FileOperation::FileSaveCode(string file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	ifstream fin(gbk_str, ios::binary);
	unsigned char  s2;
	fin.read((char*)&s2, sizeof(s2));//读取第一个字节，然后左移8位
	int p = s2 << 8;
	fin.read((char*)&s2, sizeof(s2));//读取第二个字节
	p += s2;
	string code;
	switch (p)//判断文本前两个字节
	{
	case 0xfffe:  //65534
		code = "Unicode";
		break;
	case 0xfeff://65279
		code = "Unicode";
		break;
	case 0xefbb://61371
		code = "UTF-8";
		break;
	default:
		code = "ANSI";
	}
	fin.close();
	return code;
}

bool FileOperation::WriteStringToFile(string str, const string& file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	ofstream out(gbk_str);
	if (out.is_open())    //is_open()返回真（1），代表打开成功
	{
		out <<str<< endl;
		out.close();
		return true;
	}
	else
	{
		return false;
	}
}

bool FileOperation::DeleteFile(const string& file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	if (remove(gbk_str.c_str()) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool FileOperation::DeleteEmptyFolder(const string& file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());
	int ret=RemoveDirectory(StringToLPCWSTR(gbk_str));
	if (ret == 0) 
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

bool FileOperation::DeleteFolder(const string& file_path)
{
	if (NULL == file_path.c_str())	return false;
	char dir[MAX_PATH] = { 0 };
	char fileName[MAX_PATH] = { 0 };
	char *str = "\\*.*";
	strcpy_s(dir, file_path.c_str());
	strcat_s(dir, str);
	//首先查找dir中符合要求的文件
	long long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir, &fileinfo)) != -1)
	{
		do
		{
			strcpy_s(fileName, file_path.c_str());
			strcat_s(fileName, "\\");
			strcat_s(fileName, fileinfo.name);
			//检查是不是目录
			//如果不是目录,则进行处理文件夹下面的文件
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				remove(fileName);
			}
			else//处理目录，递归调用
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					DeleteFolder(fileName);
					//rmdir(fileName);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //_findnext()第一个参数”路径句柄”，返回的类型为intptr_t（long long），如果定义为long，在win7中是没有问题，但是在win10中就要改为long long或者intptr_t
		_findclose(hFile);
		_rmdir(file_path.c_str());
		return 1;
	}
	return false;
}


bool FileOperation::ClearFolder(const string& file_path)
{
	string gbk_str = UTF8ToGB(file_path.c_str());

	if (NULL == gbk_str.c_str())	return false;
	char dir[MAX_PATH] = { 0 };
	char fileName[MAX_PATH] = { 0 };
	char *str = "\\*.*";
	strcpy_s(dir, gbk_str.c_str());
	strcat_s(dir, str);
	//首先查找dir中符合要求的文件
	long long hFile;
	_finddata_t fileinfo;
	if ((hFile = _findfirst(dir, &fileinfo)) != -1)
	{
		do
		{
			strcpy_s(fileName, gbk_str.c_str());
			strcat_s(fileName, "\\");
			strcat_s(fileName, fileinfo.name);
			//检查是不是目录
			//如果不是目录,则进行处理文件夹下面的文件
			if (!(fileinfo.attrib & _A_SUBDIR))
			{
				remove(fileName);
			}
			else//处理目录，递归调用
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					DeleteFolder(fileName);
					//rmdir(fileName);
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //_findnext()第一个参数”路径句柄”，返回的类型为intptr_t（long long），如果定义为long，在win7中是没有问题，但是在win10中就要改为long long或者intptr_t
		_findclose(hFile);
		//_rmdir(file_path.c_str());
		return 1;
	}
	return false;
}

vector<std::string> FileOperation::GetAllFilePath(const std::string & path)
{
	string gbk_str = UTF8ToGB(path.c_str());

	vector<std::string>file_path_vector;
	file_path_vector.clear();
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息，_finddata_t需要io.h头文件  
	struct _finddata_t fileinfo;
	std::string p;        
	if ((hFile = _findfirst(p.assign(gbk_str).append("\\*").c_str(), &fileinfo))!= -1)//assign:将str替换原字串的内容
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					vector<string>temp_vector;
					//!!!!GYF 这里一定要转成utf8 因为这个函数递归接口就是utf8的 
					//fileinfo.name得到是GBK的
					string fileinfo_name = GbkToUtf8(fileinfo.name);
					temp_vector = GetAllFilePath(p.assign(path).append("\\").append(fileinfo_name));
					file_path_vector.insert(file_path_vector.end(), temp_vector.begin(), temp_vector.end());
				}
			}
			else
			{
				file_path_vector.push_back(p.assign(gbk_str).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return file_path_vector;
}


std::vector<std::string> FileOperation::GetAllFileName(const std::string & path)
{
	string gbk_str = UTF8ToGB(path.c_str());
	vector<std::string>file_name_vector;
	file_name_vector.clear();
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息，_finddata_t需要io.h头文件  
	struct _finddata_t fileinfo;
	string p=UTF8ToGB(u8"");
	if ((hFile = _findfirst(p.assign(gbk_str).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					vector<string>temp_vector;
					//！！！！GYF 这里一定要转成utf8 因为这个函数递归接口就是utf8的 
					//fileinfo.name得到是GBK的
					string fileinfo_name = GbkToUtf8(fileinfo.name);
					temp_vector = GetAllFileName(p.assign(path).append("\\").append(fileinfo_name));
					file_name_vector.insert(file_name_vector.end(), temp_vector.begin(), temp_vector.end());
				}
			}
			else
			{
				file_name_vector.push_back(fileinfo.name);
				cout << fileinfo.name << "\n";
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return file_name_vector;
}

std::vector<std::string> FileOperation::GetNextFolderPath(const std::string & path)
{
	string gbk_str = UTF8ToGB(path.c_str());

	vector<std::string>folder_path_vector;
	folder_path_vector.clear();
	//文件句柄  
	intptr_t hFile = 0;
	//文件信息，_finddata_t需要io.h头文件  
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(gbk_str).append("\\*").c_str(), &fileinfo)) != -1)//assign:将str替换原字串的内容
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					folder_path_vector.push_back(p.assign(gbk_str).append("\\").append(fileinfo.name));
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return folder_path_vector;
}

std::vector<std::string> FileOperation::GetLowestFolderPath(const std::string & path)
{
	//结果文件夹
	vector<std::string>result_folder;
	result_folder.clear();

	vector<std::string>next_list = this->GetNextFolderPath(path);
	for (auto iter = next_list.begin(); iter != next_list.end(); iter++)
	{
		//这里需要将gbk的iter改成u8 GetNextFolderPath默认输出是gbk
		string temp_str_u8 = GbkToUtf8((*iter).c_str());
		if (0 == GetNextFolderPath(temp_str_u8).size())
		{
			result_folder.push_back(*iter);//输出依然输出gbk的
		}
		else
		{
			vector<std::string>vec = this->GetLowestFolderPath(temp_str_u8);
			result_folder.insert(result_folder.end(), vec.begin(), vec.end());
		}
	}
	return result_folder;
}
bool FileOperation::CopyFile(string file_path, string target_path)
{
	string gbk_str_file_path = UTF8ToGB(file_path.c_str());
	string gbk_str_target_path = UTF8ToGB(target_path.c_str());

	ifstream in;
	ofstream out;

	in.open(gbk_str_file_path);

	if (in.fail()) 
	{
		in.close();
		out.close();
		return 0;
	}

	out.open(gbk_str_target_path);
	
	if (out.fail())
	{
		in.close();
		out.close();
		return 0;
	}

	//开始复制
	out << in.rdbuf();
	out.close();
	in.close();
	return true;
}

bool FileOperation::CopyFolder(string folder_path, string target_path)
{
	SHFILEOPSTRUCT fop;
	ZeroMemory(&fop, sizeof fop);
	fop.wFunc = FO_COPY;

	fop.pFrom = StringToLPCWSTR(folder_path+'\0');
	fop.pTo   = StringToLPCWSTR(target_path+ '\0');

	fop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
	SHFileOperation(&fop);
	return 0;
}

bool FileOperation::CopyAllFile(string file_path, string target_pat)
{
	bool flag=CopyFile(file_path.c_str(), target_pat.c_str());
	return flag;
}

FileOperation::~FileOperation()
{

}

FileOperation* FileOperation::GetFileOperation()
{
	if (file_operation == NULL)
	{
		file_operation = new FileOperation;
	}
	return file_operation;
}




