#pragma once



// CChatDlg �� ���Դϴ�.

class CChatDlg : public CFormView
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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


