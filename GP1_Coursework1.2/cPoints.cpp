#include "cPoints.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cPoints::cPoints()
{
	this->setMapPosition(this->spriteRandom(gen), this->spriteRandom(gen));
}
void cPoints::update(int row, int column)
{
	this->setMapPosition(column, row);
}

void cPoints::setMapPosition(int row, int column)
{
	this->baublePos = { column, row };
}

MapRC cPoints::getMapPosition()
{
	return this->baublePos;
}

void cPoints::setBaubleRotation(float angle)
{
	this->baubleRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cPoints::getBaubleRotation()
{
	return this->baubleRotation;
}

void cPoints::genRandomPos(int row, int column)
{

	while (this->getMapPosition().C == column && this->getMapPosition().R == row)
	{
		this->setMapPosition(this->spriteRandom(gen), this->spriteRandom(gen));
	}
}

/*
=================================================================
Defualt Destructor
=================================================================
*/


cPoints::~cPoints()
{
}
