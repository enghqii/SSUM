// Bubble.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Bubble.h"


// CBubble

CBubble::CBubble() : x(10), y(0), align(CB_LEFT)
{
}

CBubble::CBubble(UINT align) : y(0), align(align)
{
	if(align == CB_LEFT)
	{
		x = 10;
	}
	else if(align == CB_RIGHT)
	{
		x = 280;
	}
}

CBubble::~CBubble()
{
}


// CBubble 멤버 함수

int CBubble::getHeight()
{
	return height;
}
void CBubble::setPosition(int y)
{
	this->y = y;
}
int CBubble::getNextPosition()
{
	return this->y-height-10;
}
BOOL CBubble::isBLocate(CPoint point)
{
	if(align==CB_LEFT)
	{
		if(x < point.x && x + width > point.x &&
			y > point.y && y - height < point.y)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(align==CB_RIGHT)
	{
		if(x > point.x && x - width < point.x &&
			y > point.y && y - height < point.y)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}