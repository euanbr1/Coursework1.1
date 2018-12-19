#pragma once
#include "cSprite.h"
class cSanta : public cSprite
{
private:
	MapRC santaPos;
	float santaRotation;

public:
	cSanta();
	virtual ~cSanta();
	void update(int row, int column);
	void setMapPosition(int row, int column);
	MapRC getMapPosition();
	void setPlayerRotation(float angle);
	float getPlayerRotation();
};

