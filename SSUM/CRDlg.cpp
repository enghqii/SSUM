// CRDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SSUM.h"
#include "CRDlg.h"
#include "MainFrm.h"
#include "../rapidjson/document.h"

const char URL[] = "http://125.209.197.196/index.php";

// CCRDlg

IMPLEMENT_DYNCREATE(CCRDlg, CFormView)

CCRDlg::CCRDlg()
	: CFormView(CCRDlg::IDD)
{
}

CCRDlg::~CCRDlg()
{
	delete [] pstrFriends;
}

void CCRDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCRDlg, CFormView)
//	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_BN_CLICKED(IDC_UPDATE_LIST, &CCRDlg::OnBnClickedUpdateList)
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

			int nFriends = doc["numberOfFriends"].GetInt();
			pstrFriends = new std::string[nFriends];

			for(int i=0;i<nFriends;i++){
				pstrFriends[i] = doc["friends"][i].GetString();
			}

			/* TODO remove this block*/{
				std::string cummulativList = "";
				for(int i=0;i<nFriends;i++){
					cummulativList += (pstrFriends[i]);
				}

				SetDlgItemText(IDC_FRIEND_LIST,CString(cummulativList.c_str()));
			}
		}
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
	
	UpdateData(true);
	CStringA id("asdf");
	UpdateData(false);
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "GET_FRIEND_LIST");
	h.AddMultipartFormData("id", id) ;
	h.EndMultipartFormData();
	this->AddRequest(h);
}
