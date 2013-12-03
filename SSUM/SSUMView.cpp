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

#include "UserInfo.h"
#include "../rapidjson/document.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace rapidjson;

const char URL[] = "http://125.209.197.196/index.php";

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
		// Window 클래스 또는 스타일을 수정합니다.

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

	// HTTP REQUEST RESPONSE FUNCTIONS -- begin
	void CSSUMView::OnAfterRequestSend (FCHttpRequest& rTask)
	{
	}

	void CSSUMView::OnAfterRequestFinish (FCHttpRequest& rTask)
	{
		// pop received data
		std::string receive_data ;
		rTask.PopReceived (receive_data) ;

		// json parse
		rapidjson::Document doc;
		doc.Parse<0>(receive_data.c_str());

		if(doc.HasParseError() == false){

			// parse successful
			// getting 'tag' value
			std::string tag = doc["tag"].GetString();

			// and dispatch here
			if(tag.compare("LOGIN") == 0){

				int success = doc["success"].GetInt();

				if(success == 1){

					this->m_NAME = doc["name"].GetString();
					CUserInfo::shared_info()->setName(m_NAME);
					CUserInfo::shared_info()->setID(m_ID);

					AfxMessageBox(L"login SUCCESSFUL");
					this->onLoginSuccess();

				}else{
					AfxMessageBox(L"login FAILED");
				}

			}else if(tag.compare("REGISTER") == 0){

				int success = doc["success"].GetInt();

				if(success == 1){
					// TODO : register success
					AfxMessageBox(L"register SUCCESSFUL.");
				}else{
					// TODO : register failed
					AfxMessageBox(L"register FAILED. try again.");
				}
			}

		}else{
			AfxMessageBox(L"received wrong data.");
			// parse failed
		}
	}
	// HTTP REQUEST RESPONSE FUNCTIONS -- end

	void CSSUMView::onLoginSuccess(){
		
		CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
		pMain->Set_View(IDD_CRDLG);
	}

	// CSSUMView 메시지 처리기

	// JOIN button handler
	void CSSUMView::OnBnClickedButton2()
	{
		CJoinDlg join;
		INT_PTR ptr = join.DoModal();
		if(ptr == IDOK)
		{
			UpdateData(TRUE);
			// TODO : 여기서 id랑 패스워드(검사), 이름 을 http request로 보내면 됨.

			CStringA id(join.m_ID);
			CStringA pw(join.m_PW);
			CStringA name(join.m_Name);

			HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
			h.m_url = URL ;
			h.AddMultipartFormData("tag", "REGISTER") ;
			h.AddMultipartFormData("id", id) ;
			h.AddMultipartFormData("password", pw);
			h.AddMultipartFormData("name", name);
			h.EndMultipartFormData();
			this->AddRequest(h);

			UpdateData(FALSE);
		}
	}

	//Login button handler
	void CSSUMView::OnBnClickedButton1()
	{
		// http request를 날려야함.
		UpdateData(true);

		CStringA id(m_ID);
		CStringA password(m_PW);

		UpdateData(false);

		HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
		h.m_url = URL ;
		h.AddMultipartFormData("tag", "LOGIN") ;
		h.AddMultipartFormData("id", id) ;
		h.AddMultipartFormData("password", password);
		h.EndMultipartFormData();
		this->AddRequest(h);
	}