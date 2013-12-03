#pragma once

// CBubble 명령 대상입니다.

#define CB_LEFT 0
#define CB_RIGHT 1

class CBubble : public CObject
{
protected:
	int x;
	int y;
	int height;
	int width;
	UINT align;
public:
	CBubble();
	CBubble(UINT align);
	virtual ~CBubble();

	virtual void onDraw(CDC* pDC)=0;
	virtual void onLClicked(HWND& m_hWnd)=0;
	virtual void onRClickedView(HWND& m_hWnd)=0;
	virtual void onRClickedSave(HWND& m_hWnd)=0;
	virtual void onRClickedSaveAs(HWND& m_hWnd)=0;
	int getHeight();
	void setPosition(int y);
	int getNextPosition();
	BOOL isBLocate(CPoint point);
};


