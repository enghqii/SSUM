// ImageBubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageBubble.h"


// CImageBubble

CImageBubble::CImageBubble() : CBubble()
{
}
CImageBubble::CImageBubble(CString fileName,CString path) : CBubble(), fileName(fileName), path(path)
{
	CImage image;
	image.Load(path);

	m_data.Attach(image.Detach());
	
	BITMAP bm;
	m_data.GetBitmap(&bm);
	if(bm.bmWidth > 105)
	{
		height = bm.bmHeight*105/bm.bmWidth+12*2;
		width = bm.bmWidth*105/bm.bmWidth+12*2;
	}
	else
	{
		height = bm.bmHeight+12*2;
		width = bm.bmWidth+12*2;
	}
}
CImageBubble::CImageBubble(CString fileName,CString path,UINT align) : CBubble(align), fileName(fileName) ,path(path)
{
	CImage image;
	image.Load(path);

	m_data.Attach(image.Detach());

	BITMAP bm;
	m_data.GetBitmap(&bm);
	if(bm.bmWidth > 105)
	{
		height = bm.bmHeight*105/bm.bmWidth+12*2;
		width = bm.bmWidth*105/bm.bmWidth+12*2;
	}
	else
	{
		height = bm.bmHeight+12*2;
		width = bm.bmWidth+12*2;
	}
}
CImageBubble::~CImageBubble()
{
}


// CImageBubble 멤버 함수
void CImageBubble::onDraw(CDC* pDC)
{
	if(align == CB_LEFT)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_data);
		
		CBrush brush(RGB(255,255,255));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		
		pDC->RoundRect(x,y-height,x+width,y,12,12);
		
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
		
		BITMAP bm;
		m_data.GetBitmap(&bm);
		
		pDC->SetBkMode(TRANSPARENT);
		if(bm.bmWidth > 105)
		{
			pDC->StretchBlt(x+12,y-height+12,
				width-12*2,height-12*2,
				&memDC,0,0,bm.bmWidth,bm.bmHeight,
				SRCCOPY);
		}
		else
		{
			pDC->BitBlt(x+12,y-bm.bmHeight-12,x+bm.bmWidth+12,y-12,&memDC,0,0,SRCCOPY);
		}

		memDC.SelectObject(pOldBitmap);
	}
	else if(align == CB_RIGHT)
	{
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_data);
		
		CBrush brush(RGB(250,237,125));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		
		pDC->RoundRect(x-width,y-height,x,y,12,12);
		
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
		
		BITMAP bm;
		m_data.GetBitmap(&bm);
		
		pDC->SetBkMode(TRANSPARENT);
		if(bm.bmWidth > 105)
		{
			pDC->StretchBlt(x-width+12,y-height+12,
				width-12*2,height-12*2,
				&memDC,0,0,bm.bmWidth,bm.bmHeight,
				SRCCOPY);
		}
		else
		{
			pDC->BitBlt(x-width+12,y-bm.bmHeight-12,x+12,y-12,&memDC,0,0,SRCCOPY);
		}

		memDC.SelectObject(pOldBitmap);
	}
}
void CImageBubble::onLClicked(HWND& m_hWnd)
{
	//AfxMessageBox(path);
	if((HINSTANCE)32 > ShellExecute(m_hWnd,L"open",path,NULL,NULL,SW_SHOW))
	{
		OPENASINFO oai;
		oai.pcszFile = path;
		oai.pcszClass = NULL;
		oai.oaifInFlags = OAIF_HIDE_REGISTRATION|OAIF_EXEC;
		SHOpenWithDialog(m_hWnd,&oai);
	}	
}
void CImageBubble::onRClickedView(HWND& m_hWnd)
{
	if((HINSTANCE)32 > ShellExecute(m_hWnd,L"open",path,NULL,NULL,SW_SHOW))
	{
		OPENASINFO oai;
		oai.pcszFile = path;
		oai.pcszClass = NULL;
		oai.oaifInFlags = OAIF_HIDE_REGISTRATION|OAIF_EXEC;
		SHOpenWithDialog(m_hWnd,&oai);
	}
}
void CImageBubble::onRClickedSave(HWND& m_hWnd)
{
	WCHAR my_document[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL,CSIDL_PROFILE,NULL,SHGFP_TYPE_CURRENT,my_document);
	CString strPathName = L"";
	strPathName += my_document;
	strPathName += L"\\Downloads\\";
	strPathName += fileName;

	CFile fp;
	CFileException e;
	if(!fp.Open(strPathName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary,&e))
	{
		e.ReportError();
		return;
	}
	CFile save;
	if(!save.Open(path,CFile::modeRead|CFile::typeBinary,&e))
	{
		e.ReportError();
		return;
	}

	UINT nRet =0;
	char buffer[1024];

	while(1)
	{
		ZeroMemory(buffer,sizeof(buffer));
		nRet = save.Read(buffer,sizeof(buffer));
		if(nRet == sizeof(buffer))
		{
			fp.Write(buffer,sizeof(buffer));
		}
		else
		{
			fp.Write(buffer,sizeof(buffer));
			break;
		}
	}

	fp.Close();
	save.Close();
}
void CImageBubble::onRClickedSaveAs(HWND& m_hWnd)
{
	CString szFilter = L"Bitmap (*.bmp)|*.bmp|JEPG (*.jpg)|*.jpg|PNG (*.png)|*.png|All Files(*.*)|*.*||";
	CFileDialog dlg(false,L"",L"",OFN_HIDEREADONLY,szFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		CFile fp;
		CFileException e;
		if(!fp.Open(strPathName,CFile::modeWrite|CFile::modeCreate|CFile::typeBinary,&e))
		{
			e.ReportError();
			return;
		}
		CFile save;
		if(!save.Open(path,CFile::modeRead|CFile::typeBinary,&e))
		{
			e.ReportError();
			return;
		}

		UINT nRet =0;
		char buffer[1024];

		while(1)
		{
			ZeroMemory(buffer,sizeof(buffer));
			nRet = save.Read(buffer,sizeof(buffer));
			if(nRet == sizeof(buffer))
			{
				fp.Write(buffer,sizeof(buffer));
			}
			else
			{
				fp.Write(buffer,sizeof(buffer));
				break;
			}
		}

		fp.Close();
		save.Close();
		//CBitmap bitmap;
		//HANDLE bmp;
		//bmp = LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
		//bitmap.Attach(bmp);
		//
		//CImage image;
		//image.Attach(bitmap);
		//image.Save(strPathName,Gdiplus::ImageFormatBMP);
	}
}
void CImageBubble::deletetempfile()
{
	::DeleteFile(path);
}