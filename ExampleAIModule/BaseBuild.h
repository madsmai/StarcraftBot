#pragma once
#include <BWAPI.h>
#include "Strategy.h"
#include "OffenseManager.h"
#include "BuildOrderManager.h"
#include "StrategyManager.h"

class BaseBuild : public Strategy {

public:

	std::string getName() { return name; }

	bool isStarter() { return starter; }
	bool isEndGame() { return endgame; }
	bool isAntiAir() { return antiAir; }
	bool isAntiInvis() { return antiInvis; }
	bool isAntiRush() { return antiRush; }
	bool isAntiTurtle() { return antiTurtle; }

	std::vector<BuildOrderType> getBuildOrder() { return buildOrder; }

	void setSquadSize(int size){ OffenseManager::getInstance().setSquadSize(size); }
	static void evaluateStrategy();
	void setStrategyVariables();
	// constructor
	BaseBuild();

private:

	//Unit Types, Upgrade types, Request Types
	UnitType probe = UnitTypes::Protoss_Probe;
	UnitType pylon = UnitTypes::Protoss_Pylon;

	bool starter = true;
	bool endgame = false;
	bool antiAir = false;
	bool antiInvis = false;
	bool antiRush = false;
	bool antiTurtle = false;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name = "basic 4 probe 1 pylon";

};

