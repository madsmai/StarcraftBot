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

	void setArmySize(){ OffenseManager::getInstance().setArmySize(999); }
	static void evaluateStrategy();

	// constructor
	BaseBuild();

private:

	//Unit Types, Upgrade types, Request Types
	UnitType probe = UnitTypes::Protoss_Probe;
	UnitType pylon = UnitTypes::Protoss_Pylon;
	UnitType gateway = UnitTypes::Protoss_Gateway;
	UnitType zealot = UnitTypes::Protoss_Zealot;
	UnitType forge = UnitTypes::Protoss_Forge;
	UnitType assimilator = UnitTypes::Protoss_Assimilator;
	UnitType cybercore = UnitTypes::Protoss_Cybernetics_Core;
	UnitType dragoon = UnitTypes::Protoss_Dragoon;
	UnitType citadel = UnitTypes::Protoss_Citadel_of_Adun;
	UnitType archives = UnitTypes::Protoss_Templar_Archives;
	UnitType dtemplar = UnitTypes::Protoss_Dark_Templar;
	UnitType cannon = UnitTypes::Protoss_Photon_Cannon;
	int scoutRequest = BuildOrderType::requests::scoutRequest;
	int gasworkerRequest = BuildOrderType::requests::gasworkerRequest;
	int evaluateStrategyRequest = BuildOrderType::requests::evaluateStrategyRequest;

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

