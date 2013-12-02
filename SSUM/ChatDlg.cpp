// ChatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SSUM.h"
#include "ChatDlg.h"
#include "UserInfo.h"
#include "../rapidjson/document.h"

const char URL[] = "http://125.209.197.196/index.php";

// CChatDlg

IMPLEMENT_DYNCREATE(CChatDlg, CFormView)

CChatDlg::CChatDlg()
	: CFormView(CChatDlg::IDD)
{
	m_message = _T("");
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_message);
}

BEGIN_MESSAGE_MAP(CChatDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CChatDlg 진단입니다.

#ifdef _DEBUG
void CChatDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CChatDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// HTTP REQUEST RESPONSE FUNCTIONS -- begin
void CChatDlg::OnAfterRequestSend (FCHttpRequest& rTask)
{
}

void CChatDlg::OnAfterRequestFinish (FCHttpRequest& rTask)
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
		if(tag.compare("SEND_MSG") == 0){
			int success = doc["success"].GetInt();

			if(success == 1){
				AfxMessageBox(_T("SEND_MSG succeed"));
			}else{
				AfxMessageBox(_T("SEND_MSG failed"));
			}
		}else if(tag.compare("UPDATE_MSG") == 0){
			
			int nTalk = doc["numberOfTalk"].GetInt();
			pchatData = new ChatData[nTalk];

			for(int i=0;i<nTalk;i++){
				pchatData[i].sender = doc["talk"][i]["sender"].GetString();
				pchatData[i].receiver = doc["talk"][i]["receiver"].GetString();
				pchatData[i].message = doc["talk"][i]["message"].GetString();
				pchatData[i].is_binary = (doc["talk"][i]["is_binary"].GetString()[0] == '0' ? 0 : 1);
				Sleep(0);
			}

			// remove this block
			{
				std::string cummulativList = "==";
				CT2CA pszConvertedAnsiString(CUserInfo::shared_info()->getTargetID());
				std::string s(pszConvertedAnsiString);
				cummulativList += s;
				cummulativList += " \n";

				for(int i=0;i<nTalk;i++){
					cummulativList += " \n";
					
					if(pchatData[i].sender == CUserInfo::shared_info()->getID()){
						cummulativList += ">";
					}else{
						cummulativList += "*";
					}
					
					CT2CA _(pchatData[i].message);
					std::string __(_);
					cummulativList += __;
				}

				SetDlgItemText(IDC_DUMMYSTATIC,CString(cummulativList.c_str()));
			}
		}
	}
}

// CChatDlg 메시지 처리기입니다.

// send_msg
void CChatDlg::OnBnClickedButton1()
{
	UpdateData(true);
	CStringA id(CUserInfo::shared_info()->getID());
	CStringA sender(CUserInfo::shared_info()->getID());
	CStringA receiver(CUserInfo::shared_info()->getTargetID());
	CStringA message(m_message);
	UpdateData(false);
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "SEND_MSG");
	h.AddMultipartFormData("sender", sender);
	h.AddMultipartFormData("receiver", receiver);
	h.AddMultipartFormData("message", message);
	h.AddMultipartFormData("is_binary", false);
	h.AddMultipartFormData("binary", NULL);
	h.EndMultipartFormData();
	this->AddRequest(h);
}

/// dummy button
void CChatDlg::OnBnClickedButton2()
{
	//SetDlgItemText(IDC_DUMMYSTATIC,CUserInfo::shared_info()->getTargetID());

	UpdateData(true);
	CStringA id(CUserInfo::shared_info()->getID());
	CStringA sender(CUserInfo::shared_info()->getID());
	CStringA receiver(CUserInfo::shared_info()->getTargetID());
	CStringA message(m_message);
	UpdateData(false);
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "UPDATE_MSG");
	h.AddMultipartFormData("sender", sender);
	h.AddMultipartFormData("receiver", receiver);
	h.AddMultipartFormData("lastTime", "2013-01-01 00:00:00");
	h.EndMultipartFormData();
	this->AddRequest(h);
}
