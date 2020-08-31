#pragma once
#include <QObject>
#include <QPixmap>
class LoginSetting : public QObject
{
	Q_OBJECT

public:
	static LoginSetting*GetLoginSetting();
	~LoginSetting();

	//登陆调用这两个接口
	void ReadLoginSettingFile();
	void GetSetting(bool&is_auto_login, QList<QString>&login_info);

	//得到保存的头像
	QPixmap GetLoginPixmap();


	//保存按钮3
	void SaveLoginSetting(bool is_auto_login, QList<QString>&login_info);

	//将文件中的自动保存置为false 用户名密码不管 
	void ChangeFileToNoAutoLogin();
private:
	LoginSetting();


	static LoginSetting*login_setting;

	QString client_path;//客户端总配置文件目录
	QString main_path;//登陆配置文件文件夹路径
	QString file_path;//登陆信息文件路径
	QString pic_avatar_path;//登陆保存头像路径
	QPixmap avatar_pix;//登陆保存头像图片

	bool is_auto_login;

	//登陆信息 0:账号 1:密码
	QList<QString>login_info;
};
