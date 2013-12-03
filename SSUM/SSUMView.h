
// SSUMView.h : CSSUMView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "SSUMDoc.h"

#include "../http-request/lib/http_request_manager.h"

class CSSUMView : public CFormView, public FCHttpRequestManager
{
protected: // serialization������ ��������ϴ�.
	CSSUMView();
	DECLARE_DYNCREATE(CSSUMView)

public:
	enum{ IDD = IDD_SSUM_FORM };

// Ư���Դϴ�.
public:
	CSSUMDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CSSUMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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
	virtual void OnDraw(CDC* /*pDC*/);
public:
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // SSUMView.cpp�� ����� ����
inline CSSUMDoc* CSSUMView::GetDocument() const
   { return reinterpret_cast<CSSUMDoc*>(m_pDocument); }
#endif

