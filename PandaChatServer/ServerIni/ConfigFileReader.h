//C库文件保存方法
#ifndef __CONFIG_FILE_READER_H__
#define __CONFIG_FILE_READER_H__

#include <map>
#include <string>

#define  IniFileName "ServerIni.ini"

class CConfigFileReader
{
public:
	CConfigFileReader(const char* floder_name);//传入文件夹名字
	~CConfigFileReader();
    char* getConfigName(const char* name);
    int setConfigValue(const char* name, const char*  value);

	bool  LoadFile(); //读取文件
private:
    int   writeFile(const char* filename = NULL);
    void  parseLine(char* line);//从每行取合理的数据插入map
    char* trimSpace(char* name);//去除头尾特殊字符 类似QString trimmer();

    bool                                m_load_ok;
    std::map<std::string, std::string>  m_config_map;
    std::string                         m_config_file;
};


#endif //!__CONFIG_FILE_READER_H__
