//表信息
#pragma once
#include <memory>
#include <map>
#include <vector>

#include "../mysqlapi/DatabaseMysql.h"

#define MAXCMDLEN 8192


//表字段数据结构
struct STableField
{
	STableField(){}
	STableField(std::string strName,std::string strType,std::string strIndex):
		m_strName(strName),
		m_strType(strType),
		m_strDesc(strIndex)
	{
	}
	std::string m_strName; //字段名
	std::string m_strType; //字段类型
	std::string m_strDesc; //字段描述
};


//表数据结构
struct STableInfo
{
	STableInfo(){}
	STableInfo(std::string strName)
		:m_strName(strName)
	{
	}
	std::string m_strName;                          //表名
	std::map<std::string,STableField> m_mapField;   //字段名和对应字段属性
	std::string m_strKeyString;                     //主键索引设置
};

class CMysqlManager
{
public:
    CMysqlManager(void);
    virtual ~CMysqlManager(void);

public:
    bool init(const char* host, const char* user, const char* pwd, const char* dbname);

	std::string getHost() { return m_strHost; }
	std::string getUser() { return m_strUser; }
	std::string getPwd() { return m_strPassword; }
	std::string getDBName() { return m_strDataBase; }
	std::string getCharSet() { return m_strCharactSet;  }

private:
	bool isDBExist();
	bool createDB();
	bool checkTable(const STableInfo& table);
	bool createTable(const STableInfo& table);
	bool updateTable(const STableInfo& table);

protected:
	std::shared_ptr<CDatabaseMysql>     m_poConn;

    std::string                         m_strHost;      //iP
    std::string                         m_strUser;      //用户名
    std::string                         m_strPassword;  //密码
    std::string                         m_strDataBase;  //数据库名
    std::string                         m_strCharactSet;

	std::vector<STableInfo>             m_vecTableInfo;
};

