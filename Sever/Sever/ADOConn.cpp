#include "StdAfx.h"
#include "ADOConn.h"


CADOConn::CADOConn(void)
{
}


CADOConn::~CADOConn(void)
{
}

void CADOConn::OnInitADOConn()
{
	//
	::CoInitialize(NULL);
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect="Provider=Microsoft.Jet.OLEDB.4.0;Data Source=InterDB.mdb";
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败，确认数据库路径是否正确!"));  
	}
}
void CADOConn::ExitConnect()
{ 
	//关闭记录集和连接
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	//释放环境
	::CoUninitialize();
}
_RecordsetPtr& CADOConn::GetRecordset(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic,adLockOptimistic,adCmdText);
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("不能打开记录集!"));  
	}
	return m_pRecordset;
}
BOOL CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		if(m_pConnection==NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("不能打开记录集!")); 
		return false;
	}
}