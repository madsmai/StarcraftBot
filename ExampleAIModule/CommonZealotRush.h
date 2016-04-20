#pragma once
#include "Strategy.h"

class CommonZealotRush :public Strategy {
public:


	std::string getName() { return name; }

	bool isStarter() { return starter; }
	bool isEndGame() { return endgame; }
	bool isAntiAir() { return antiAir; }
	bool isAntiInvis() { return antiInvis; }
	bool isAntiRush() { return antiRush; }
	bool isAntiTurtle() { return antiTurtle; }

	std::vector<BuildOrderType> getBuildOrder() { return buildOrder; }

	// constructor
	CommonZealotRush();

private:

	//Unit Types, Upgrade types, Request Types
	UnitType probe = BWAPI::UnitTypes::Protoss_Probe;
	UnitType pylon = BWAPI::UnitTypes::Protoss_Pylon;
	UnitType gateway = BWAPI::UnitTypes::Protoss_Gateway;
	UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;
	UnitType forge = BWAPI::UnitTypes::Protoss_Forge;
	UnitType assimilator = BWAPI::UnitTypes::Protoss_Assimilator;
	UnitType cybercore = BWAPI::UnitTypes::Protoss_Cybernetics_Core;
	UnitType dragoon = BWAPI::UnitTypes::Protoss_Dragoon;
	UnitType citadel = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;
	UnitType archives = BWAPI::UnitTypes::Protoss_Templar_Archives;
	UnitType dtemplar = BWAPI::UnitTypes::Protoss_Dark_Templar;
	UnitType cannon = BWAPI::UnitTypes::Protoss_Photon_Cannon;
	int scoutRequest = BuildOrderType::requests::scoutRequest;
	int gasworkerRequest = BuildOrderType::requests::gasworkerRequest;

	bool starter = true;
	bool endgame = false;
	bool antiAir = false;
	bool antiInvis = false;
	bool antiRush = false;
	bool antiTurtle = false;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name = "10/12 Gateway zealot rush";

};

