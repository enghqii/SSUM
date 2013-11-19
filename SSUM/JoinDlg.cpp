// JoinDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSUM.h"
#include "JoinDlg.h"
#include "afxdialogex.h"


// CJoinDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CJoinDlg, CDialogEx)

CJoinDlg::CJoinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJoinDlg::IDD, pParent)
{

	m_ID = _T("");
	m_Name = _T("");
	m_PW = _T("");
	m_PWC = _T("");
}

CJoinDlg::~CJoinDlg()
{
}

void CJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID, m_ID);
	DDX_Text(pDX, IDC_NAME, m_Name);
	//  DDX_Control(pDX, IDC_PW, m_PW);
	DDX_Text(pDX, IDC_PW, m_PW);
	DDX_Text(pDX, IDC_PWC, m_PWC);
}


BEGIN_MESSAGE_MAP(CJoinDlg, CDialogEx)
	ON_BN_CLICKED(ID_OK, &CJoinDlg::OnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &CJoinDlg::OnClickedCancel)
END_MESSAGE_MAP()


// CJoinDlg �޽��� ó�����Դϴ�.


void CJoinDlg::OnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if(m_PW!=m_PWC)
	{
		AfxMessageBox(L"");
		m_PW=L"";
		m_PWC=L"";
		UpdateData(FALSE);
	}
	else
	{
		OnOK();
		UpdateData(FALSE);
	}
}


void CJoinDlg::OnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
