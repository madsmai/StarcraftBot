#pragma once
#include <BWAPI.h>
#include "BuildOrderType.h"

using namespace BWAPI;

class Strategy {
public:

	virtual std::string getName() = 0;

	virtual bool isStarter() = 0;
	virtual bool isEndGame() = 0;
	virtual bool isAntiAir() = 0;
	virtual bool isAntiInvis() = 0;
	virtual bool isAntiRush() = 0;
	virtual bool isAntiTurtle() = 0;

	virtual std::vector<BuildOrderType> getBuildOrder() = 0;


	virtual void setArmySize() = 0;

};

