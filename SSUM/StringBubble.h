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
	virtual ~CStringBubble();
	
	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();

};


