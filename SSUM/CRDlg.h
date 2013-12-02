#pragma once

#include "../http-request/lib/http_request_manager.h"


// CCRDlg 폼 뷰입니다.

class CCRDlg : public CFormView, public FCHttpRequestManager
{
	DECLARE_DYNCREATE(CCRDlg)

protected:
	CCRDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CCRDlg();

public:
	enum { IDD = IDD_CRDLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// custom
private :

	int nFriends;
	std::string * pstrFriends;

	virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask);

	CString m_name;
	CString m_id;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void On32772();
	afx_msg void On32772();
	afx_msg void OnBnClickedUpdateList();
	afx_msg void OnBnClickedChat1();
	afx_msg void OnBnClickedChat2();
	afx_msg void OnBnClickedChat3();
};


