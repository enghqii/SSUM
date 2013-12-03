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
		// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

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

	// CSSUMView �޽��� ó����

	// JOIN button handler
	void CSSUMView::OnBnClickedButton2()
	{
		CJoinDlg join;
		INT_PTR ptr = join.DoModal();
		if(ptr == IDOK)
		{
			UpdateData(TRUE);
			// TODO : ���⼭ id�� �н�����(�˻�), �̸� �� http request�� ������ ��.

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
		// http request�� ��������.
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