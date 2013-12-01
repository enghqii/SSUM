#pragma once
#include "Bubble.h"
// CStringBubble 명령 대상입니다.

class CStringBubble : public CBubble
{
private:
	CString m_data;
public:
	CStringBubble();
	CStringBubble(int x,int y,CString data);
	virtual ~CStringBubble();
	
	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();

};


