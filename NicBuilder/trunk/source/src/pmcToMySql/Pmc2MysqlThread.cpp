#include "Pmc2MysqlThread.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ATLComTime.h>
#include <direct.h>
#include <shellapi.h>
#include"librtk.h"
#include"rtkproxy.h"
#include"pmc.h"

using namespace std;

#pragma comment(lib, "libutils.lib")
#pragma comment(lib, "libntservice.lib")
#pragma comment(lib, "librtk.lib")
#pragma comment(lib, "librtknet.lib")
#pragma comment(lib, "librtkproxy.lib")
#pragma comment(lib, "librtdb.lib")
#pragma comment(lib, "libmysql.lib")


CPQMap tag;
char*projdir=NULL;
CPmc2MysqlThread g_pmc2mysqlThread;

CPmc2MysqlThread::CPmc2MysqlThread(void)
{
	m_dwPeriod = 1000;//执行周期1秒
	m_bMsgLoops = __false;
	LoadSetting();
	//connectMysql();
}

CPmc2MysqlThread::~CPmc2MysqlThread(void)
{
}

__bool CPmc2MysqlThread::init()
{
	init_proxy(
		0, 
		PROXY_ENABLE_BGLOADER | 
		PROXY_SILENT
		);

	Sleep(10000);


	__bool ret = g_pmc2mysqlThread.start();
	utils_trace(
		"Krt pmc2mysqlThread started(0x%08x)...%s\n", 
		g_pmc2mysqlThread.id(), 
		ret? "Ok" : "Failed."
		);

	return ret;
}

__bool CPmc2MysqlThread::uninit()
{
	if (g_pmc2mysqlThread.stop(5000)){
		utils_trace("Krt BackupThread stopped.\n");
	}else{
		utils_trace("Krt BackupThread stopping timeout.\n");
	}
	return __true;
}

void CPmc2MysqlThread::task()
{
	UpdatePMCvalue();
	UpdateMySQLvalue();
}

/// <summary> 
/// 程序初始化
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns> 
//int CPmc2MysqlThread::Init()
//{
//	if(LoadSetting())
//	//	return  InitSQL() && InitPMCDB();
//   return  InitMySQL();
//	else
//		return 0;
//}

int CPmc2MysqlThread::LoadSetting()
{
	char buff[1024];
	USES_CONVERSION;
	projdir=get_working_dir();

	GetPrivateProfileString(
		_T("MYSQL"),
		_T("servername"),
		_T(""),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy_s(m_strMySQLServer,buff,_countof(m_strMySQLServer));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("username"),
		_T("sa"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy_s(m_strUsername,buff,min(_tcslen(buff),_countof(m_strUsername)));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("password"),
		_T(""),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy_s(m_strPassword,buff,min(_tcslen(buff),_countof(m_strPassword)));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("database"),
		_T(""),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy_s(m_strDataBase,buff,min(_tcslen(buff),_countof(m_strDataBase)));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("table"),
		_T(""),
		buff,
		_countof(buff),
		get_config_file());
	if(buff){
		CArgs table_list;
		parse_arg_ex(buff,&table_list,",");
		for(int i = 0; i < table_list.argc && i< TABLESIZE ;i++){
			int f=	_countof(m_strMySQLTable[i]);			  
			_tcsncpy_s(m_strMySQLTable[i],table_list.argv[i],min(_tcslen(table_list.argv[i]),_countof(m_strMySQLTable[i])));
		}
		m_iMySQLTableCount = table_list.argc ;
		memset(buff,0,sizeof(buff));
	}


	GetPrivateProfileString(_T("MYSQL"),
		_T("valuekey"),
		_T("value"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy_s(m_strValueIndex,buff,min(_tcslen(buff),_countof(m_strValueIndex)));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("namekey"),
		_T("tagname"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy(m_strNameIndex,buff,min(_tcslen(buff),_countof(m_strNameIndex)));
		memset(buff,0,sizeof(buff));
	}

	GetPrivateProfileString(_T("MYSQL"),
		_T("stampkey"),
		_T("curtime"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy(m_strStampIndex,buff,min(_tcslen(buff),_countof(m_strStampIndex)));
		memset(buff,0,sizeof(buff));
	}
	GetPrivateProfileString(_T("MYSQL"),
		_T("qualitykey"),
		_T("q"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy(m_strQualityIndex,buff,min(_tcslen(buff),_countof(m_strQualityIndex)));
		memset(buff,0,sizeof(buff));
	}
	GetPrivateProfileString(_T("MYSQL"),
		_T("typeKey"),
		_T("type"),
		buff,
		_countof(buff),
		get_config_file());
	if(buff)
	{
		_tcsncpy(m_strValueTypeIndex,buff,min(_tcslen(buff),_countof(m_strValueTypeIndex)));
		memset(buff,0,sizeof(buff));
	}
	//m_dwPeriod =GetPrivateProfileInt(_T("PMC"),
	//					_T("refreshtime"),
	//					2000,
	//					get_config_file());

	printf("\n初始化配置csv个数为 : %d",m_iMySQLTableCount);

	LoadConfigure();

	return true;
}

int CPmc2MysqlThread::connectMysql()
{
		int j = 0;
		mysql_init(&mydata);
		char *IpName=T2A(m_strMySQLServer);
		char *UserName1=T2A(m_strUsername);
		char *PassWd=T2A(m_strPassword);
		char *DBName=T2A(m_strDataBase);
		while(!mysql_real_connect(&mydata,IpName,UserName1,PassWd,DBName,3306,NULL,0))
		{
			_tprintf(_T(".."));
			j++;
			Sleep(1000);
			if(j= 30) return 0;
		}
		_tprintf(_T("\nConnect MySQL  successed"));
		return 1;
}

//int CPmc2MysqlThread::InitProxy()
//{
//	init_proxy(
//		0, 
//		PROXY_ENABLE_BGLOADER | PROXY_ENABLE_DISPATCHER
//		);
//}

/// <summary> 
/// 初始化MySQL数据库
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns> 
//int CPmc2MysqlThread:: InitMySQL()
//{
//	int i =0;
//	CString strSetText;
//	CString strMySQLText;
//
//	_tprintf(_T("\nusername:%s,password : %s"),m_strUsername,m_strPassword);
//	_tprintf(_T("\nConnecting MySQL.."));
//
//	Sleep(100);
//
//	return true;
//}

/// <summary> 
/// 读csv配置文件，完成配置
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns>
int CPmc2MysqlThread::LoadConfigure()
{
	
	try{
//		if(m_iSQLTableCount == 0) return 0;
		if(m_iMySQLTableCount == 0) return 0;
		TCHAR line[1024];
		TCHAR filepath[256];
		TCHAR tablename[256];
		FILE *fp;
		USES_CONVERSION;
		for( int i = 0; i < m_iMySQLTableCount;i++)
		{
			int nRow = 0;
			memset(tablename,0,sizeof(tablename));
			_tcsncpy(tablename,m_strMySQLTable[i],_countof(tablename));
			_tcscat_s(tablename,_T(".csv"));
			_stprintf(filepath,"%sconfig\\%s",projdir,tablename);
			  if(fp = _tfopen(filepath,_T("r")))
			  {
				  if(fp != NULL)
				  {
					  while(_fgetts(line,_countof(line),fp) != NULL)
					  {
						  if( 0 == nRow){
							  nRow++;
							  continue;
						  }
						  CreatePQTag(line,m_strMySQLTable[i]);
						  memset(line,0,sizeof(line));
						  nRow++;
					  }
					  fclose(fp);
				  }			  				
			  }
		}
	}catch(...){

	}

	return 1;
}

// <summary> 
/// 解析csv中的行，创建标签map
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns>
int CPmc2MysqlThread::CreatePQTag(TCHAR *strLineBuf,TCHAR *strTable)
{
	try{
		USES_CONVERSION;

		SQLTAG	q;
		PMCTAG  p;
		memset(&q,0,sizeof(q));

		VARIANT val;
		val.vt = VT_R4;
		val.fltVal=0.00;
		string strLine,sqlkey,pmckey;
		CArgs args;
		int idx;
		strLine = T2A(strLineBuf);	
		idx=strLine.find_first_not_of(" \t\n");
		if(idx == string::npos) 
		{
			return -1;
		}else if(idx != 0)
		{
			strLine=strLine.substr(idx);
		}
		if(strLine.length() == 0)  
		{
			return -1;
		}
		if(strLine.data()[0] == '#') 
		{
			return -2;
		}
		parse_arg_ey(strLine.data(), &args, _T(",\t\r\n"));

		if(args.argc < 2)
		{
			return -3;
		}
		idx = 0;

		pmckey = args.argv[idx++];

	 	if(pmckey.length() == 0)
		{		
			return -4;
		}
		if(pmckey.length() > NAME_LENGTH){
			return -6;
		}
		
		//将pmc标签写入map
		_tcscpy_s(p.pmcname,pmckey.c_str());


		sqlkey = args.argv[idx++];

	    if(sqlkey.length() == 0)
		{		
			return -4;
		}
		if(sqlkey.length() > NAME_LENGTH){
			return -6;
		}

	   	//将sql标签写入map
		_tcscpy_s(q.table,strTable);
		_tcscpy_s(q.sqlname,sqlkey.c_str());
		q.v =	val;

		pair<CPQMap::iterator,bool> ib;

		ib = tag.insert(CPQMap::value_type(p,q));

		if(!ib.second)
		{
			return -7;
		}
	 	

	}
	catch(...)
	{

	}
	return 1;

}

CString   CPmc2MysqlThread::EidtMySQLCmdText(const CString strDTName,
		int style,
		VARIANT strValue,
		VARIANT strValueType,
		VARIANT strValueQ,
		VARIANT strStamp,
		CString strName
		)
{

	CString	cmdText;

	switch(style)
	{
	case 0:
		{	
			cmdText = "create database ";
			cmdText = cmdText +"if not exists ";
			cmdText = cmdText + strDTName;
			break;
		}
	case 1:
		{

			cmdText =   "CREATE TABLE IF NOT EXISTS ";
			cmdText = cmdText +	m_strDTname;
			cmdText = cmdText + "(";
			cmdText = cmdText +	m_strNameIndex;
			cmdText = cmdText +	" varchar(32) NULL,";
			cmdText = cmdText +	m_strValueIndex;
			cmdText = cmdText +	" varchar(32) NULL)";
			cmdText = cmdText +	m_strValueTypeIndex;
			cmdText = cmdText +	" varchar(32) NULL)";
			cmdText = cmdText +	m_strQualityIndex;
			cmdText = cmdText +	" varchar(32) NULL)";
			cmdText = cmdText +	m_strStampIndex;
			cmdText = cmdText +	" varchar(32) NULL)";
			break;
		}
	case 2:
		{
			cmdText= "update ";
			cmdText = cmdText +	 m_strDataBase;
			cmdText = cmdText +	"." ;
			cmdText = cmdText +  strDTName;
			cmdText = cmdText + " SET ";
			cmdText = cmdText + m_strValueIndex;
			cmdText = cmdText +  "='";
			cmdText = cmdText + strValue;
			cmdText = cmdText + "', ";
			cmdText = cmdText + m_strValueTypeIndex;
			cmdText = cmdText +  "='";
			cmdText = cmdText + strValueType;
			cmdText = cmdText + "', ";
			cmdText = cmdText + m_strQualityIndex;
			cmdText = cmdText +  "='";
			cmdText = cmdText + strValueQ;
			cmdText = cmdText + "', ";
			cmdText = cmdText + m_strStampIndex;
			cmdText = cmdText +  "='";
			cmdText = cmdText + strStamp;
			cmdText = cmdText + "' ";
			cmdText = cmdText + " WHERE ";
			cmdText = cmdText + m_strNameIndex;
			cmdText = cmdText + "='";
			cmdText = cmdText + strName;
			cmdText = cmdText + "'";
			break;
		}

	}

	return cmdText;

}

__bool getBoolValue(string &tagNameStr, __bool &val)
{
	//val = FALSE;
	TAG_NAME tn;
	//PRTK_TAG *pRtkTag;
	const char *tagNameChar = tagNameStr.data();
	__bool retVal;
	retVal = tagname_by_textA(&tn, tagNameChar);
	if (!retVal)
	{
		return FALSE;
	}

	if(!lock_rtdb(__false, 1000)){
		return FALSE;
	}
	PRTK_TAG tagInfo = query_tag_f(&tn);
	
	if (tagInfo==NULL)
	{ 
		unlock_rtdb();
		return FALSE;
	}
	if (tagInfo->d.Value.Flags  & TF_Valid)
	{
		val = tagInfo->d.Value.Value.iValue;
	}

	unlock_rtdb();
	
	return TRUE;
}

/// <summary> 
/// PMC数据库导出数据
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns> 
int CPmc2MysqlThread::UpdatePMCvalue()
{
	
	CPQMap::iterator p=tag.begin();
	
	if(!lock_rtdb(__false, 500)){
		return S_FALSE;
	}


		while(p != tag.end())
		{	
			TAG_NAME tn;
			tagname_by_textA(&tn, p->first.pmcname);
			PRTK_TAG pTag;

			USES_CONVERSION;

			
			pTag = query_tag_f(&tn);

			if(!pTag){
				p->second.v.bstrVal = SysAllocString(L"N.A.");
				p->second.v.vt = VT_BSTR;
			}else{
				//实时值
				pmc_value_to_variant(&pTag->d.Value, &p->second.v);
				//时间戳
				p->second.time.vt =  VT_DATE;
				p->second.time.date	= (DATE)COleDateTime::GetCurrentTime();
				//类型
				p->second.type.vt =  VT_I4;
				p->second.type.lVal	= 2;
				//质量码
				p->second.q.vt =  VT_I4;
				p->second.q.lVal	= 2;

			}
			++p;
		}

	unlock_rtdb();
	
	return 1;
}

/// <summary> 
/// 刷新SQL数据库
/// </summary> 
/// <param name=""></param> 
/// <returns>
///		1:successed 
///		0: failed
///	</returns> 
int CPmc2MysqlThread::UpdateMySQLvalue()
{	
	/*try		
	{*/	
		//if(!m_adoConn->IsOpen()) return 0;
		//		BOOL bRet;
		int j = 0;
		const char * bchr;
		unsigned int bRet;
		CString strSetText;
		CString strMySQLText;
		VARIANT strSetValue;
		VARIANT strSetValueType;
		VARIANT strSetValueQ;
		VARIANT strSetStamp;
		CString strSetName;
		USES_CONVERSION;

		mysql_init(&mydata);
		char *IpName=T2A(m_strMySQLServer);
		char *UserName1=T2A(m_strUsername);
		char *PassWd=T2A(m_strPassword);
		char *DBName=T2A(m_strDataBase);
		while(!mysql_real_connect(&mydata,IpName,UserName1,PassWd,DBName,3306,NULL,0))
		{
			_tprintf(_T(".."));
			j++;
			Sleep(1000);
			if(j= 30) return 0;
		}
		_tprintf(_T("\nConnect MySQL  successed"));


		for(int i = 0; i < m_iMySQLTableCount; i++)
		{
			CPQMap::iterator p=tag.begin();
			while(p != tag.end())
			{
				if(_tcsicmp(m_strMySQLTable[i],p->second.table) == 0)
				{
					strSetText.Empty();
					strMySQLText.Empty();
					//						strSetText = p->second.sqlname;
					strSetText = m_strMySQLTable[i];
					strSetValue= p->second.v;

					strSetValueType = p->second.type;
					strSetValueQ =p->second.q;
					strSetStamp = p->second.time;
					strSetName = p->second.sqlname;

					strMySQLText = EidtMySQLCmdText(strSetText,
						2,
						strSetValue,
						strSetValueType,
						strSetValueQ,
						strSetStamp,
						strSetName);
					char *sqltext= T2A(strMySQLText.GetBuffer(0));
					//	strcpy(ff,strMySQLText.GetBuffer(256));
					//						判断sql语句在MySQL数据库中是否执行成功
					if(mysql_query(&mydata,sqltext)==0)
					{
						_tprintf(_T("\n Write SQL Index  %s successed"),p->second.sqlname);

					}
					else
					{
						bRet=mysql_errno(&mydata);
						bchr=mysql_error(&mydata);
						_tprintf(_T("\n Write SQL Index  %s failed: %s"),p->second.sqlname, bchr);
					}
					++p;
				}
				//Sleep(100);
			}
		}
		mysql_close(&mydata);

	/*}
	catch(...)
	{

	}*/
	return 1;
}