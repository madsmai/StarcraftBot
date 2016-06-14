#pragma once
#include <BWAPI.h>
#include "Strategy.h"
#include "OffenseManager.h"
#include "BuildOrderManager.h"
#include "StrategyManager.h"

using namespace BWAPI;

class ObserverTech :public Strategy {
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
	ObserverTech();

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

	UnitType robotics = UnitTypes::Protoss_Robotics_Facility;
	UnitType observatory = UnitTypes::Protoss_Observatory;
	UnitType observer = UnitTypes::Protoss_Observer;

	int scoutRequest = BuildOrderType::requests::scoutRequest;
	int gasworkerRequest = BuildOrderType::requests::gasworkerRequest;
	int evaluateStrategyRequest = BuildOrderType::requests::evaluateStrategyRequest;

	bool starter = false;
	bool endgame = true;
	bool antiAir = false;
	bool antiInvis = true;
	bool antiRush = false;
	bool antiTurtle = false;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name = "ObserverTech";

};

