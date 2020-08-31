#include "LoginSetting.h"
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QApplication>
#include <QByteArray>
LoginSetting*LoginSetting::login_setting = NULL;

LoginSetting::LoginSetting()
{
	client_path = QApplication::applicationDirPath() + "\\ClientIni";
	main_path = client_path +"\\LoginIni";
	QString file_name= "\\LoginSetting.ini";

	pic_avatar_path = main_path + "\\Login_Avatar.png";
	file_path = main_path + file_name;

	is_auto_login = false;
	login_info.clear();
}

LoginSetting* LoginSetting::GetLoginSetting()
{
	if (login_setting == NULL)
	{
		login_setting = new LoginSetting;
	}
	return login_setting;
}

LoginSetting::~LoginSetting()
{

}

void LoginSetting::ReadLoginSettingFile()
{
	//不存在客户端配置文件夹创建ClientIni
	if (!QDir(client_path).exists())
	{
		QDir dir;
		dir.mkpath(client_path);
	}
	
	//不存在LoginIni文件夹创建LoginIni
	if (!QDir(main_path).exists())
	{
		QDir dir;
		dir.mkpath(main_path);
	}


	if (!QFile::exists(file_path))
	{
		//创建默认文件
		QDir dir;
		dir.mkpath(main_path);
	
		QString data="";
		data+="auto_login:false\n";
		data+="login_info:";
		
		//加密下
		QString data_encryption = data.toUtf8().toBase64();	
		QFile file(file_path);
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		QTextStream stream(&file);
		(stream)<<data_encryption;
		file.close();
	}
	else//存在文件
	{
		QString read_str;
		QFile file(file_path);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
				QByteArray all = file.readAll();
				QString str = QString::fromLocal8Bit(all);
				read_str += str;
		}
		file.close();

		QStringList read_list = QString::fromUtf8(QByteArray::fromBase64(read_str.toUtf8())).split("\n");

		if (0 == read_list.size() ||!read_list[0].contains("auto_login") ||!read_list[0].contains("login_info"))
		{
			//不符合规则的文件 直接删除 重新创建
			QFile::remove(file_path);
			this->ReadLoginSettingFile();//递归创建新的初始文件
			return;
		}

		QStringList context_list = read_list[0].split("\n");

		//判断第一行登陆模式
		QString auto_login_key= context_list[0].split(":")[0];
		QString auto_login_value = context_list[0].split(":")[1];
		if ("false" == auto_login_value)
		{
			is_auto_login = false;
		}
		else if ("true" == auto_login_value)
		{
			is_auto_login = true;
		}


		//判断第二行登陆信息
		login_info.clear();
		QString login_info_key = context_list[1].split(":")[0];
		QString login_info_value = context_list[1].split(":")[1];
		if ("login_info:" == context_list[1])
		{
			//login_info继续为空
		}
		else
		{
			login_info.append(login_info_value.split(",")[0]);
			login_info.append(login_info_value.split(",")[1]);
		}
	}

	if (!QFile::exists(pic_avatar_path))
	{
		//不存在就不去管他 QPixmap里为空
	}
	else
	{
        //存在就继续读取
		avatar_pix.load(pic_avatar_path);
	}
}

void LoginSetting::GetSetting(bool&is_auto_login, QList<QString>&login_info)
{
	is_auto_login = this->is_auto_login;
	login_info = this->login_info;
}

QPixmap LoginSetting::GetLoginPixmap()
{
	return avatar_pix;
}

void LoginSetting::SaveLoginSetting(bool is_auto_login, QList<QString>&login_info)
{
	//这一步一定是存在文件的
	QFile file(file_path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out.setCodec("UTF-8");
		file.resize(0);//清空文件内容
		file.close();
	}
	
	 //写入当前的属性
	QString auto_login_str;
	if (is_auto_login)
	{
		auto_login_str= "auto_login:true\n";
	}
	else
	{
		auto_login_str = "auto_login:false\n";
	}

	QString login_info_str;
	if (0 == login_info.size())
	{
		login_info_str="login_info:";
	}
	else if (2 == login_info.size())
	{
		login_info_str="login_info:"+login_info[0]+","+login_info[1];
	}

	QStringList input_list;
	input_list.append(auto_login_str + login_info_str);
	//CFileEncryption::WriteFile(input_list,file_path);
}

void LoginSetting::ChangeFileToNoAutoLogin()
{
	this->ReadLoginSettingFile();
	this->SaveLoginSetting(false, login_info);
}
