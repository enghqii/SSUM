#pragma once
#include "Bubble.h"
// CImageBubble 명령 대상입니다.

class CImageBubble : public CBubble
{
public:
	CImageBubble();
	virtual ~CImageBubble();

	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();
};


