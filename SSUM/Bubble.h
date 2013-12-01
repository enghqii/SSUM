#pragma once

// CBubble 명령 대상입니다.

class CBubble : public CObject
{
protected:
	int x;
	int y;
	int height;
public:
	CBubble();
	virtual ~CBubble();

	virtual void onDraw(CDC* pDC)=0;
	virtual void onLClicked()=0;
	virtual void onRClicked()=0;
	void setPosition(int x,int y);
	void setPosition(POINT p);
	POINT getNextPosition();
};


