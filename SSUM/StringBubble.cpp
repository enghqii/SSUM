// StringBubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StringBubble.h"


// CStringBubble

CStringBubble::CStringBubble() : CBubble(), m_data(L"")
{	
}

CStringBubble::CStringBubble(CString data) : CBubble(), m_data(data)
{
	int length = m_data.GetLength();
	int width;
	int height = 0;
	int i=0;
	while(1)
	{		
		int size;
		for(size=0;size<15 && i<length;i++)
		{
			if(isascii(m_data.GetAt(i)))
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
	this->height = height*13+12*2;
	this->width = width*10+13*2;
}

CStringBubble::CStringBubble(CString data,UINT align) : CBubble(align), m_data(data)
{
	int length = m_data.GetLength();
	int width;
	int height = 0;
	int i=0;
	while(1)
	{		
		int size;
		for(size=0;size<15 && i<length;i++)
		{
			if(isascii(m_data.GetAt(i)))
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
	this->height = height*13+12*2;
	this->width = width*10+12*2;
}
CStringBubble::~CStringBubble()
{
}


// CStringBubble 멤버 함수
void CStringBubble::onDraw(CDC* pDC)
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
		font.CreatePointFont(100,L"굴림");
		pOldFont = pDC->SelectObject(&font);
		CRect rec;
		rec.left = x+12;
		rec.bottom = y-12;
		rec.top = y-height+12;
		rec.right = x+width-12; 
		//pDC->Rectangle(&rec);
		pDC->DrawTextW(m_data,&rec,DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);
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
		font.CreatePointFont(100,L"굴림");
		pOldFont = pDC->SelectObject(&font);
		CRect rec;
		rec.left = x-width+12;
		rec.bottom = y-12;
		rec.top = y-height+12;
		rec.right = x-12; 
		//pDC->Rectangle(&rec);
		pDC->DrawTextW(m_data,&rec,DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);
		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
}

void CStringBubble::onLClicked(HWND& m_hWnd)
{

}
void CStringBubble::onRClickedView(HWND& m_hWnd)
{
}
void CStringBubble::onRClickedSave(HWND& m_hWnd)
{
}
void CStringBubble::onRClickedSaveAs(HWND& m_hWnd)
{
}