#pragma once
#include "BWAPI.h"
#include "BuildOrderType.h"

using namespace BWAPI;

class Strategy {
public:


	enum strategies { justSomeStrategy, standardZealotRush };

	Strategy(int strategy);
	std::string getName(){ return name; }
	bool getStarter(){ return starter; }
	std::vector<BuildOrderType> const & getStrategyQueue(){ return strategyQueue; }

	//BuildOrderTypes for easy access

private:
	std::string name;
	bool starter;
	std::vector<BuildOrderType> strategyQueue;

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

	//Template strategies
	void theFirstStrat();
	void initialStrat();


};

