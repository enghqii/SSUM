// TestDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSUM.h"
#include "TestDlg.h"
#include "afxdialogex.h"


// CTestDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestDlg �޽��� ó�����Դϴ�.


void CTestDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}
