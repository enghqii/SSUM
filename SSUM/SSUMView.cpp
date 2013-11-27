
// SSUMView.cpp : CSSUMView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CSSUMView 생성/소멸

CSSUMView::CSSUMView()
	: CFormView(CSSUMView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

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


// CSSUMView 진단

#ifdef _DEBUG
void CSSUMView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSSUMView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSSUMDoc* CSSUMView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSSUMDoc)));
	return (CSSUMDoc*)m_pDocument;
}
#endif //_DEBUG


// CSSUMView 메시지 처리기


void CSSUMView::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

void CSSUMView::OnBnClickedButton1()//Login 버튼 클릭시
{
	// http request를 날려야함.

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

	//우선 무조건 넘어가도록 함
	//CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	//pMain->Set_View(IDD_CRDLG);
}
