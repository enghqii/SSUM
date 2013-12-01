#pragma once
#include "Bubble.h"
// CImageBubble ��� ����Դϴ�.

class CImageBubble : public CBubble
{
private:
	CBitmap m_data;
public:
	CImageBubble();
	CImageBubble(CString path);
	virtual ~CImageBubble();

	void onDraw(CDC* pDC);
	void onLClicked();
	void onRClicked();
};


