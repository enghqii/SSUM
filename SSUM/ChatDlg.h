#pragma once

#include "../http-request/lib/http_request_manager.h"

#include "afxcoll.h"

class ChatData{
public :
	ChatData():binary(NULL){
	}

	CString sender;
	CString receiver;
	CString message;
	bool is_binary;
	CString file_name;
	BYTE * binary;
};


// CChatDlg 폼 뷰입니다.

class CChatDlg : public CFormView, public FCHttpRequestManager
{
	DECLARE_DYNCREATE(CChatDlg)

protected:
	CChatDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CChatDlg();

public:
	enum { IDD = IDD_CHATDLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

// custom
private:

	std::string lastTime;

	int nTalk;
	ChatData* pchatData;
	virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask);

	void RequestSendMsg();
	void RequestUpdateMsg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CString m_message;
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMenuFrientlist();
	CObArray bubble;
	virtual void OnDraw(CDC* /*pDC*/);
};


