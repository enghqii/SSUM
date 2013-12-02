
// SSUMView.h : CSSUMView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "SSUMDoc.h"

#include "../http-request/lib/http_request_manager.h"

class CSSUMView : public CFormView, public FCHttpRequestManager
{
protected: // serialization에서만 만들어집니다.
	CSSUMView();
	DECLARE_DYNCREATE(CSSUMView)

public:
	enum{ IDD = IDD_SSUM_FORM };

// 특성입니다.
public:
	CSSUMDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSSUMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	CString m_ID;
	CString m_PW;
	CString m_NAME;
	afx_msg void OnBnClickedButton1();

// custom
private:
	void onLoginSuccess();
	virtual void OnAfterRequestSend(FCHttpRequest& rTask);
    virtual void OnAfterRequestFinish (FCHttpRequest& rTask);
};

#ifndef _DEBUG  // SSUMView.cpp의 디버그 버전
inline CSSUMDoc* CSSUMView::GetDocument() const
   { return reinterpret_cast<CSSUMDoc*>(m_pDocument); }
#endif

