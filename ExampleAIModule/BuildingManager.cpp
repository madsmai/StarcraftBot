#include "BuildingManager.h"

/*
TODO:
- Hvis en bygning ikke eksisterer s� units/upgrade ikke kan laves, bliver k�en stoppet til
*/

void BuildingManager::onFrame(){
	//Construct the next unit in the queue
	std::vector<BuildOrderType>& queue = BuildOrderManager::getInstance().getNewFixedOrderQueue();
	if (!queue.empty()){
		if (queue.front().isUnit()){
			BWAPI::UnitType type = queue.front().getUnitType(); //Find unit type
			if (!type.isBuilding()){
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
						queue.erase(queue.begin());
					}
				}
			}
		}
		//Reasearch to the next upgrade in the queue
		if (queue.front().isUpgrade()){
			BWAPI::UpgradeType type = queue.front().getUpgradeType();
			int minPrice = type.mineralPrice(); //Price of unit
			int gasPrice = type.gasPrice(); //Price of unit

			std::vector<BWAPI::Unit>::iterator it;
			for (it = buildings.begin(); it != buildings.end(); it++){
				BWAPI::Unit unit = *it;
				if (unit->isCompleted()
					&& unit->canUpgrade(type)
					&& unit->isIdle()
					&& Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice
					&& Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice){

					unit->upgrade(type);
					queue.erase(queue.begin());
				}
			}
		}
		if (queue.front().isTech()){
			BWAPI::TechType type = queue.front().getTechType();
			int minPrice = type.mineralPrice(); //Price of unit
			int gasPrice = type.gasPrice(); //Price of unit

			std::vector<BWAPI::Unit>::iterator it;
			for (it = buildings.begin(); it != buildings.end(); it++){
				BWAPI::Unit unit = *it;
				if (unit->isCompleted()
					&& unit->canUpgrade(type)
					&& unit->isIdle()
					&& Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice
					&& Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice){

					unit->research(type);
					queue.erase(queue.begin());
				}
			}
		}
	}
}

void BuildingManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		std::vector<BWAPI::Unit>::iterator it;

		//Loop through buildings
		for (it = buildings.begin(); it != buildings.end();){
			if (*it == unit){
				buildings.erase(it);
				break;
			}
			else {
				it++;
			}
		}
	}
}

void BuildingManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		buildings.push_back(unit);
	}
}

BuildingManager& BuildingManager::getInstance(){ //Return ref to BuildingManager object
	static BuildingManager i; //Make static instance i
	return i;
}
