#pragma once
#include "Bubble.h"
// CStringBubble ��� ����Դϴ�.

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


