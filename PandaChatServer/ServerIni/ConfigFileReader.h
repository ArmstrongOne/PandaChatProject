//C���ļ����淽��
#ifndef __CONFIG_FILE_READER_H__
#define __CONFIG_FILE_READER_H__

#include <map>
#include <string>

#define  IniFileName "ServerIni.ini"

class CConfigFileReader
{
public:
	CConfigFileReader(const char* floder_name);//�����ļ�������
	~CConfigFileReader();
    char* getConfigName(const char* name);
    int setConfigValue(const char* name, const char*  value);

	bool  LoadFile(); //��ȡ�ļ�
private:
    int   writeFile(const char* filename = NULL);
    void  parseLine(char* line);//��ÿ��ȡ��������ݲ���map
    char* trimSpace(char* name);//ȥ��ͷβ�����ַ� ����QString trimmer();

    bool                                m_load_ok;
    std::map<std::string, std::string>  m_config_map;
    std::string                         m_config_file;
};


#endif //!__CONFIG_FILE_READER_H__
