#pragma once
#include <BWAPI.h>
#include "Strategy.h"
#include "OffenseManager.h"
#include "BuildOrderManager.h"
#include "StrategyManager.h"

class CarrierSpam : public Strategy {
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
	CarrierSpam();

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
	UnitType supportbay = UnitTypes::Protoss_Robotics_Support_Bay;
	UnitType reaver = UnitTypes::Protoss_Reaver;
	UnitType scarab = UnitTypes::Protoss_Scarab;
	UnitType stargate = UnitTypes::Protoss_Stargate;
	UnitType fleet_beacon = UnitTypes::Protoss_Fleet_Beacon;
	UnitType carrier = UnitTypes::Protoss_Carrier;
	UnitType interceptor = UnitTypes::Protoss_Interceptor;

	UpgradeType carrier_capacity = UpgradeTypes::Carrier_Capacity;
	UpgradeType reaver_capacity = UpgradeTypes::Reaver_Capacity;

	int scoutRequest = BuildOrderType::requests::scoutRequest;
	int gasworkerRequest = BuildOrderType::requests::gasworkerRequest;
	int evaluateStrategyRequest = BuildOrderType::requests::evaluateStrategyRequest;

	bool starter = false;
	bool endgame = true;
	bool antiAir = false;
	bool antiInvis = false;
	bool antiRush = false;
	bool antiTurtle = true;

	// Build order for the specific tactic
	std::vector<BuildOrderType> buildOrder;

	// name of the tactic
	std::string name = "Carrier spam strategy";

};