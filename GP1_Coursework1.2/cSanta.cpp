/*
=================
- cShip.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cSanta.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cSanta::cSanta()
{
	this->setMapPosition( 0, 0 );
}

void cSanta::update(int row, int column)
{
	this->setMapPosition(column, row );
}

void cSanta::setMapPosition(int row, int column)
{
	this->santaPos = { column, row };
}

MapRC cSanta::getMapPosition()
{
	return this->santaPos;
}

void cSanta::setPlayerRotation(float angle)
{
	this->santaRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cSanta::getPlayerRotation()
{
	return this->santaRotation;
}

/*
=================================================================
Defualt Destructor
=================================================================
*/
cSanta::~cSanta()
{
}
