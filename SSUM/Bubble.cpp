// Bubble.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "CBubbleTest.h"
#include "Bubble.h"


// CBubble

CBubble::CBubble() : x(0), y(0)
{
}

CBubble::~CBubble()
{
}


// CBubble ��� �Լ�

void CBubble::setPosition(int x,int y)
{
	this->x = x;
	this->y = y;
}
void CBubble::setPosition(POINT p)
{
	this->x = p.x;
	this->y = p.y;
}
POINT CBubble::getNextPosition()
{
	POINT p;
	p.x = this->x;
	p.y = this->y-height-10;
	return p;
}