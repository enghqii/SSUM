#pragma once
#include "Bubble.h"
// CImageBubble ��� ����Դϴ�.

class CImageBubble : public CBubble
{
private:
	CBitmap m_data;
	CString path;
public:
	CImageBubble();
	CImageBubble(CString path);
	CImageBubble(CString path,UINT align);
	virtual ~CImageBubble();

	void onDraw(CDC* pDC);
	void onLClicked(HWND& m_hWnd);
	void onRClicked();
};


