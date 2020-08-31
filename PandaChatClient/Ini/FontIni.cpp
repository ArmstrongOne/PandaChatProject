#include "FontIni.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>
FontIni * FontIni::font_ini = 0;

FontIni * FontIni::GetRXFont()
{
	if (font_ini == 0)
	{
		font_ini = new FontIni();
	}
	return font_ini;
}

void FontIni::SetLanguageMode(int mode)
{
	m_langeuge_mode = mode;
}

FontIni::FontIni()
{
	m_langeuge_mode = 0;
}

void FontIni::InitFontIni()
{
	QString dirpath = QApplication::applicationDirPath()+"\\ClientIni";
	QString file_name = "\\FontIni.ini";
	QString filepath = dirpath+ file_name;
	
	QDir dir;
	if (!QFile::exists(filepath))
	{
		m_chinese_family = QString::fromLocal8Bit("黑体");
		m_chinese_pointSize_level_Max = 30;
		m_chinese_pointSize_level_1 = 20;
		m_chinese_pointSize_level_2 = 14;
		m_chinese_pointSize_level_3 = 10;
		m_chinese_pointSize_level_Min = 8;

		m_english_family = QString::fromLocal8Bit("Arial");
		m_english_pointSize_level_Max = 30;
		m_english_pointSize_level_1 = 20;
		m_english_pointSize_level_2 = 14;
		m_english_pointSize_level_3 = 10;
		m_english_pointSize_level_Min = 8;

		m_digital_family = QString::fromLocal8Bit("黑体");
		m_digital_pointSize_level_Max2= 50;
		m_digital_pointSize_level_Max = 30;
		m_digital_pointSize_level_1 = 20;
		m_digital_pointSize_level_2 = 14;
		m_digital_pointSize_level_3 = 10;
		m_digital_pointSize_level_Min = 8;

		m_symbol_family = QString::fromLocal8Bit("黑体");
		m_symbol_pointSize_level_Max = 30;
		m_symbol_pointSize_level_1 = 20;
		m_symbol_pointSize_level_2 = 14;
		m_symbol_pointSize_level_3 = 10;
		m_symbol_pointSize_level_Min = 8;

		//创建文件夹和文件
		QDir dir;
		dir.mkpath(dirpath);
		QFile file(filepath);
		file.open(QIODevice::ReadWrite | QIODevice::Text);
		QTextStream data(&file);
		(data) << "***************Chinese Font******************" << "\r\n";
		(data) << "Chinese_Font:" << m_chinese_family << "\r\n";
		(data) << "ChineseFont_Level_Max:" << m_chinese_pointSize_level_Max << "\r\n";
		(data) << "ChineseFont_Level_1:"<< m_chinese_pointSize_level_1 << "\r\n";
		(data) << "ChineseFont_Level_2:" << m_chinese_pointSize_level_2 << "\r\n";
		(data) << "ChineseFont_Level_3:" << m_chinese_pointSize_level_3 << "\r\n";
		(data) << "ChineseFont_Level_Min:" << m_chinese_pointSize_level_Min << "\r\n";
		(data) << "***************English Font******************"  << "\r\n";
		(data) << "English_Font:" << m_english_family << "\r\n";
		(data) << "English_Font_Level_Max:" << m_english_pointSize_level_Max << "\r\n";
		(data) << "English_Font_Level_1:" << m_english_pointSize_level_1 << "\r\n";
		(data) << "English_Font_Level_2:" << m_english_pointSize_level_2 << "\r\n";
		(data) << "English_Font_Level_3:" << m_english_pointSize_level_3 << "\r\n";
		(data) << "English_Font_Level_Min:" << m_english_pointSize_level_Min << "\r\n";
		(data) << "***************Digital Font******************" << "\r\n";
		(data) << "Digital_Font:" << m_digital_family << "\r\n";
		(data) << "Digital_Font_Level_Max2:" << m_digital_pointSize_level_Max2 << "\r\n";
		(data) << "Digital_Font_Level_Max:" << m_digital_pointSize_level_Max << "\r\n";
		(data) << "Digital_Font_Level_1:" << m_digital_pointSize_level_1 << "\r\n";
		(data) << "Digital_Font_Level_2:" << m_digital_pointSize_level_2 << "\r\n";
		(data) << "Digital_Font_Level_3:" << m_digital_pointSize_level_3 << "\r\n";
		(data) << "Digital_Font_Level_Min:" << m_digital_pointSize_level_Min << "\r\n";
		(data) << "***************Symbol Font******************" << "\r\n";
		(data) << "Symbol_Font:" << m_symbol_family << "\r\n";
		(data) << "Symbol_Font_Level_Max:" << m_symbol_pointSize_level_Max << "\r\n";
		(data) << "Symbol_Font_Level_1:" << m_symbol_pointSize_level_1 << "\r\n";
		(data) << "Symbol_Font_Level_2:" << m_symbol_pointSize_level_2 << "\r\n";
		(data) << "Symbol_Font_Level_3:" << m_symbol_pointSize_level_3 << "\r\n";
		(data) << "Symbol_Font_Level_Min:" << m_symbol_pointSize_level_Min << "\r\n";
		file.close();
	}
	else 
	{
		QFile file(filepath);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{

			while (!file.atEnd())
			{
				QByteArray line = file.readLine();
				QString str = QString::fromLocal8Bit(line);
				QStringList  datalist = str.split(":");

				if (datalist.size() == 2)
				{
					if (datalist.at(0) == "Chinese_Font")
						m_chinese_family = datalist.at(1).simplified();
					if (datalist.at(0) == "ChineseFont_Level_Max")
						m_chinese_pointSize_level_Max = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "ChineseFont_Level_1")
						m_chinese_pointSize_level_1 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "ChineseFont_Level_2")
						m_chinese_pointSize_level_2 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "ChineseFont_Level_3")
						m_chinese_pointSize_level_3 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "ChineseFont_Level_Min")
						m_chinese_pointSize_level_Min = datalist.at(1).simplified().toInt();

					if (datalist.at(0) == "English_Font")
						m_english_family = datalist.at(1).simplified();
					if (datalist.at(0) == "English_Font_Level_Max")
						m_english_pointSize_level_Max = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "English_Font_Level_1")
						m_english_pointSize_level_1 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "English_Font_Level_2")
						m_english_pointSize_level_2 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "English_Font_Level_3")
						m_english_pointSize_level_3 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "English_Font_Level_Min")
						m_english_pointSize_level_Min = datalist.at(1).simplified().toInt();

					if (datalist.at(0) == "Digital_Font")
						m_digital_family = datalist.at(1).simplified();
					if (datalist.at(0) == "Digital_Font_Level_Max2")
						m_digital_pointSize_level_Max2 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Digital_Font_Level_Max")
						m_digital_pointSize_level_Max = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Digital_Font_Level_1")
						m_digital_pointSize_level_1 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Digital_Font_Level_2")
						m_digital_pointSize_level_2 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Digital_Font_Level_3")
						m_digital_pointSize_level_3 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Digital_Font_Level_Min")
						m_digital_pointSize_level_Min = datalist.at(1).simplified().toInt();

					if (datalist.at(0) == "Symbol_Font")
						m_symbol_family = datalist.at(1).simplified();
					if (datalist.at(0) == "Symbol_Font_Level_Max")
						m_symbol_pointSize_level_Max = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Symbol_Font_Level_1")
						m_symbol_pointSize_level_1 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Symbol_Font_Level_2")
						m_symbol_pointSize_level_2 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Symbol_Font_Level_3")
						m_symbol_pointSize_level_3 = datalist.at(1).simplified().toInt();
					if (datalist.at(0) == "Symbol_Font_Level_Min")
						m_symbol_pointSize_level_Min = datalist.at(1).simplified().toInt();
				}
			}
		}
		file.close();
	}

	m_chinese_levelMax.setFamily(m_chinese_family);
	m_chinese_levelMax.setPointSize(m_chinese_pointSize_level_Max);
	m_chinese_level1.setFamily(m_chinese_family);
	m_chinese_level1.setPointSize(m_chinese_pointSize_level_1);
	m_chinese_level2.setFamily(m_chinese_family);
	m_chinese_level2.setPointSize(m_chinese_pointSize_level_2);
	m_chinese_level3.setFamily(m_chinese_family);
	m_chinese_level3.setPointSize(m_chinese_pointSize_level_3);
	m_chinese_levelMin.setFamily(m_chinese_family);
	m_chinese_levelMin.setPointSize(m_chinese_pointSize_level_Min);

	m_english_levelMax.setFamily(m_english_family);
	m_english_levelMax.setPointSize(m_english_pointSize_level_Max);
	m_english_level1.setFamily(m_english_family);
	m_english_level1.setPointSize(m_english_pointSize_level_1);
	m_english_level2.setFamily(m_english_family);
	m_english_level2.setPointSize(m_english_pointSize_level_2);
	m_english_level3.setFamily(m_english_family);
	m_english_level3.setPointSize(m_english_pointSize_level_3);
	m_english_levelMin.setFamily(m_english_family);
	m_english_levelMin.setPointSize(m_english_pointSize_level_Min);

	m_digital_levelMax2.setFamily(m_digital_family);
	m_digital_levelMax2.setPointSize(m_digital_pointSize_level_Max2);
	m_digital_levelMax.setFamily(m_digital_family);
	m_digital_levelMax.setPointSize(m_digital_pointSize_level_Max);
	m_digital_level1.setFamily(m_digital_family);
	m_digital_level1.setPointSize(m_digital_pointSize_level_1);
	m_digital_level2.setFamily(m_digital_family);
	m_digital_level2.setPointSize(m_digital_pointSize_level_2);
	m_digital_level3.setFamily(m_digital_family);
	m_digital_level3.setPointSize(m_digital_pointSize_level_3);
	m_digital_levelMin.setFamily(m_digital_family);
	m_digital_levelMin.setPointSize(m_digital_pointSize_level_Min);

	m_symbol_levelMax.setFamily(m_symbol_family);
	m_symbol_levelMax.setPointSize(m_symbol_pointSize_level_Max);
	m_symbol_level1.setFamily(m_symbol_family);
	m_symbol_level1.setPointSize(m_symbol_pointSize_level_1);
	m_symbol_level2.setFamily(m_symbol_family);
	m_symbol_level2.setPointSize(m_symbol_pointSize_level_2);
	m_symbol_level3.setFamily(m_symbol_family);
	m_symbol_level3.setPointSize(m_symbol_pointSize_level_3);
	m_symbol_levelMin.setFamily(m_symbol_family);
	m_symbol_levelMin.setPointSize(m_symbol_pointSize_level_Min);
}
