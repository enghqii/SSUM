#pragma once


// CJoinDlg ��ȭ �����Դϴ�.

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJoinDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_JOIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedOk();
	afx_msg void OnClickedCancel();
	CString m_ID;
	CString m_Name;
//	CEdit m_PW;
	CString m_PW;
	CString m_PWC;
};
