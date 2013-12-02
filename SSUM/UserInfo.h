#pragma once
class CUserInfo
{
public:
	static CUserInfo* shared_info();
	~CUserInfo(void);

	CString getName(){ return this->m_name; }
	void setName(CString name){ this->m_name = name; }
	CString getID(){ return this->m_id; } 
	void setID(CString id){ this->m_id = id; }

private:
	
	CUserInfo(void);
	
	CString m_id;
	//CString m_password;
	CString m_name;

	//Cstring m_tagetID;
};

