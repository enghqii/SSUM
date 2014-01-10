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
#include "MainFrm.h"

const char URL[] = "http://125.209.197.196/index.php";

// CChatDlg

IMPLEMENT_DYNCREATE(CChatDlg, CFormView)

CChatDlg::CChatDlg()
: CFormView(CChatDlg::IDD) ,stIndex(0),endIndex(0),tnum(0)
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
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scroll);
}

BEGIN_MESSAGE_MAP(CChatDlg, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CChatDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_COMMAND(ID_MENU_FRIENTLIST, &CChatDlg::OnMenuFrientlist)
	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CChatDlg::OnNMThemeChangedScrollbar1)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHATPOPUP_VIEW, &CChatDlg::OnChatpopupView)
	ON_COMMAND(ID_CHATPOPUP_SAVE, &CChatDlg::OnChatpopupSave)
	ON_COMMAND(ID_CHATPOPUP_SAVEAS, &CChatDlg::OnChatpopupSaveas)
	ON_BN_CLICKED(IDC_FILE, &CChatDlg::OnBnClickedFile)
	ON_WM_DESTROY()	
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

					CString strName;
					strName.Format(L"T_%d",tnum++);
					strName +=pchatData[i].file_name;

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
					AfxExtractSubString(ext,pchatData[i].file_name,count,'.');
					if(ext == L"bmp" || ext == L"jpg" || ext == L"png")
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
					else
					{
						if(pchatData[i].sender == CUserInfo::shared_info()->getID())
						{
							bub = new CBinaryBubble(pchatData[i].file_name,pchatData[i].path,CB_RIGHT);
						}
						else
						{
							bub = new CBinaryBubble(pchatData[i].file_name,pchatData[i].path,CB_LEFT);
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
				if(i == 0)
				{
					int nMax=0;
					CBubble * bub;
					for(int i=0;i<bubble.GetSize();i++)
					{
						bub = reinterpret_cast<CBubble *>(this->bubble[i]);
						nMax += bub->getHeight() + 10;
					}
					SCROLLINFO scrinfo;
					scrinfo.cbSize = sizeof(scrinfo);
					scrinfo.fMask = SIF_ALL;
					scrinfo.nMin = 330-10;          // 최소값
					scrinfo.nMax = nMax;      // 최대값
					scrinfo.nPage = 20;      // 페이지단위 증가값
					scrinfo.nTrackPos = 0;  // 트랙바가 움직일때의 위치값
					scrinfo.nPos = nMax;        // 위치
					m_scroll.SetScrollInfo(&scrinfo);
					Invalidate();
				}
			}
		}
	}else{
		AfxMessageBox(L"received wrong data.");
		// parse failed
	}
}

void CChatDlg::RequestSendMsg(){

	UpdateData(true);
	CStringA id(CUserInfo::shared_info()->getID());
	CStringA sender(CUserInfo::shared_info()->getID());
	CStringA receiver(CUserInfo::shared_info()->getTargetID());
	CStringA message(m_message);
	m_message = L"";
	UpdateData(false);

	std::vector<byte> buf;
	FCFileEx::Read (_T("hjkl.jpg"), buf) ;
	
	HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
	h.m_url = URL ;
	h.AddMultipartFormData("tag", "SEND_MSG");
	h.AddMultipartFormData("sender", sender);
	h.AddMultipartFormData("receiver", receiver);
	h.AddMultipartFormData("message", message);
	h.AddMultipartFormData("is_binary", "false");
	//h.AddMultipartFormData("is_binary", "true"); // TODO
	//h.AddMultipartFormData("datums", &buf[0], buf.size(), "hjkl.jpg") ;
	h.EndMultipartFormData();
	this->AddRequest(h);
}
void CChatDlg::RequestSendFile(){
	UpdateData(true);
	CStringA id(CUserInfo::shared_info()->getID());
	CStringA sender(CUserInfo::shared_info()->getID());
	CStringA receiver(CUserInfo::shared_info()->getTargetID());
	CStringA message(m_message);
	m_message = L"";
	UpdateData(false);
	CString szFilter = L"Bitmap (*.bmp)|*.bmp|JEPG (*.jpg)|*.jpg|PNG (*.png)|*.png|PDF (*.pdf)|*.pdf|Word (*.doc;*.docx;)|*.doc;*.docx;|PowerPoint (*.ppt;*.pptx;)|*.ppt;*.pptx;|All Files(*.*)|*.*||";
	CFileDialog dlg(true,L"",L"",OFN_HIDEREADONLY,szFilter);
	CString strPathName;
	CString strfilename;
	if(dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
		strfilename = dlg.GetFileName();

		std::vector<byte> buf;
		FCFileEx::Read (strPathName, buf) ;

		HTTP_REQUEST_HEADER h (HTTP_REQUEST_HEADER::VERB_TYPE_POST_MULTIPART);
		h.m_url = URL ;
		h.AddMultipartFormData("tag", "SEND_MSG");
		h.AddMultipartFormData("sender", sender);
		h.AddMultipartFormData("receiver", receiver);
		h.AddMultipartFormData("message", message);
		//h.AddMultipartFormData("is_binary", "false");
		h.AddMultipartFormData("is_binary", "true"); // TODO
		h.AddMultipartFormData("datums", &buf[0], buf.size(), (CStringA)strfilename) ;
		h.EndMultipartFormData();
		this->AddRequest(h);
	}
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
}

/// dummy button
void CChatDlg::OnBnClickedButton2()
{
	RequestUpdateMsg();
}


<<<<<<< HEAD
void CChatDlg::OnInitialUpdate()
{
	__super::OnInitialUpdate();
	OnTimer(1);
	SetTimer(1,1500,NULL);
	CString tt;
	tt = L"   ";
	tt += CUserInfo::shared_info()->getName();
	tt += L"   ->   ";
	tt += CUserInfo::shared_info()->getTargetName();
	SetDlgItemText(IDC_NAME,tt);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;          // 최소값
	scrinfo.nMax = 0;      // 최대값
	scrinfo.nPage = 10;      // 페이지단위 증가값
	scrinfo.nTrackPos = 0;  // 트랙바가 움직일때의 위치값
	scrinfo.nPos = 0;        // 위치
	m_scroll.SetScrollInfo(&scrinfo);
}


=======
>>>>>>> gettingFriendList
void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RequestUpdateMsg();
<<<<<<< HEAD
=======

>>>>>>> gettingFriendList
	__super::OnTimer(nIDEvent);
}


<<<<<<< HEAD
void CChatDlg::OnMenuFrientlist()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int size = bubble.GetSize();
	for(int i=0;i<size;i++)
	{
		CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
		cur->deletetempfile();		
	}

	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CRDLG);
}


void CChatDlg::OnDraw(CDC* pDC)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	CRect rect;
	GetClientRect(&rect);

	// 메모리 DC 선언
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// 화면 DC와 호환되는 메모리 DC 객체를 생성
	memDC.CreateCompatibleDC(pDC);

	// 마찬가지로 화면 DC와 호환되는 Bitmap 생성
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);

	//print back ground to memDC
	CBitmap back;
	back.LoadBitmapW(IDB_BITMAP1);

	CDC bkDC;
	bkDC.CreateCompatibleDC(&memDC);
	CBitmap *pOldbkBitmap = bkDC.SelectObject(&back);

	BITMAP bm;
	back.GetBitmap(&bm);

	memDC.StretchBlt(0,0,
		rect.Width(),rect.Height(),
		&bkDC,0,0,bm.bmWidth,bm.bmHeight,
		SRCCOPY);

	bkDC.SelectObject(pOldbkBitmap);

	//print bubble to memDC
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		int pos=bubSize-1;

		SetPosition();

		for(int i=stIndex;i>=endIndex;i--)
		{
			CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			cur->onDraw(&memDC);
		}
	}

	//print mDC to pDC
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}


void CChatDlg::SetPosition(void)
{
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		int pos=bubSize-1;
		CBubble *cur = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		//CBubble *next = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		CScrollBar* pScrollBar;
		pScrollBar = (CScrollBar*)&m_scroll;
		SCROLLINFO  scrinfo;
		pScrollBar->GetScrollInfo(&scrinfo);

		cur->setPosition(330+(scrinfo.nMax-scrinfo.nPos));
		int h;
		while(1)
		{
			h = cur->getNextPosition();
			if(pos < 0 || h < 330)
			{
				stIndex = pos+1;
				break;
			}
			cur = reinterpret_cast<CBubble *>(this->bubble[pos--]);
			cur->setPosition(h);
			//next->setPosition(h);
			//cur = next;
			//next = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		}
		while(1)
		{
			h = cur->getNextPosition();
			if(pos < 0 || h < 0)
			{
				endIndex = pos+1;
				break;
			}
			cur = reinterpret_cast<CBubble *>(this->bubble[pos--]);
			cur->setPosition(h);
			//next->setPosition(h);
			//cur = next;
			//next = reinterpret_cast<CBubble *>(this->bubble[pos--]);
		}
	}
}


void CChatDlg::OnNMThemeChangedScrollbar1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 이 기능을 사용하려면 Windows XP 이상이 필요합니다.
	// _WIN32_WINNT 기호는 0x0501보다 크거나 같아야 합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	//Invalidate();
}


void CChatDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO  scrinfo;
	if(pScrollBar)
	{
		// 스크롤 바 검사
		if(pScrollBar == (CScrollBar*)&m_scroll)
		{
			// 스크롤바 정보를 가져온다.
			if(pScrollBar->GetScrollInfo(&scrinfo))
			{
				switch(nSBCode)
				{
				case SB_PAGEUP:   // 스크롤 바의 위쪽 바를 클릭
					scrinfo.nPos -= scrinfo.nPage;
					break;
				case SB_PAGEDOWN:  // 스크롤 바의 아래쪽 바를 클릭
					scrinfo.nPos += scrinfo.nPage;
					break;
				case SB_LINEUP:   // 스크롤 바의 위쪽 화살표를 클릭
					scrinfo.nPos -= scrinfo.nPage/10;
					break;
				case SB_LINEDOWN:  // 스크롤 바의 아래쪽 화살표를 클릭
					scrinfo.nPos += scrinfo.nPage/10;
					break;
				case SB_THUMBPOSITION: // 스크롤바의 트랙이 움직이고 나서
				case SB_THUMBTRACK:  // 스크롤바의 트랙이 움직이는 동안
					scrinfo.nPos = scrinfo.nTrackPos;   // 16bit값 이상을 사용
					break;
				}
				// 스크롤바의 위치를 변경한다.
				pScrollBar->SetScrollPos(scrinfo.nPos);
				Invalidate(FALSE);
			}
		}
	}
	else
	{
		// 이 부분은 Scroll기능이 있는 뷰를 사용시 사용된다.
		int ScrollPos = GetScrollPos(SB_VERT); // 세로 스크롤바 포지션 구하기
		// 가로는: SB_HORZ

		switch(nSBCode)
		{
		case SB_PAGEUP:        // 스크롤 바의 위쪽 바를 클릭
		case SB_PAGEDOWN:  // 스크롤 바의 아래쪽 바를 클릭
		case SB_LINEUP:         // 스크롤 바의 위쪽 화살표를 클릭
		case SB_LINEDOWN:    // 스크롤 바의 아래쪽 화살표를 클릭
			CChatDlg::OnVScroll(nSBCode, nPos, pScrollBar);
			break;
		case SB_THUMBPOSITION:  // 스크롤바의 트랙이 움직이고 나서
		case SB_THUMBTRACK:      // 스크롤바의 트랙이 움직이는 동안
			scrinfo.nPos = scrinfo.nTrackPos;
			// 스크롤바의 위치를 변경한다.
			SetScrollPos(SB_VERT, scrinfo.nPos);
			Invalidate(FALSE);
			break;
			}
		}
		//CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CChatDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollBar* pScrollBar;
	pScrollBar = (CScrollBar*)&m_scroll;
	SCROLLINFO  scrinfo;
	pScrollBar->GetScrollInfo(&scrinfo);
	if(zDelta>0)
	{
		scrinfo.nPos -= scrinfo.nPage;
	}
	if(zDelta<0)
	{
		scrinfo.nPos += scrinfo.nPage;
	}
	pScrollBar->SetScrollPos(scrinfo.nPos);
	Invalidate(FALSE);
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void CChatDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		for(int i=stIndex;i>=endIndex;i--)
		{
			CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			if(cur->isBLocate(point))
			{
				cur->onLClicked(m_hWnd);
			}
		}
	}
	__super::OnLButtonUp(nFlags, point);
}


void CChatDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	ClientToScreen(&point);
	OnContextMenu(this, point);
	__super::OnRButtonUp(nFlags, point);
}


void CChatDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu mymenu;
	mymenu.LoadMenu(IDR_POPUP_EDIT);

	CMenu *pContextMenu = mymenu.GetSubMenu(1);

	CRect winRect;
	GetWindowRect(&winRect);

	CPoint newPoint;
	newPoint.x = point.x - winRect.left;
	newPoint.y = point.y - winRect.top;
	np = newPoint;

	if(newPoint.y > 380)
	{
		//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}
	else
	{
		int bubSize = bubble.GetSize();
		if(bubSize)
		{
			for(int i=stIndex;i>=endIndex;i--)
			{
				CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
				if(cur->isBLocate(newPoint))
				{
					pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTALIGN,point.x,point.y,this);
				}
			}
		}
	}
}


void CChatDlg::OnChatpopupView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		for(int i=stIndex;i>=endIndex;i--)
		{
			CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			if(cur->isBLocate(np))
			{
				cur->onRClickedView(m_hWnd);
			}
		}
	}
}


void CChatDlg::OnChatpopupSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		for(int i=stIndex;i>=endIndex;i--)
		{
			CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			if(cur->isBLocate(np))
			{
				cur->onRClickedSave(m_hWnd);
			}
		}
	}
}


void CChatDlg::OnChatpopupSaveas()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int bubSize = bubble.GetSize();
	if(bubSize)
	{
		for(int i=stIndex;i>=endIndex;i--)
		{
			CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
			if(cur->isBLocate(np))
			{
				cur->onRClickedSaveAs(m_hWnd);
			}
		}
	}
}


void CChatDlg::OnBnClickedFile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RequestSendFile();
}

void CChatDlg::OnDestroy()
{
	__super::OnDestroy();
	int size = bubble.GetSize();
	for(int i=0;i<size;i++)
	{
		 CBubble* cur = reinterpret_cast<CBubble *>(this->bubble[i]);
		 cur->deletetempfile();		
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
=======
void CChatDlg::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	SetTimer(0,1000,NULL);
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
>>>>>>> gettingFriendList
}
