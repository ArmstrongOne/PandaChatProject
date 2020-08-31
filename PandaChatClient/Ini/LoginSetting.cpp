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
	//�����ڿͻ��������ļ��д���ClientIni
	if (!QDir(client_path).exists())
	{
		QDir dir;
		dir.mkpath(client_path);
	}
	
	//������LoginIni�ļ��д���LoginIni
	if (!QDir(main_path).exists())
	{
		QDir dir;
		dir.mkpath(main_path);
	}


	if (!QFile::exists(file_path))
	{
		//����Ĭ���ļ�
		QDir dir;
		dir.mkpath(main_path);
	
		QString data="";
		data+="auto_login:false\n";
		data+="login_info:";
		
		//������
		QString data_encryption = data.toUtf8().toBase64();	
		QFile file(file_path);
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		QTextStream stream(&file);
		(stream)<<data_encryption;
		file.close();
	}
	else//�����ļ�
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
			//�����Ϲ�����ļ� ֱ��ɾ�� ���´���
			QFile::remove(file_path);
			this->ReadLoginSettingFile();//�ݹ鴴���µĳ�ʼ�ļ�
			return;
		}

		QStringList context_list = read_list[0].split("\n");

		//�жϵ�һ�е�½ģʽ
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


		//�жϵڶ��е�½��Ϣ
		login_info.clear();
		QString login_info_key = context_list[1].split(":")[0];
		QString login_info_value = context_list[1].split(":")[1];
		if ("login_info:" == context_list[1])
		{
			//login_info����Ϊ��
		}
		else
		{
			login_info.append(login_info_value.split(",")[0]);
			login_info.append(login_info_value.split(",")[1]);
		}
	}

	if (!QFile::exists(pic_avatar_path))
	{
		//�����ھͲ�ȥ���� QPixmap��Ϊ��
	}
	else
	{
        //���ھͼ�����ȡ
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
	//��һ��һ���Ǵ����ļ���
	QFile file(file_path);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out.setCodec("UTF-8");
		file.resize(0);//����ļ�����
		file.close();
	}
	
	 //д�뵱ǰ������
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
