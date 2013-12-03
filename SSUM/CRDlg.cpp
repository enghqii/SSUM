// CRDlg.cpp : ���� �����Դϴ�.
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
	, pstrFriends(NULL)
{
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
}

BEGIN_MESSAGE_MAP(CCRDlg, CFormView)
//	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_COMMAND(ID_32772, &CCRDlg::On32772)
	ON_BN_CLICKED(IDC_UPDATE_LIST, &CCRDlg::OnBnClickedUpdateList)
	ON_BN_CLICKED(IDC_CHAT1, &CCRDlg::OnBnClickedChat1)
	ON_BN_CLICKED(IDC_CHAT2, &CCRDlg::OnBnClickedChat2)
	ON_BN_CLICKED(IDC_CHAT3, &CCRDlg::OnBnClickedChat3)
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
			pstrFriendsID = new std::string[nFriends];

			for(int i=0;i<nFriends;i++){
				pstrFriends[i] = doc["friends"][i].GetString();
				pstrFriendsID[i] = doc["friendsID"][i].GetString();
			}

			{
				std::string cummulativList = "";
				
				cummulativList += "Hello, ";
				CT2CA pszConvertedAnsiString(CUserInfo::shared_info()->getName());
				std::string s(pszConvertedAnsiString);
				cummulativList += s;
				cummulativList += " ( id = ";
				CT2CA pszConvertedAnsiString1(CUserInfo::shared_info()->getID());
				std::string s1(pszConvertedAnsiString1);
				cummulativList += s1;
				cummulativList += " ) ";
				
				cummulativList += " \n\n*friend List*\n";

				for(int i=0;i<nFriends;i++){
					cummulativList += (pstrFriends[i]);
					cummulativList += " \n";
				}

				SetDlgItemText(IDC_FRIEND_LIST,CString(cummulativList.c_str()));
			}/* TODO remove this block*/
		}
	}else{
		AfxMessageBox(L"received wrong data.");
		// parse failed
	}
}


// CCRDlg �޽��� ó�����Դϴ�.


void CCRDlg::On32772()//�޴��� �α׾ƿ��� �������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_SSUM_FORM);
}


void CCRDlg::OnBnClickedUpdateList()
{
	// TODO : ģ�� ��� ������ ������Ʈ ������
	
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
