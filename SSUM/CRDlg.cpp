// CRDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SSUM.h"
#include "CRDlg.h"
#include "MainFrm.h"

// CCRDlg

IMPLEMENT_DYNCREATE(CCRDlg, CFormView)

CCRDlg::CCRDlg()
	: CFormView(CCRDlg::IDD)
{

}

CCRDlg::~CCRDlg()
{
}

void CCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCRDlg, CFormView)
//	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_COMMAND(ID_32772, &CCRDlg::On32772)
END_MESSAGE_MAP()


// CCRDlg �����Դϴ�.

#ifdef _DEBUG
void CCRDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCRDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCRDlg �޽��� ó�����Դϴ�.


void CCRDlg::On32772()//�޴��� �α׾ƿ��� �������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_SSUM_FORM);
}
