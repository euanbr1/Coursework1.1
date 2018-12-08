/*
=================
- cShip.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cEnemy.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cEnemy::cEnemy()
{
	this->setMapPosition(0, 0);
	this->enemyVelocity = 0;
}

void cEnemy::update(int row, int column)
{
	this->setMapPosition(column, row);
}

void cEnemy::moveEnemy(int row, int column)
{
	// New enemy position
	this->setMapPosition(this->enemyPos.R + sgn(row - this->enemyPos.R), this->enemyPos.C + sgn(column - this->enemyPos.C));

}

void cEnemy::setMapPosition(int row, int column)
{
	this->enemyPos = { column, row };
}

MapRC cEnemy::getMapPosition()
{
	return this->enemyPos;
}

void cEnemy::setEnemyRotation(float angle)
{
	this->enemyRotation = angle;
	this->setSpriteRotAngle(angle);
}

float cEnemy::getEnemyRotation()
{
	return this->enemyRotation;
}

void cEnemy::genRandomPos(MapRC ship, MapRC bottle)
{
	this->setMapPosition(spriteRandom(gen), spriteRandom(gen));
	while (this->getMapPosition() == ship && this->getMapPosition() == bottle)
	{
		this->setMapPosition(this->spriteRandom(gen), 0);   ///this->spriteRandom(gen)
	}
}


void cEnemy::update(double deltaTime)
{

	/*this->setEnemyRotation((float)(this->getEnemyRotation() + (5.0f * deltaTime)));
	if (this->getEnemyRotation() > 360)
	{
		this->setEnemyRotation(this->getEnemyRotation() - 360.0f);
	}

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += (int)(this->getSpriteTranslation().x * deltaTime);
	currentSpritePos.y -= (int)(this->getSpriteTranslation().y * deltaTime);

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	this->setBoundingRect(this->getSpritePos()); */
}

/*void cEnemy::setEnemyVelocity(int AsteroidVel)
{
	enemyVelocity = AsteroidVel;
}
int cEnemy::getEnemyVelocity()
{
	return enemyVelocity;
}*/
/*
=================================================================
Defualt Destructor
=================================================================
*/

cEnemy::~cEnemy()
{
}
