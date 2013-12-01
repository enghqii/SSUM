#pragma once
#include "Bubble.h"
// CBinaryBubble 명령 대상입니다.

class CBinaryBubble : public CBubble
{
public:
	CBinaryBubble();
	virtual ~CBinaryBubble();

	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();
};


