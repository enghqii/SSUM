
// SSUMView.cpp : CSSUMView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "SSUM.h"
#endif

#include "SSUMDoc.h"
#include "SSUMView.h"
#include "JoinDlg.h"

#include "TestDlg.h"
#include "CRDlg.h"
#include "MainFrm.h"
#include "../http-request/lib/http_request_manager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSSUMView

IMPLEMENT_DYNCREATE(CSSUMView, CFormView)

BEGIN_MESSAGE_MAP(CSSUMView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON2, &CSSUMView::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSSUMView::OnBnClickedButton1)
END_MESSAGE_MAP()

// CSSUMView ����/�Ҹ�

CSSUMView::CSSUMView()
	: CFormView(CSSUMView::IDD)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	m_ID = _T("");
	m_PW = _T("");
}

CSSUMView::~CSSUMView()
{
}

void CSSUMView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID, m_ID);
	DDX_Text(pDX, IDC_PW, m_PW);
}

BOOL CSSUMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CSSUMView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CSSUMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSSUMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSSUMView ����

#ifdef _DEBUG
void CSSUMView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSSUMView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSSUMDoc* CSSUMView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSSUMDoc)));
	return (CSSUMDoc*)m_pDocument;
}
#endif //_DEBUG


// CSSUMView �޽��� ó����


void CSSUMView::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CJoinDlg join;
	if(join.DoModal() == ID_OK)
	{
		UpdateData(TRUE);
		
		UpdateData(FALSE);
	}
}

class CMyManager : public FCHttpRequestManager {

	virtual void OnAfterRequestSend (FCHttpRequest& rTask)
    {
		Beep(300,10);
    }

    virtual void OnAfterRequestFinish (FCHttpRequest& rTask)
    {
		Beep(900,10);
    }
};

CMyManager httpMgr;

void CSSUMView::OnBnClickedButton1()//Login ��ư Ŭ����
{
	// http request�� ��������.

	UpdateData(true);

	CString id = m_ID;
	CString password = m_PW;

	UpdateData(false); 

	HTTP_REQUEST_HEADER   h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = _T("http://125.209.197.196/index.php") ;
	h.AddMultipartFormData("tag", "LOGIN") ;
	h.AddMultipartFormData("id", "enghqii") ;
	h.AddMultipartFormData("password", "1234");
	h.EndMultipartFormData();
	httpMgr.AddRequest(h);

	//�켱 ������ �Ѿ���� ��
	//CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	//pMain->Set_View(IDD_CRDLG);
}
