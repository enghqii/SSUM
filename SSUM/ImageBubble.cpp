// ImageBubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CBubbleTest.h"
#include "ImageBubble.h"


// CImageBubble

CImageBubble::CImageBubble() : CBubble()
{
}
CImageBubble::CImageBubble(CString path) : CBubble()
{
	HANDLE bmp;
	bmp = LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	m_data.Attach(bmp);
}
CImageBubble::~CImageBubble()
{
}


// CImageBubble 멤버 함수
void CImageBubble::onDraw(CDC* pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = memDC.SelectObject(&m_data);
	BITMAP bm;
	m_data.GetBitmap(&bm);
	if(bm.bmWidth > 105)
	{
		CBrush brush(RGB(250,237,125));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		pDC->RoundRect(x,y-bm.bmHeight*105/bm.bmWidth-12*2,x+bm.bmWidth*105/bm.bmWidth+12*2,y,12,12);
		height = bm.bmHeight*105/bm.bmWidth+12*2;

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->StretchBlt(x+12,y-bm.bmHeight*105/bm.bmWidth-12,
			bm.bmWidth*105/bm.bmWidth,bm.bmHeight*105/bm.bmWidth,
			&memDC,0,0,bm.bmWidth,bm.bmHeight,
			SRCCOPY);
	}
	else
	{
		CBrush brush(RGB(250,237,125));
		CBrush *pOldBrush;
		pOldBrush = pDC->SelectObject(&brush);
		pDC->RoundRect(x,y-bm.bmHeight-12*2,x+bm.bmWidth+12*2,y,12,12);
		height = bm.bmHeight+12*2;
		int test = y-height;

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->BitBlt(x+12,y-bm.bmHeight-12,x+bm.bmWidth+12,y-12,&memDC,0,0,SRCCOPY);
	}
	memDC.SelectObject(pOldBitmap);

}
void CImageBubble::onLClicked()
{
}
void CImageBubble::onRClicked()
{
}