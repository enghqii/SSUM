// Bubble.cpp : 구현 파일입니다.
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


// CBubble 멤버 함수

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