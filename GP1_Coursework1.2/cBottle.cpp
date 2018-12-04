#include "cBottle.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cBottle::cBottle()
{
	this->setMapPosition(this->spriteRandom(gen), this->spriteRandom(gen));
}
void cBottle::update(int row, int column)
{
	this->setMapPosition(column, row);
}

void cBottle::setMapPosition(int row, int column)
{
	this->bottlePos = { column, row };
}

MapRC cBottle::getMapPosition()
{
	return this->bottlePos;
}

void cBottle::setBottleRotation(float angle)
{
	this->bottleRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cBottle::getBottleRotation()
{
	return this->bottleRotation;
}

void cBottle::genRandomPos(int row, int column)
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


cBottle::~cBottle()
{
}
