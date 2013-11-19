#pragma once

// CUser 명령 대상입니다.

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


