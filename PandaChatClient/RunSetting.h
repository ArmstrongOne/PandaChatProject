//�����������
#pragma once
#include <windows.h>
class CConfigFileReader;
class RunSetting
{
public:
	static RunSetting*GetRunSetting();

	//�ж����������������
	bool RunSoftWare();

	bool InitWinSock(); //��ʼ��Windows�����
	bool InitClentIni();//��ȡ�����ļ�ģ��

	CConfigFileReader*GetCinfigFileInfo();
	~RunSetting();
private:
	 RunSetting();
	 RunSetting(RunSetting&) = delete;
	 RunSetting&operator=(RunSetting&) = delete;
	 static RunSetting*run_set_obj;

	//******************�����ʼ��
	 void UninitSocket();
	 BOOL InitSocket();

	 //******************�����ļ���ȡ
	 CConfigFileReader *base_ini_reader;//������Ϣ������ȡ
};