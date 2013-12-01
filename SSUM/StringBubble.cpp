// StringBubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CBubbleTest.h"
#include "StringBubble.h"


// CStringBubble

CStringBubble::CStringBubble() : CBubble(), m_data(L"")
{
	
}

CStringBubble::CStringBubble(int x,int y,CString data) : CBubble(x,y), m_data(data)
{
}

CStringBubble::~CStringBubble()
{
}


// CStringBubble 멤버 함수
void CStringBubble::onDraw(CDC* pDC)
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

	CBrush brush(RGB(250,237,125));
	CBrush *pOldBrush;
	pOldBrush = pDC->SelectObject(&brush);
	pDC->RoundRect(x,y-height*13-12*2,x+width*7+12*2,y,12,12);

	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	
	pDC->SetBkMode(TRANSPARENT);
	CFont font;
	CFont *pOldFont;
	font.CreatePointFont(100,L"굴림");
	pOldFont = pDC->SelectObject(&font);
	i=0;
	int h=0;
	while(1)
	{
		int tmp = i;
		int size;
		int w=0;
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
			w++;
		}
		
		pDC->TextOut(x+12,y-height*13-12+h*13,m_data.Right(length-tmp),w);
		h++;
		if(i>=length)
		{
			break;
		}
	}
	pDC->SelectObject(pOldFont);
	font.DeleteObject();
}

void CStringBubble::onLClicked()
{

}
void CStringBubble::onRClicked()
{

}