/**
 *  简单的配置文件读取类，ConfigFileReader.cpp
 *  zhangyl 2017.05.27
 */
#include "ConfigFileReader.h"
#include <stdio.h>  //for snprintf
#include <string.h>
#include"..\Utils\FileOperation.h"

CConfigFileReader::CConfigFileReader(const char* filename)
{
	//这里进行检测
	//没有文件夹就进行创建,确保loadfile时文件一定存在
	bool is_folder_exist = FileOperation::GetFileOperation()->IsFolderExist(filename);
	if (!is_folder_exist)
	{
		bool is_create_folder = FileOperation::GetFileOperation()->CreateFolder(filename);
	}

	//判断有无文件 无文件创建默认值
	string folder_path(filename);
	string whole_file_path = folder_path + "\\" + IniFileName;
	bool is_file_exist = FileOperation::GetFileOperation()->IsFileExist(whole_file_path);
	if (!is_file_exist)
	{
		//不存在,开始创建文件，并写入默认值
		string need_write_str = "";
		need_write_str = "ip=192.168.0.0.1\n"
			             "need_asd=false\n"
			             "need_asd=true\n"
			             "port=450\n"
			             "11=false\n"
			             "can_link=false\n";
		bool creat_file = FileOperation::GetFileOperation()->CreateFile(need_write_str, whole_file_path);
		//创建是否成功不重要 失败后面LoadFile会返回false
	}


	m_config_file = whole_file_path;
}

CConfigFileReader::~CConfigFileReader()
{
}

char* CConfigFileReader::getConfigName(const char* name)
{
	if (!m_load_ok)
	{
		return NULL;
	}

	char* value = NULL;
    std::map<std::string, std::string>::iterator it = m_config_map.find(name);
	if (it != m_config_map.end())
    {
		value = (char*)it->second.c_str();
	}
	return value;
}

int CConfigFileReader::setConfigValue(const char* name, const char* value)
{
    if(!m_load_ok)
        return -1;

    std::map<std::string, std::string>::iterator it = m_config_map.find(name);
    if(it != m_config_map.end())
    {
        it->second = value;
    }
    else
    {
        m_config_map.insert(std::make_pair(name, value));
    }

    return writeFile();
}
bool CConfigFileReader::LoadFile()
{
	FILE* fp = NULL;
	errno_t err;

	err = fopen_s(&fp, m_config_file.c_str(), "r");

	if (err != 0)
	{
		return false;
	}


	char buf[256];
	for (;;)
	{
		char* p = fgets(buf, 256, fp); //fgets函数功能为从指定的流中读取数据，每次读取一行
		if (!p)
			break;

		size_t len = strlen(buf);
		if (buf[len - 1] == '\n')
		{
			buf[len - 1] = 0;			// remove \n at the end 赋个空字符
		}

		char* ch = strchr(buf, '#');	// remove string start with #
		if (ch) //如果一行中带#,只取#前面的内容
		{
			*ch = 0;
		}

		if (strlen(buf) == 0) //长度为0 直接下次循环
		{
			continue;
		}
		parseLine(buf);
	}
	fclose(fp);
	m_load_ok = true;
	return true;
}

int CConfigFileReader::writeFile(const char* filename)
{
   FILE* fp = NULL;
   errno_t err;

   err = fopen_s(&fp, filename, "w");


   if(err!=0)
   {
       return -1;
   }

   char szPaire[128];
   std::map<std::string, std::string>::iterator it = m_config_map.begin();
   for (; it != m_config_map.end(); it++)
   {
      memset(szPaire, 0, sizeof(szPaire));
      snprintf(szPaire, sizeof(szPaire), "%s=%s\n", it->first.c_str(), it->second.c_str());
      size_t ret =  fwrite(szPaire, strlen(szPaire),1,fp);//向指定的文件中写入若干数据块
      if(ret != 1)
      {
          fclose(fp);
          return -1;
      }
   }
   fclose(fp);
   return 0;
}

void CConfigFileReader::parseLine(char* line)
{
	//找到=开始的字符串
	char* p = strchr(line, '=');
	if (p == NULL)
	{
		return;
	}
	*p = 0;
	char* key =  trimSpace(line);
	char* value = trimSpace(p + 1);
	if (key && value)
	{
        m_config_map.insert(std::make_pair(key, value));
	}
}

char* CConfigFileReader::trimSpace(char* name)
{
	//移除字符串首特殊字符
	char* start_pos = name;
	while ( (*start_pos == ' ') || (*start_pos == '\t') || (*start_pos == '\r'))
	{
		start_pos++;
	}

	if (strlen(start_pos) == 0)
		return NULL;

	//移除字符串尾特殊字符
	char* end_pos = name + strlen(name) - 1;
	while ( (*end_pos == ' ') || (*end_pos == '\t') || (*end_pos == '\r'))
	{
		*end_pos = 0;
		end_pos--;
	}

	//判断
	int len = (int)(end_pos - start_pos) + 1;
	if (len <= 0)
	{
		return NULL;
	}
	return start_pos;
}
