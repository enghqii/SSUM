// ChatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include <vector>
#include "SSUM.h"
#include "ChatDlg.h"
#include "UserInfo.h"

#include "../rapidjson/document.h"
#include "file.h"
#include "base64.h"

#include "Bubble.h"
#include "BinaryBubble.h"
#include "ImageBubble.h"
#include "StringBubble.h"

const char URL[] = "http://125.209.197.196/index.php";

// CChatDlg

IMPLEMENT_DYNCREATE(CChatDlg, CFormView)

CChatDlg::CChatDlg()
	: CFormView(CChatDlg::IDD)
{
	m_message = _T("");
	
	lastTime = "2013-01-01 00:00:00"; // get all

	nTalk = 0;
	pchatData = NULL;
}

CChatDlg::~CChatDlg()
{
	if(pchatData != NULL){
		delete [] pchatData;
	}
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, m_message);
}

BEGIN_MESSAGE_MAP(CChatDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_FRIENTLIST, &CChatDlg::OnMenuFrientlist)
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
				//AfxMessageBox(_T("SEND_MSG succeed"));
			}else{
				AfxMessageBox(_T("SEND_MSG failed"));
			}

		}else if(tag.compare("UPDATE_MSG") == 0){

			this->lastTime = doc["lastTime"].GetString();

			nTalk = doc["numberOfTalk"].GetInt();
			pchatData = new ChatData[nTalk];

			for(int i=0;i<nTalk;i++){
				pchatData[i].sender = doc["talk"][i]["sender"].GetString();
				pchatData[i].receiver = doc["talk"][i]["receiver"].GetString();
				pchatData[i].message = doc["talk"][i]["message"].GetString();
				pchatData[i].is_binary = (doc["talk"][i]["is_binary"].GetString()[0] == '0' ? 0 : 1);

				if(pchatData[i].is_binary == true){

					// saving file_name;
					pchatData[i].file_name = doc["talk"][i]["file_name"].GetString();

					// TODO : decode binaries;
					std::string datums = doc["talk"][i]["datums"].GetString();
					std::string decoded = base64_decode(datums);

					pchatData[i].binary = new BYTE[decoded.size()];
					memcpy(pchatData[i].binary, decoded.c_str(), decoded.size());

					CString str;
					CString strName;
					GetTempPath(MAX_PATH,str.GetBuffer(MAX_PATH));
					GetTempFileName(str,L"T_",0,strName.GetBuffer(MAX_PATH));
					pchatData[i].path = strName;
					CFile fp;
					CFileException e;
					if(!fp.Open(strName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary,&e))
					{
						e.ReportError();
						return;
					}

					fp.Write(pchatData[i].binary,decoded.size());

					fp.Close();

					Sleep(0);
				}
				Sleep(0);
			}
			for(int i=nTalk-1;i>=0;i--)
			{
				CBubble *bub;
				if(pchatData[i].is_binary)
				{					
					int count =0;
					int length = pchatData[i].file_name.GetLength();
					for(int j=0;j<length;j++)
					{
						if(pchatData[i].file_name[j] == '.')
						{
							count++;
						}
					}
					CString ext;
					AfxExtractSubString(ext,pchatData[i].file_name,'.');
					//if(ext == L"bmp" || ext == L"jpg" || ext == L"png")
					{
						if(pchatData[i].sender == CUserInfo::shared_info()->getID())
						{
							bub = new CImageBubble(pchatData[i].file_name,pchatData[i].path,CB_RIGHT);
						}
						else
						{
							bub = new CImageBubble(pchatData[i].file_name,pchatData[i].path,CB_LEFT);
						}
					}
				}
				else
				{
					if(pchatData[i].sender == CUserInfo::shared_info()->getID())
					{
						bub = new CStringBubble(pchatData[i].message,CB_RIGHT);
					}
					else
					{
						bub = new CStringBubble(pchatData[i].message,CB_LEFT);
					}
				}
				bubble.Add(bub);
			}
		}
	}else{
		AfxMessageBox(L"received wrong data.");
		// parse failed
	}
	Invalidate();
}

void CChatDlg::RequestSendMsg(){

	UpdateData(true);
	CStringA id(CUserInfo::shared_info()->getID());
	CStringA sender(CUserInfo::shared_info()->getID());
	CStringA receiver(CUserInfo::shared_info()->getTargetID());
	CStringA message(m_message);
	UpdateData(false);

	//std::vector<byte> buf;
	//FCFileEx::Read (_T("c:\\asdf.jpg"), buf) ;
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "SEND_MSG");
	h.AddMultipartFormData("sender", sender);
	h.AddMultipartFormData("receiver", receiver);
	h.AddMultipartFormData("message", message);
	h.AddMultipartFormData("is_binary", "false");
	//h.AddMultipartFormData("is_binary", "true"); // TODO
	//h.AddMultipartFormData("datums", &buf[0], buf.size(), "asdf.jpg") ;
	h.EndMultipartFormData();
	this->AddRequest(h);
}
void CChatDlg::RequestUpdateMsg(){
	
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
	h.AddMultipartFormData("lastTime", lastTime.c_str()); // TODO update this value;
	h.EndMultipartFormData();
	this->AddRequest(h);
}

// CChatDlg 메시지 처리기입니다.

// send_msg
void CChatDlg::OnBnClickedButton1()
{
	RequestSendMsg();
	m_message = L"";
}

/// dummy button
void CChatDlg::OnBnClickedButton2()
{
	RequestUpdateMsg();
}


void CChatDlg::OnInitialUpdate()
{
	__super::OnInitialUpdate();
	OnTimer(1);
	SetTimer(1,1000,NULL);
	SetDlgItemText(IDC_NAME,CUserInfo::shared_info()->getTargetName());
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RequestUpdateMsg();
	__super::OnTimer(nIDEvent);
}


void CChatDlg::OnMenuFrientlist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

}


void CChatDlg::OnDraw(CDC* pDC)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		int pos=bubSize-1;
		CBubble *cur = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		CBubble *next = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		cur->setPosition(200);
		for(;pos>=0;pos--)
		{
			int h = cur->getNextPosition();
			next->setPosition(h);
			cur = next;
			next = reinterpret_cast<CBubble *>(this->bubble[pos]);
		}
		int h = cur->getNextPosition();
		next->setPosition(h);

		for(int i=bubSize-1;i>=0;i--)
		{
			cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			cur->onDraw(pDC);
		}
	}
}
