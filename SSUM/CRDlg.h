#pragma once



// CCRDlg �� ���Դϴ�.

class CCRDlg : public CFormView
{
	DECLARE_DYNCREATE(CCRDlg)

protected:
	CCRDlg();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void On32772();
	afx_msg void On32772();
};


