#include "ExampleAIModule.h"
#include <iostream>
#include <vector>
#include <BWTA.h>
#include <windows.h>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;


void ExampleAIModule::onStart() {

	// Initializing variables

	//Basic game variables
	scout = NULL;
	scouting = true;
	enemyUnits;
	ourZealots;


	// tactic variables
	zealot_rush = true;

	// zealot rush variables
	if (zealot_rush){
		zealotMAX = 6;
	}

	// Upgrade level variables
	ground_weapons_count = 0;
	ground_armor_count = 0;
	air_weapons_count = 0;
	air_armor_count = 0;
	plasma_shields_count = 0;
	singularity_Charge_Researched = false;

	// BWTA
	BWTA::readMap();
	BWTA::analyze();
	analyzed = false;
	analysis_just_finished = false;
	ourBase = BWTA::getStartLocation(Broodwar->self());
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
}

void ExampleAIModule::onFrame() {

	if (Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Zealot) >= zealotMAX){
		for (BWAPI::Unit zealot : ourZealots){
			zealot->attack(enemyBase->getPosition());
		}
		zealotMAX = zealotMAX * 2;
	}

	// Iterate through all the units that we own
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == UnitTypes::Protoss_Zealot && u->isIdle()) {

			//TODO: Find en god radius til det her
			u->attack(u->getClosestUnit((Filter::IsEnemy && Filter::CanAttack), 50));
		}

		if (u->getType() == UnitTypes::Protoss_Zealot && u->isUnderAttack()) {
			u->attack(u->getClosestUnit((Filter::IsEnemy && Filter::CanAttack), 4));
		}

		if (u->getType().isWorker() && u == scout && u->isUnderAttack()) {
			scout->move(ourBase->getPosition());
			scouting = false;
			scout = NULL;
		}

		// A resource depot is a Command Center, Nexus, or Hatchery
		else if (u->getType().isResourceDepot()) {

			int workerCount = Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe);
			int gatewayCount = Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway);

		} // closure: resoruceDepot

	} // closure: unit iterator
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit) {

	// scouting code

	if (unit->getPlayer()->isEnemy(Broodwar->self())){
		//enemyUnits.push_back(unit);
		Broodwar << "Unit found: " << unit->getType().getName() << "!" << std::endl;

	}

	if (scout != NULL){
		if (scouting
			&& unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			scout->attack(unit);
			scouting = false;
			Broodwar->sendText("Done scouting, found mainbase");

		}

		else if (unit->getPlayer() != Broodwar->self() && unit->getType().isResourceDepot()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {

			expansion = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			Broodwar->sendText("Found expansion");
			goScout(scout);
		}
	}



}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{

	if (unit->getType() == UnitTypes::Protoss_Zealot
		&& unit->getPlayer() == Broodwar->self()){
		ourZealots.push_back(unit);
	}

	if (unit->getType() == UnitTypes::Protoss_Gateway){
		unit->setRallyPoint(BWTA::getNearestChokepoint(ourBase->getPosition())->getCenter());
	}


}


// Kasper
void ExampleAIModule::trainZealots(BWAPI::Unit gateway){

	BWAPI::UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;
	int zealotPrice = zealot.mineralPrice();

	if (gateway->isIdle()
		&& Broodwar->self()->minerals() - mineralsReserved >= zealotPrice
		&& (Broodwar->self()->incompleteUnitCount(zealot) + Broodwar->self()->completedUnitCount(zealot)) < zealotMAX
		&& Broodwar->self()->supplyUsed() < Broodwar->self()->supplyTotal()){

		gateway->train(zealot);
	}


}


// Kasper
void ExampleAIModule::releaseFromList(BWAPI::Unit unit, std::vector<BWAPI::Unit> list){

	std::vector<BWAPI::Unit>::iterator it; 		// iteratation vector
	for (it = list.begin(); it != list.end(); it++){
		// remove the price if it is found in the vector
		if (*it == unit){
			list.erase(it);
			break;
		}
	}

}


// Kasper
void ExampleAIModule::releaseMinerals(BWAPI::Unit unit){
	if (unit->getType().isBuilding()) {
		int mineralPrice = unit->getType().mineralPrice();
		std::vector<int>::iterator it; 		// iteratation vector
		for (it = pendingBuildings.begin(); it != pendingBuildings.end(); it++){
			// remove the price if it is found in the vector
			if (*it == mineralPrice){
				mineralsReserved -= mineralPrice;
				pendingBuildings.erase(it);
				// break the loop to make sure no dublicates are removed
				break;
			}
		}
	}
}

// Kasper
void ExampleAIModule::constructBuilding(BWAPI::UnitType buildingType, BWAPI::Unit worker){
	int buildingPrice = buildingType.mineralPrice();
	TilePosition targetBuildLocation = Broodwar->getBuildLocation(buildingType,
		worker->getTilePosition());
	if (!targetBuildLocation.isValid()){
		targetBuildLocation = Broodwar->getBuildLocation(buildingType, ourBase->getTilePosition());
	}
	if (targetBuildLocation) {
		// Register an event that draws the target build location
		Broodwar->registerEvent([targetBuildLocation, buildingType](Game*) {
			Broodwar->drawBoxMap(Position(targetBuildLocation),
				Position(targetBuildLocation + buildingType.tileSize()),
				Colors::Yellow);
		},
			nullptr,  // condition
			buildingType.buildTime() + 100);  // frames to run

		// Order the worker to construct the structure
		worker->build(buildingType, targetBuildLocation);
		mineralsReserved += buildingPrice;
		pendingBuildings.push_back(buildingPrice);
	}
}

//Kasper
void ExampleAIModule::callBuildFunctions(BWAPI::Unit worker){
	buildGateway(worker);
	buildSupply(worker);
	buildForge(worker);
	buildRefinery(worker);
	buildCitadel_Of_Adun(worker);
	buildCybernetics_Core(worker);
	buildPhoton_Cannon(worker);
}


// Kasper
void ExampleAIModule::buildGateway(BWAPI::Unit worker){

	static int lastChecked = 0;
	UnitType gateway = UnitTypes::Protoss_Gateway;
	if (zealot_rush
		&& ((Broodwar->self()->supplyUsed() / 2 >= 10 && Broodwar->self()->allUnitCount(gateway) == 0)
		|| (Broodwar->self()->supplyUsed() / 2 >= 12 && Broodwar->self()->allUnitCount(gateway) == 1))
		&& Broodwar->self()->minerals() - mineralsReserved >= gateway.mineralPrice()
		&& lastChecked + 400 < Broodwar->getFrameCount()
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(gateway, worker);
	}

}


// Kasper
void ExampleAIModule::buildSupply(BWAPI::Unit worker){
	static int lastChecked = 0;
	UnitType supplyType = UnitTypes::Protoss_Pylon;
	if (((Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed()) / 2 < 5
		|| Broodwar->self()->supplyUsed() < 17)
		&& lastChecked + 400 < Broodwar->getFrameCount()
		&& Broodwar->self()->minerals() - mineralsReserved >= supplyType.mineralPrice()
		&& Broodwar->self()->incompleteUnitCount(supplyType) == 0) {

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(supplyType, worker);

		if (Broodwar->self()->supplyTotal() < 19){
			scout = worker;
		}
	}
}


// Kasper
void ExampleAIModule::buildRefinery(BWAPI::Unit worker){

	static int lastChecked = 0;
	UnitType refineryType = UnitTypes::Protoss_Assimilator;
	if (Broodwar->self()->allUnitCount(refineryType) == 0
		&& Broodwar->self()->minerals() >= refineryType.mineralPrice()
		&& (Broodwar->self()->supplyUsed() / 2) > 12
		&& lastChecked + 400 < Broodwar->getFrameCount()
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(refineryType, worker);

	}

}

// Kasper
void ExampleAIModule::buildCybernetics_Core(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType cybernetics_Core = UnitTypes::Protoss_Cybernetics_Core;

	// Amount of gateways (including ones in construction)
	int gatewayCount = Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Gateway)
		+ Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Gateway);


	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(cybernetics_Core) == 0
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Gateway) >= 1
		&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
		&& Broodwar->self()->minerals() >= cybernetics_Core.mineralPrice()
		&& Broodwar->self()->supplyUsed() / 2 >= 15
		//TODO: Tactic
		) {

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(cybernetics_Core, worker);

	}
}

// Kasper
void ExampleAIModule::buildPhoton_Cannon(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType photon_cannon = UnitTypes::Protoss_Photon_Cannon;

	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(photon_cannon) < 5
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Forge) >= 1
		&& Broodwar->self()->minerals() >= photon_cannon.mineralPrice() + 400
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(photon_cannon, worker);

	}

}

// Kasper
void ExampleAIModule::buildCitadel_Of_Adun(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType citadel_Of_Adun = UnitTypes::Protoss_Citadel_of_Adun;

	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(citadel_Of_Adun) == 0
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Cybernetics_Core)
		&& Broodwar->self()->minerals() >= citadel_Of_Adun.mineralPrice()
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(citadel_Of_Adun, worker);

	}
}

// Kasper
void ExampleAIModule::buildForge(BWAPI::Unit worker){

	BWAPI::UnitType forge = UnitTypes::Protoss_Forge;
	static int lastChecked = 0;

	if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
		&& Broodwar->self()->minerals() >= forge.mineralPrice() + 100
		&& lastChecked + 400 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(forge) == 0
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(forge, worker);

	}

}


//Kasper
void ExampleAIModule::researchForge(BWAPI::Unit forge){

	if (forge->isIdle()){

		// Ground weapons
		BWAPI::UpgradeType ground_weapons = BWAPI::UpgradeTypes::Protoss_Ground_Weapons;
		int ground_weapons_mineral_price = ground_weapons.mineralPrice()
			+ (ground_weapons.mineralPriceFactor() * ground_weapons_count);
		int ground_weapons_gas_price = ground_weapons.gasPrice()
			+ (ground_weapons.gasPriceFactor() * ground_weapons_count);

		if (ground_weapons_count == 0
			&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
			&& Broodwar->self()->minerals() >= ground_weapons_mineral_price + 100
			&& Broodwar->self()->gas() >= ground_weapons_gas_price
			//TODO: Tactic
			){

			forge->upgrade(ground_weapons);
			ground_weapons_count++;

		}

		// Ground armor
		BWAPI::UpgradeType ground_armor = BWAPI::UpgradeTypes::Protoss_Ground_Armor;
		int ground_armor_mineral_price = ground_armor.mineralPrice()
			+ (ground_armor.mineralPriceFactor() * ground_armor_count);
		int ground_armor_gas_price = ground_armor.gasPrice()
			+ (ground_armor.gasPriceFactor() * ground_armor_count);

		if (ground_weapons_count >= 1
			&& Broodwar->self()->minerals() >= ground_armor_mineral_price + 100
			&& Broodwar->self()->gas() >= ground_armor_gas_price
			//TODO: Tactic
			){

			forge->upgrade(ground_armor);
			ground_armor_count++;

		}

		// Plasma shields
		BWAPI::UpgradeType plasma_shields = BWAPI::UpgradeTypes::Protoss_Plasma_Shields;
		int plasma_shields_mineral_price = plasma_shields.mineralPrice()
			+ (plasma_shields.mineralPriceFactor() * plasma_shields_count);
		int plasma_shields_gas_price = plasma_shields.gasPrice()
			+ (plasma_shields.gasPriceFactor() * plasma_shields_count);

		if (ground_armor_count >= 1
			&& Broodwar->self()->minerals() >= plasma_shields_mineral_price + 100
			&& Broodwar->self()->gas() >= plasma_shields_gas_price
			//TODO: Tactic
			){

			forge->upgrade(plasma_shields);
			plasma_shields_count++;

		}

	}

}

// Kasper
void ExampleAIModule::researchCybernetics_Core(BWAPI::Unit cybernetics_Core){

	if (cybernetics_Core->isIdle()){

		// Air weapons
		BWAPI::UpgradeType air_weapons = BWAPI::UpgradeTypes::Protoss_Air_Weapons;
		int air_weapons_mineral_price = air_weapons.mineralPrice()
			+ (air_weapons.mineralPriceFactor() * air_weapons_count);
		int air_weapons_gas_price = air_weapons.gasPrice()
			+ (air_weapons.gasPriceFactor() * air_weapons_count);

		if (FALSE
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(air_weapons);
			air_weapons_count++;

		}

		// Air armor
		BWAPI::UpgradeType air_armor = BWAPI::UpgradeTypes::Protoss_Air_Armor;
		int air_armor_mineral_price = air_armor.mineralPrice()
			+ (air_armor.mineralPriceFactor() * air_armor_count);
		int air_armor_gas_price = air_armor.gasPrice()
			+ (air_armor.gasPriceFactor() * air_armor_count);

		if (FALSE
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(air_armor);
			air_armor_count++;

		}

		// Singularity_Charge
		BWAPI::UpgradeType singularity_charge = BWAPI::UpgradeTypes::Singularity_Charge;

		if (!singularity_Charge_Researched
			&& Broodwar->self()->minerals() >= singularity_charge.mineralPrice() + 400
			&& Broodwar->self()->gas() >= singularity_charge.gasPrice() + 200
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(singularity_charge);
			singularity_charge = true;

		}
	}


}



void ExampleAIModule::goScout(BWAPI::Unit scout){
	baseLocations = BWTA::getStartLocations();
	double minDistance = 9999999;

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = baseLocations.begin(); it != baseLocations.end(); ++it){
		scoutedBase = *it;
		// Remove our baselocation
		if (scoutedBase->getPosition() == ourBase->getPosition()){
			baseLocations.erase(*it);
			Broodwar->sendText("Removed our base");
		}
		else if (ourBase->getGroundDistance(scoutedBase) < minDistance) {
			minDistance = ourBase->getGroundDistance(scoutedBase);
			Broodwar->sendText("Moving to enemy base");
			scout->move(scoutedBase->getPosition());
			Broodwar->sendText("Removing found base");
			baseLocations.erase(*it);
		}
	}
}
