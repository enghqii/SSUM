// User.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSUM.h"
#include "User.h"


// CUser

CUser::CUser() : ID(L""),PW(L""),Name(L"")
{
}

CUser::CUser(CString ID,CString PW,CString Name) : ID(ID),PW(PW),Name(Name)
{
}

CUser::~CUser()
{
}


// CUser ��� �Լ�
