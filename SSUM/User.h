#pragma once

// CUser ��� ����Դϴ�.

class CUser : public CObject
{
private:
	CString ID,Name,PW;
	//Add Chat Room ID Array
public:
	CUser();
	CUser(CString,CString,CString);
	virtual ~CUser();
};


