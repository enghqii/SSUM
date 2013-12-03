#pragma once

#include "../http-request/lib/http_request_manager.h"

class ChatData{
public :
	ChatData(){}

	CString sender;
	CString receiver;
	CString message;
	bool is_binary;
	CString file_name;
	BYTE * binary;
};


// CChatDlg �� ���Դϴ�.

class CChatDlg : public CFormView, public FCHttpRequestManager
{
	DECLARE_DYNCREATE(CChatDlg)

protected:
	CChatDlg();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	CString m_message;
};


