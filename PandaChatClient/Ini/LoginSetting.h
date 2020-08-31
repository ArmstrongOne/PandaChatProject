#pragma once
#include <QObject>
#include <QPixmap>
class LoginSetting : public QObject
{
	Q_OBJECT

public:
	static LoginSetting*GetLoginSetting();
	~LoginSetting();

	//��½�����������ӿ�
	void ReadLoginSettingFile();
	void GetSetting(bool&is_auto_login, QList<QString>&login_info);

	//�õ������ͷ��
	QPixmap GetLoginPixmap();


	//���水ť3
	void SaveLoginSetting(bool is_auto_login, QList<QString>&login_info);

	//���ļ��е��Զ�������Ϊfalse �û������벻�� 
	void ChangeFileToNoAutoLogin();
private:
	LoginSetting();


	static LoginSetting*login_setting;

	QString client_path;//�ͻ����������ļ�Ŀ¼
	QString main_path;//��½�����ļ��ļ���·��
	QString file_path;//��½��Ϣ�ļ�·��
	QString pic_avatar_path;//��½����ͷ��·��
	QPixmap avatar_pix;//��½����ͷ��ͼƬ

	bool is_auto_login;

	//��½��Ϣ 0:�˺� 1:����
	QList<QString>login_info;
};
