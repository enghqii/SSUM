#pragma once

// CBubble ��� ����Դϴ�.

class CBubble : public CObject
{
protected:
	int x;
	int y;
public:
	CBubble();
	CBubble(int x,int y);
	virtual ~CBubble();

	virtual void onDraw(CDC* pDC)=0;
	virtual void onLClicked()=0;
	virtual void onRClicked()=0;
};


