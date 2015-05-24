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
		AfxMessageBox(_T("���ݿ�����ʧ�ܣ�ȷ�����ݿ�·���Ƿ���ȷ!"));  
	}
}
void CADOConn::ExitConnect()
{ 
	//�رռ�¼��������
	if(m_pRecordset!=NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	//�ͷŻ���
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
		AfxMessageBox(_T("���ܴ򿪼�¼��!"));  
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
		AfxMessageBox(_T("���ܴ򿪼�¼��!")); 
		return false;
	}
}