#pragma once
#include "Bubble.h"
// CImageBubble ��� ����Դϴ�.

class CImageBubble : public CBubble
{
public:
	CImageBubble();
	virtual ~CImageBubble();

	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();
};


