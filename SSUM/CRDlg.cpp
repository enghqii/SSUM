// CRDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SSUM.h"
#include "CRDlg.h"
#include "MainFrm.h"

#include "UserInfo.h"
#include "../rapidjson/document.h"

const char URL[] = "http://125.209.197.196/index.php";

// CCRDlg

IMPLEMENT_DYNCREATE(CCRDlg, CFormView)

CCRDlg::CCRDlg()
	: CFormView(CCRDlg::IDD)
	, pstrFriends(NULL){
	this->m_name = CUserInfo::shared_info()->getName();
	this->m_id = CUserInfo::shared_info()->getID();
	Sleep(10);
}

CCRDlg::~CCRDlg()
{
	if(pstrFriends != NULL)
		delete [] pstrFriends;
}

void CCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CCRDlg, CFormView)
//	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_BN_CLICKED(IDC_UPDATE_LIST, &CCRDlg::OnBnClickedUpdateList)
	ON_BN_CLICKED(IDC_CHAT1, &CCRDlg::OnBnClickedChat1)
	ON_BN_CLICKED(IDC_CHAT2, &CCRDlg::OnBnClickedChat2)
	ON_BN_CLICKED(IDC_CHAT3, &CCRDlg::OnBnClickedChat3)
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST, &CCRDlg::OnLbnDblclkList)
END_MESSAGE_MAP()


// CCRDlg 진단입니다.

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


// HTTP REQUEST RESPONSE FUNCTIONS -- begin
void CCRDlg::OnAfterRequestSend (FCHttpRequest& rTask)
{
}

void CCRDlg::OnAfterRequestFinish (FCHttpRequest& rTask)
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
		if(tag.compare("GET_FRIEND_LIST") == 0){

			nFriends = doc["numberOfFriends"].GetInt();
			pstrFriends = new std::string[nFriends];
			pstrFriendsID = new std::string[nFriends];

			for(int i=0;i<nFriends;i++){
				pstrFriends[i] = doc["friends"][i].GetString();
				pstrFriendsID[i] = doc["friendsID"][i].GetString();
			}

			//
			m_List.ResetContent();
			for(int i=0;i<nFriends;i++)
			{
				CString temp(pstrFriends[i].c_str());
				m_List.AddString(temp);
			}

		}
	}else{
		AfxMessageBox(L"received wrong data.");
		// parse failed
	}
}


// CCRDlg 메시지 처리기입니다.


void CCRDlg::On32772()//메뉴의 로그아웃을 누를경우
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_SSUM_FORM);
}


void CCRDlg::OnBnClickedUpdateList()
{
	// TODO : 친구 목록 얻어오는 리퀘스트 보내기
	
	CStringA id(CUserInfo::shared_info()->getID());
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "GET_FRIEND_LIST");
	h.AddMultipartFormData("id", id) ;
	h.EndMultipartFormData();
	this->AddRequest(h);
}

// dummy below

void CCRDlg::OnBnClickedChat1() // goto chat room
{
	CUserInfo::shared_info()->setTargetName(CString(pstrFriends[0].c_str()));
	CUserInfo::shared_info()->setTargetID(CString(pstrFriendsID[0].c_str()));
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CHATDLG);
}


void CCRDlg::OnBnClickedChat2()
{
	CUserInfo::shared_info()->setTargetName(CString(pstrFriends[1].c_str()));
	CUserInfo::shared_info()->setTargetID(CString(pstrFriendsID[1].c_str()));
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CHATDLG);
}


void CCRDlg::OnBnClickedChat3()
{
	CUserInfo::shared_info()->setTargetName(CString(pstrFriends[2].c_str()));
	CUserInfo::shared_info()->setTargetID(CString(pstrFriendsID[2].c_str()));
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CHATDLG);
}


void CCRDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO : 친구 목록 얻어오는 리퀘스트 보내기
	
	CStringA id(CUserInfo::shared_info()->getID());
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "GET_FRIEND_LIST");
	h.AddMultipartFormData("id", id) ;
	h.EndMultipartFormData();
	this->AddRequest(h);
	__super::OnTimer(nIDEvent);
}


void CCRDlg::OnInitialUpdate()
{
	__super::OnInitialUpdate();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	OnTimer(0);
	SetTimer(0,100000,NULL);

	CString tt;
	tt = L"Hello, ";
	tt += CUserInfo::shared_info()->getName();
	SetDlgItemText(IDC_NAME,tt);
}


void CCRDlg::OnDraw(CDC* pDC)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CRect rect;
	GetClientRect(&rect);

	CBitmap back;
	back.LoadBitmapW(IDB_BITMAP1);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = memDC.SelectObject(&back);

	BITMAP bm;
	back.GetBitmap(&bm);

	pDC->StretchBlt(0,0,
		rect.Width(),rect.Height(),
		&memDC,0,0,bm.bmWidth,bm.bmHeight,
		SRCCOPY);

	memDC.SelectObject(pOldBitmap);
}


void CCRDlg::OnLbnDblclkList()
{
	int cur = m_List.GetCurSel();
	CUserInfo::shared_info()->setTargetName(CString(pstrFriends[cur].c_str()));
	CUserInfo::shared_info()->setTargetID(CString(pstrFriendsID[cur].c_str()));

	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CHATDLG);
}
