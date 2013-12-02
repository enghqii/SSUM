#pragma once
#include "Bubble.h"
// CBinaryBubble ��� ����Դϴ�.

class CBinaryBubble : public CBubble
{
private:
	CString path;
	CString fileName;
public:
	CBinaryBubble();
	CBinaryBubble(CString fileName,CString path);
	CBinaryBubble(CString fileName,CString path,UINT align);
	virtual ~CBinaryBubble();

	void onDraw(CDC* pDC);
	void onLClicked(HWND& m_hWnd);
	void onRClicked();
};


