#pragma once
#include "Bubble.h"
// CStringBubble 명령 대상입니다.

class CStringBubble : public CBubble
{
private:
	CString m_data;
public:
	CStringBubble();
	CStringBubble(CString data);
	CStringBubble(CString data,UINT align);
	virtual ~CStringBubble();
	
	void onDraw(CDC* pDC);
	void onLClicked(HWND& m_hWnd);
	void onRClicked();

};


