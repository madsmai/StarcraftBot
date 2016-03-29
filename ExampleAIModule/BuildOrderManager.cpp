#include "BuildOrderManager.h"

/*
TODO:


*/

void BuildOrderManager::onStart(){
	BWAPI::UnitType probe = BWAPI::UnitTypes::Protoss_Probe;
	BWAPI::UnitType pylon = BWAPI::UnitTypes::Protoss_Pylon;
	BWAPI::UnitType gateway = BWAPI::UnitTypes::Protoss_Gateway;
	BWAPI::UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;

	//fixedOrderQueue.push(pylon); for testing
	fixedOrderQueue.push(probe);
	fixedOrderQueue.push(probe);
	fixedOrderQueue.push(probe);
	fixedOrderQueue.push(pylon);
	fixedOrderQueue.push(gateway);
	fixedOrderQueue.push(zealot);
	fixedOrderQueue.push(zealot);
	fixedOrderQueue.push(zealot);

	//This is commented out for testing purposes
	//fixedOrder = false;
}

void BuildOrderManager::onFrame(){
	if (!fixedOrder){ //FixedOrder might not be needed, since everyhting is enqueued in onStart()
		trainZealot();
		trainProbe();

		//Make sure we only build one building per frame
		//This might fuck shit up; it might only build Gateways since those are first; as long as the logic allows it
		if (buildGateway()){
			return;
		}
		else if (buildSupply()){
			return;
		}
		else if (buildForge()){
			return;
		}
		else if (buildRefinery()){
			return;
		}
		else if (buildCitadelOfAdun()){
			return;
		}
		else if (buildCyberneticsCore()){
			return;
		}
		else if (buildPhotonCannon()){
			return;
		}
	}

	//Research and scouts has to have their own logic since it can't be in the queue
	researchForge();
	researchCyberneticsCore();
	makeScout();
}

bool BuildOrderManager::buildGateway(){
	static int lastChecked = 0;
	BWAPI::UnitType gateway = BWAPI::UnitTypes::Protoss_Gateway;
	if (((BWAPI::Broodwar->self()->supplyUsed() / 2 >= 10 && BWAPI::Broodwar->self()->allUnitCount(gateway) == 0) || 
		(BWAPI::Broodwar->self()->supplyUsed() / 2 >= 12 && BWAPI::Broodwar->self()->allUnitCount(gateway) == 1)) && 
		lastChecked + 400 < BWAPI::Broodwar->getFrameCount()){

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(gateway);
		fixedOrderQueue.push(gateway);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildSupply(){
	static int lastChecked = 0;
	BWAPI::UnitType supplyType = BWAPI::UnitTypes::Protoss_Pylon;
	if (((BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed()) / 2 < 5|| 
		BWAPI::Broodwar->self()->supplyUsed() < 17) && lastChecked + 400 < BWAPI::Broodwar->getFrameCount() && 
		BWAPI::Broodwar->self()->incompleteUnitCount(supplyType) == 0) {

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(supplyType);
		fixedOrderQueue.push(supplyType);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildForge(){
	BWAPI::UnitType forge = BWAPI::UnitTypes::Protoss_Forge;
	static int lastChecked = 0;

	if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 2
		&& lastChecked + 400 < BWAPI::Broodwar->getFrameCount()
		&& BWAPI::Broodwar->self()->allUnitCount(forge) == 0){

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(forge);
		fixedOrderQueue.push(forge);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildRefinery(){
	static int lastChecked = 0;
	BWAPI::UnitType refineryType = BWAPI::UnitTypes::Protoss_Assimilator;
	if (BWAPI::Broodwar->self()->allUnitCount(refineryType) == 0
		&& (BWAPI::Broodwar->self()->supplyUsed() / 2) > 12
		&& lastChecked + 400 < BWAPI::Broodwar->getFrameCount()){

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(refineryType);
		fixedOrderQueue.push(refineryType);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildCitadelOfAdun(){
	static int lastChecked = 0;
	BWAPI::UnitType citadel_Of_Adun = BWAPI::UnitTypes::Protoss_Citadel_of_Adun;

	if (lastChecked + 600 < BWAPI::Broodwar->getFrameCount()
		&& BWAPI::Broodwar->self()->allUnitCount(citadel_Of_Adun) == 0
		&& BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core)){

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(citadel_Of_Adun);
		fixedOrderQueue.push(citadel_Of_Adun);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildCyberneticsCore(){
	static int lastChecked = 0;
	BWAPI::UnitType cybernetics_Core = BWAPI::UnitTypes::Protoss_Cybernetics_Core;

	// Amount of gateways (including ones in construction)
	int gatewayCount = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Gateway)
		+ BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Protoss_Gateway);

	if (lastChecked + 600 < BWAPI::Broodwar->getFrameCount()
		&& BWAPI::Broodwar->self()->allUnitCount(cybernetics_Core) == 0
		&& BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 1
		&& BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 2
		&& BWAPI::Broodwar->self()->supplyUsed() / 2 >= 15) {

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(cybernetics_Core);
		fixedOrderQueue.push(cybernetics_Core);
		return true;
	}
	return false;
}

bool BuildOrderManager::buildPhotonCannon(){
	static int lastChecked = 0;
	BWAPI::UnitType photon_cannon = BWAPI::UnitTypes::Protoss_Photon_Cannon;

	if (lastChecked + 600 < BWAPI::Broodwar->getFrameCount()
		&& BWAPI::Broodwar->self()->allUnitCount(photon_cannon) < 5
		&& BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Forge) >= 1){

		lastChecked = BWAPI::Broodwar->getFrameCount();
		//ProbeManager::getInstance().addBuilding(photon_cannon);
		fixedOrderQueue.push(photon_cannon);
		return true;
	}
	return false;
}

void BuildOrderManager::researchForge(){

	//Upgrade types
	BWAPI::UpgradeType ground_weapons = BWAPI::UpgradeTypes::Protoss_Ground_Weapons;
	BWAPI::UpgradeType ground_armor = BWAPI::UpgradeTypes::Protoss_Ground_Armor;
	BWAPI::UpgradeType plasma_shields = BWAPI::UpgradeTypes::Protoss_Plasma_Shields;

	//Prioritised list of upgrades
	// Ground weapons
	if (OffenseManager::getInstance().getGroundWeapons() == 0 && BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >= 2){
		BuildingManager::getInstance().addUpgrade(ground_weapons);
		OffenseManager::getInstance().getGroundWeapons()++;
	}

	// Ground armor
	else if (OffenseManager::getInstance().getGroundWeapons() >= 1){
		BuildingManager::getInstance().addUpgrade(ground_armor);
		OffenseManager::getInstance().getGroundArmor()++;
	}

	// Plasma shields
	else if (OffenseManager::getInstance().getGroundArmor() >= 1){
		BuildingManager::getInstance().addUpgrade(plasma_shields);
		OffenseManager::getInstance().getPlasmaShields()++;
	}
}

void BuildOrderManager::researchCyberneticsCore(){

	//Upgrade types
	BWAPI::UpgradeType air_weapons = BWAPI::UpgradeTypes::Protoss_Air_Weapons;
	BWAPI::UpgradeType air_armor = BWAPI::UpgradeTypes::Protoss_Air_Armor;
	BWAPI::UpgradeType singularity_charge = BWAPI::UpgradeTypes::Singularity_Charge;

	// Air weapons
	if (FALSE){
		BuildingManager::getInstance().addUpgrade(air_weapons);
		OffenseManager::getInstance().getAirWeapons()++;
	}

	// Air armor
	if (FALSE){
		BuildingManager::getInstance().addUpgrade(air_armor);
		OffenseManager::getInstance().getAirArmor()++;
	}

	// Singularity_Charge
	if (FALSE){
		BuildingManager::getInstance().addUpgrade(singularity_charge);
		OffenseManager::getInstance().getSingularityCharge() = true;
	}
}

void BuildOrderManager::trainZealot(){
	BWAPI::UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;

	if ((BWAPI::Broodwar->self()->incompleteUnitCount(zealot) + 
		BWAPI::Broodwar->self()->completedUnitCount(zealot)) < OffenseManager::getInstance().getZealotMax()
		&& BWAPI::Broodwar->self()->supplyUsed() < BWAPI::Broodwar->self()->supplyTotal() 
		&& BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Gateway) >=1){

		//BuildingManager::getInstance().addUnit(zealot);
		fixedOrderQueue.push(zealot);
	}
}

void BuildOrderManager::trainProbe(){
	BWAPI::UnitType probe = BWAPI::UnitTypes::Protoss_Probe;
	int workerCount = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe);
	int gatewayCount = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Gateway);
	if (workerCount < 25 && (!(workerCount >= 10 && gatewayCount == 0) || !(workerCount >= 12 && gatewayCount == 1))){
		//BuildingManager::getInstance().addUnit(probe);
		fixedOrderQueue.push(probe);
	}
}

void BuildOrderManager::makeScout(){
	if (BWAPI::Broodwar->self()->supplyTotal() < 19){
		ProbeManager::getInstance().addScoutRequest();
	}
}

BuildOrderManager& BuildOrderManager::getInstance(){ //Return ref to BuildOrderManager object
	static BuildOrderManager i; //Make static instance i
	return i;
}
