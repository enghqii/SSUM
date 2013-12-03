// ChatDlg.cpp : ���� �����Դϴ�.
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
: CFormView(CChatDlg::IDD) ,stIndex(0),endIndex(0)
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
END_MESSAGE_MAP()


// CChatDlg �����Դϴ�.

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
					strName = str + pchatData[i].file_name;
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
					scrinfo.nMin = 330-10;          // �ּҰ�
					scrinfo.nMax = nMax;      // �ִ밪
					scrinfo.nPage = 20;      // ���������� ������
					scrinfo.nTrackPos = 0;  // Ʈ���ٰ� �����϶��� ��ġ��
					scrinfo.nPos = nMax;        // ��ġ
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

// CChatDlg �޽��� ó�����Դϴ�.

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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;          // �ּҰ�
	scrinfo.nMax = 0;      // �ִ밪
	scrinfo.nPage = 10;      // ���������� ������
	scrinfo.nTrackPos = 0;  // Ʈ���ٰ� �����϶��� ��ġ��
	scrinfo.nPos = 0;        // ��ġ
	m_scroll.SetScrollInfo(&scrinfo);
}


void CChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	RequestUpdateMsg();
	__super::OnTimer(nIDEvent);
}


void CChatDlg::OnMenuFrientlist()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame *pMain=(CMainFrame *)AfxGetMainWnd();
	pMain->Set_View(IDD_CRDLG);
}


void CChatDlg::OnDraw(CDC* pDC)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	CRect rect;
	GetClientRect(&rect);

	// �޸� DC ����
	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	// ȭ�� DC�� ȣȯ�Ǵ� �޸� DC ��ü�� ����
	memDC.CreateCompatibleDC(pDC);

	// ���������� ȭ�� DC�� ȣȯ�Ǵ� Bitmap ����
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
	// �� ����� ����Ϸ��� Windows XP �̻��� �ʿ��մϴ�.
	// _WIN32_WINNT ��ȣ�� 0x0501���� ũ�ų� ���ƾ� �մϴ�.
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	Invalidate();
}


void CChatDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SCROLLINFO  scrinfo;
	if(pScrollBar)
	{
		// ��ũ�� �� �˻�
		if(pScrollBar == (CScrollBar*)&m_scroll)
		{
			// ��ũ�ѹ� ������ �����´�.
			if(pScrollBar->GetScrollInfo(&scrinfo))
			{
				switch(nSBCode)
				{
				case SB_PAGEUP:   // ��ũ�� ���� ���� �ٸ� Ŭ��
					scrinfo.nPos -= scrinfo.nPage;
					break;
				case SB_PAGEDOWN:  // ��ũ�� ���� �Ʒ��� �ٸ� Ŭ��
					scrinfo.nPos += scrinfo.nPage;
					break;
				case SB_LINEUP:   // ��ũ�� ���� ���� ȭ��ǥ�� Ŭ��
					scrinfo.nPos -= scrinfo.nPage/10;
					break;
				case SB_LINEDOWN:  // ��ũ�� ���� �Ʒ��� ȭ��ǥ�� Ŭ��
					scrinfo.nPos += scrinfo.nPage/10;
					break;
				case SB_THUMBPOSITION: // ��ũ�ѹ��� Ʈ���� �����̰� ����
				case SB_THUMBTRACK:  // ��ũ�ѹ��� Ʈ���� �����̴� ����
					scrinfo.nPos = scrinfo.nTrackPos;   // 16bit�� �̻��� ���
					break;
				}
				// ��ũ�ѹ��� ��ġ�� �����Ѵ�.
				pScrollBar->SetScrollPos(scrinfo.nPos);
			}
		}
	}
	else
	{
		// �� �κ��� Scroll����� �ִ� �並 ���� ���ȴ�.
		int ScrollPos = GetScrollPos(SB_VERT); // ���� ��ũ�ѹ� ������ ���ϱ�
		// ���δ�: SB_HORZ

		switch(nSBCode)
		{
		case SB_PAGEUP:        // ��ũ�� ���� ���� �ٸ� Ŭ��
		case SB_PAGEDOWN:  // ��ũ�� ���� �Ʒ��� �ٸ� Ŭ��
		case SB_LINEUP:         // ��ũ�� ���� ���� ȭ��ǥ�� Ŭ��
		case SB_LINEDOWN:    // ��ũ�� ���� �Ʒ��� ȭ��ǥ�� Ŭ��
			CChatDlg::OnVScroll(nSBCode, nPos, pScrollBar);
			break;
		case SB_THUMBPOSITION:  // ��ũ�ѹ��� Ʈ���� �����̰� ����
		case SB_THUMBTRACK:      // ��ũ�ѹ��� Ʈ���� �����̴� ����
			scrinfo.nPos = scrinfo.nTrackPos;
			// ��ũ�ѹ��� ��ġ�� �����Ѵ�.
			SetScrollPos(SB_VERT, scrinfo.nPos);
			Invalidate(FALSE);
			break;
			}
		}
		Invalidate();
		//CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CChatDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	Invalidate();
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}


void CChatDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	ClientToScreen(&point);
	OnContextMenu(this, point);
	__super::OnRButtonUp(nFlags, point);
}


void CChatDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	RequestSendFile();
}
