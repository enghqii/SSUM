#pragma once



// CCRDlg 폼 뷰입니다.

class CCRDlg : public CFormView
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void On32772();
	afx_msg void On32772();
};


