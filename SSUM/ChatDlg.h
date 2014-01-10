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
	CString path;
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
	void RequestSendFile();
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
	int stIndex,endIndex;
	void SetPosition(void);
	CScrollBar m_scroll;
	afx_msg void OnNMThemeChangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	CPoint np;
	afx_msg void OnChatpopupView();
	afx_msg void OnChatpopupSave();
	afx_msg void OnChatpopupSaveas();
	afx_msg void OnBnClickedFile();
	int tnum;
	afx_msg void OnDestroy();

	//virtual void OnInitialUpdate();
};


