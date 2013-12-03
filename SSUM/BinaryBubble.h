#pragma once
#include "Bubble.h"
// CBinaryBubble 명령 대상입니다.

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
	void onRClickedView(HWND& m_hWnd);
	void onRClickedSave(HWND& m_hWnd);
	void onRClickedSaveAs(HWND& m_hWnd);
};


