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
	
	CString getTargetID(){ return this->m_targetID; } 
	void setTargetID(CString m_targetID){ this->m_targetID = m_targetID; }
	
	CString getTargetName(){ return this->m_targetName; } 
	void setTargetName(CString m_targetName){ this->m_targetName = m_targetName; }

private:
	
	CUserInfo(void);
	
	CString m_id;
	//CString m_password;
	CString m_name;

	CString m_targetName;
	CString m_targetID;
};

