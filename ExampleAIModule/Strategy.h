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

	// constructor
	Strategy();

private:

	BuildOrderType probe = BuildOrderType(UnitTypes::Protoss_Probe);
	BuildOrderType pylon = BuildOrderType(UnitTypes::Protoss_Pylon);
	BuildOrderType gateway = BuildOrderType(UnitTypes::Protoss_Gateway);
	BuildOrderType scoutRequest = BuildOrderType(BuildOrderType::requests::scoutRequest);
	BuildOrderType assimilator = BuildOrderType(UnitTypes::Protoss_Assimilator);
	BuildOrderType gasworkerRequest = BuildOrderType(BuildOrderType::requests::gasworkerRequest);
	BuildOrderType zealot = BuildOrderType(UnitTypes::Protoss_Zealot);
	BuildOrderType cybercore = BuildOrderType(UnitTypes::Protoss_Cybernetics_Core);
	BuildOrderType dragoon = BuildOrderType(UnitTypes::Protoss_Dragoon);
	BuildOrderType citadel = BuildOrderType(UnitTypes::Protoss_Citadel_of_Adun);
	BuildOrderType dtemplar = BuildOrderType(UnitTypes::Protoss_Dark_Templar);
	BuildOrderType archives = BuildOrderType(UnitTypes::Protoss_Templar_Archives);
	BuildOrderType forge = BuildOrderType(UnitTypes::Protoss_Forge);
	BuildOrderType cannon = BuildOrderType(UnitTypes::Protoss_Photon_Cannon);

	bool starter;
	bool endgame;
	bool antiAir, antiInvis, antiRush, antiTurtle;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name;


};

