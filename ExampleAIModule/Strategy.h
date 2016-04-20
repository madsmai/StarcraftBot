#pragma once
#include "BWAPI.h"
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



private:

	bool starter;
	bool endgame;
	bool antiAir, antiInvis, antiRush, antiTurtle;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name;


};

