#include "StdAfx.h"
#include "UserInfo.h"

CUserInfo* CUserInfo::shared_info(){

	static CUserInfo* info = NULL;
	
	if(info == NULL){
		info = new CUserInfo();
	}

	return info;
}

CUserInfo::CUserInfo(void)
{
}


CUserInfo::~CUserInfo(void)
{
}
