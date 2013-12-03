#pragma once
#include "Bubble.h"
// CImageBubble 명령 대상입니다.

class CImageBubble : public CBubble
{
private:
	CBitmap m_data;
	CString path;
	CString fileName;
public:
	CImageBubble();
	CImageBubble(CString fileName,CString path);
	CImageBubble(CString fileName,CString path,UINT align);
	virtual ~CImageBubble();

	void onDraw(CDC* pDC);
	void onLClicked(HWND& m_hWnd);
	void onRClickedView(HWND& m_hWnd);
	void onRClickedSave(HWND& m_hWnd);
	void onRClickedSaveAs(HWND& m_hWnd);
	void deletetempfile();
};


