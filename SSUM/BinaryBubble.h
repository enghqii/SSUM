#pragma once
#include "Bubble.h"
// CBinaryBubble ��� ����Դϴ�.

class CBinaryBubble : public CBubble
{
public:
	CBinaryBubble();
	virtual ~CBinaryBubble();

	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();
};


