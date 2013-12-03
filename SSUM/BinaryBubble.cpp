// BinaryBubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "BinaryBubble.h"


// CBinaryBubble

CBinaryBubble::CBinaryBubble() : CBubble()
{
}

CBinaryBubble::CBinaryBubble(CString fileName,CString path) : CBubble(), fileName(fileName), path(path)
{
	int length = fileName.GetLength();
	int width;
	int height = 0;
	int i=0;
	while(1)
	{		
		int size;
		for(size=0;size<15 && i<length;i++)
		{
			if(isascii(fileName.GetAt(i)))
			{
				size+=1;
			}
			else
			{
				size+=2;
			}
		}
		height++;
		if(i>=length)
		{
			if(height>1)
			{
				width=15;
			}
			else
			{
				width=size;
			}
			break;
		}
	}
	this->height = height*16+12*2;
	this->width = width*7+12*2;
}

CBinaryBubble::CBinaryBubble(CString fileName,CString path,UINT align) : CBubble(align), fileName(fileName), path(path)
{
	int length = fileName.GetLength();
	int width;
	int height = 0;
	int i=0;
	while(1)
	{		
		int size;
		for(size=0;size<15 && i<length;i++)
		{
			if(isascii(fileName.GetAt(i)))
			{
				size+=1;
			}
			else
			{
				size+=2;
			}
		}
		height++;
		if(i>=length)
		{
			if(height>1)
			{
				width=15;
			}
			else
			{
				width=size;
			}
			break;
		}
	}
	this->height = height*16+12*2;
	this->width = width*7+12*2;
}
CBinaryBubble::~CBinaryBubble()
{
}


// CBinaryBubble 멤버 함수
void CBinaryBubble::onDraw(CDC* pDC)
{
	if(align == CB_LEFT)
	{
		CBrush brush(RGB(255,255,255));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		pDC->RoundRect(x,y-height,x+width,y,12,12);

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();

		pDC->SetBkMode(TRANSPARENT);
		CFont font;
		CFont *pOldFont;
		font.CreatePointFont(80,L"ARIAL BLACK");
		pOldFont = pDC->SelectObject(&font);
		CRect rec;
		rec.left = x+12;
		rec.bottom = y-12;
		rec.top = y-height+12;
		rec.right = x+width-12; 
		//pDC->Rectangle(&rec);
		pDC->DrawTextW(fileName,&rec,DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);
		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
	else if(align == CB_RIGHT)
	{
		CBrush brush(RGB(250,237,125));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		pDC->RoundRect(x-width,y-height,x,y,12,12);

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();

		pDC->SetBkMode(TRANSPARENT);
		CFont font;
		CFont *pOldFont;
		font.CreatePointFont(80,L"ARIAL BLACK");
		pOldFont = pDC->SelectObject(&font);
		CRect rec;
		rec.left = x-width+12;
		rec.bottom = y-12;
		rec.top = y-height+12;
		rec.right = x-12; 
		//pDC->Rectangle(&rec);
		pDC->DrawTextW(fileName,&rec,DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);
		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
}
void CBinaryBubble::onLClicked(HWND& m_hWnd)
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
void CBinaryBubble::onRClickedView(HWND& m_hWnd)
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
void CBinaryBubble::onRClickedSave(HWND& m_hWnd)
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
void CBinaryBubble::onRClickedSaveAs(HWND& m_hWnd)
{
	CString szFilter = L"All Files(*.*)|*.*|";
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
	}
}