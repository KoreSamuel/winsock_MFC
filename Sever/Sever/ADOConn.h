#pragma once
class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);
	_RecordsetPtr m_pRecordset;
	_ConnectionPtr m_pConnection;

	void OnInitADOConn();
	_RecordsetPtr& GetRecordset(_bstr_t bstrSQL);
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};

