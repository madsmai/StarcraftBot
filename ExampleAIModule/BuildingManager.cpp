#include "BuildingManager.h"

/*
TODO:
- Hvis en bygning ikke eksisterer så units/upgrade ikke kan laves, bliver køen stoppet til
*/

void BuildingManager::onFrame(){
	//Construct the next unit in the queue
	std::queue<BWAPI::UnitType>& queue = BuildOrderManager::getInstance().getFixedOrderQueue();
	if (!queue.empty() && !queue.front().isBuilding()){
		
		BWAPI::UnitType type = queue.front(); //Type of unit
		int minPrice = type.mineralPrice(); //Price of unit
		int gasPrice = type.gasPrice(); //Price of unit

		std::vector<BWAPI::Unit>::iterator it;
		for (it = buildings.begin(); it != buildings.end(); it++){
			BWAPI::Unit unit = *it;
			if (unit->canTrain(type) && 
				unit->isIdle() &&
				BWAPI::Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice &&
				BWAPI::Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice) {

				unit->train(type);
				queue.pop();
			}
		}
	}

	//Reasearch to the next upgrade in the queue
	if (!pendingUpgrades.empty()){
		BWAPI::UpgradeType type = pendingUpgrades.front();
		int minPrice = type.mineralPrice(); //Price of unit
		int gasPrice = type.gasPrice(); //Price of unit

		std::vector<BWAPI::Unit>::iterator it;
		for (it = buildings.begin(); it != buildings.end(); it++){
			BWAPI::Unit unit = *it;
			if (unit->canUpgrade(type) &&
				unit->isIdle() &&
				BWAPI::Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice &&
				BWAPI::Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice){
				
				unit->upgrade(type);
				pendingUpgrades.pop();
			}
		}
	}
}

void BuildingManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		std::vector<BWAPI::Unit>::iterator it;

		//Loop through buildings
		for (it = buildings.begin(); it != buildings.end(); it++){
			if (*it == unit){
				buildings.erase(it);
				break;
			}
		}
	}
}

void BuildingManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		buildings.push_back(unit);
	}
}

//void BuildingManager::addUnit(BWAPI::UnitType type){
//	if (!type.isBuilding()){
//		pendingUnits.push(type);
//	}
//}

void BuildingManager::addUpgrade(BWAPI::UpgradeType type){
	pendingUpgrades.push(type);
}

BuildingManager& BuildingManager::getInstance(){ //Return ref to BuildingManager object
	static BuildingManager i; //Make static instance i
	return i;
}
