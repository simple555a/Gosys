#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include "mysql.h"
#include "pmc.h"
#include<atlstr.h>
using  namespace ::std;




//�˽ӿ�֧�ֵ�MYsql�������

#define TABLESIZE 32

#define NAME_LENGTH 64

typedef struct SQLTag_st{
	  TCHAR server[NAME_LENGTH];
	  TCHAR table[NAME_LENGTH];
	  TCHAR sqlname[NAME_LENGTH];
      VARIANT v;	//	ʵʱֵ
      VARIANT type;	//	��������
	  VARIANT time;	//	ʱ���
	  VARIANT q;	//	������

}SQLTAG;

struct PMCTAG{
	 TCHAR pmcname[NAME_LENGTH];
	 bool operator < (const PMCTAG &a) const
	{
        //��pmcname�ɴ�С���С����Ҫ��С�������У�ʹ�á�>���ż���
        return  a.pmcname < pmcname  ;
    }
};

typedef map<PMCTAG,SQLTAG> CPQMap;


class CPmc2MysqlThread :
	public CPeriodicThread
{
public:
	CPmc2MysqlThread(void);
	~CPmc2MysqlThread(void);

	static __bool init();
	static __bool uninit();

protected:
	virtual void task();
	//int Init();
	int LoadSetting();
	//int InitProxy();
	//int InitMySQL();
	int LoadConfigure();
	int CreatePQTag(TCHAR *strLineBuf,TCHAR *strTable);

	int UpdatePMCvalue();
	int UpdateMySQLvalue();
	int connectMysql();

	CString	EidtMySQLCmdText(const CString strDBName,
		int style,
		VARIANT strValue,
		VARIANT strValueType,
		VARIANT strValueQ,
		VARIANT strStamp,
		CString strName
		);
	char	m_strSQLServer[NAME_LENGTH];
	char	m_strMySQLServer[NAME_LENGTH];
	char	m_strDataBase[NAME_LENGTH];
	char	m_strDTname[NAME_LENGTH];
	char	m_strSQLTable[TABLESIZE][NAME_LENGTH];
	char	m_strMySQLTable[TABLESIZE][NAME_LENGTH];
	char	m_strUsername[NAME_LENGTH];
	char	m_strPassword[NAME_LENGTH];
	char	m_strNameIndex[NAME_LENGTH];
	char	m_strValueIndex[NAME_LENGTH];
	char	m_strValueTypeIndex[NAME_LENGTH];
	char	m_strStampIndex[NAME_LENGTH];
	char	m_strQualityIndex[NAME_LENGTH];
	int m_iSQLTableCount;
	int m_iMySQLTableCount;
	MYSQL   mydata;
};

