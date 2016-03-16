#include "ProbeManager.h"

/*
TODO:
- metode makeScout:
	- Sende en probe til ScoutManager og sletter probe fra egen liste, hvis bedt om det
*/

void ProbeManager::onFrame(){
	//Construct the next building in the queue
	if (!pendingBuildings.empty()){
		BWAPI::Unit unit = probes.front(); //Get a probe
		BWAPI::UnitType type = pendingBuildings.front(); //Find building type
		int price = type.mineralPrice(); //Price of building
		BWAPI::TilePosition position = BWAPI::Broodwar->getBuildLocation(type, unit->getTilePosition()); //Buildposition

		if (BWAPI::Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() > price){
			unit->build(type, position);
			ResourceManager::getInstance().reserveMinerals(pendingBuildings.front());
			pendingBuildings.pop(); //Remove building from queue
		}
		else { //IF we don't have enough minerals
			BWAPI::Broodwar->sendText("Not enough minerals");
		}
	}

	//Make idle workers do stuff
	std::vector<BWAPI::Unit>::iterator it;
	for (it = probes.begin(); it != probes.end(); it++){
		BWAPI::Unit unit = *it;
		if (unit->exists() && unit->isIdle() && unit->getPlayer() == BWAPI::Broodwar->self()){
			unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
		}
	}
}

//Remove destroyed worker
void ProbeManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == BWAPI::Broodwar->self()){
		std::vector<BWAPI::Unit>::iterator it;
		for (it = probes.begin(); it != probes.end(); it++){
			if (*it == unit){
				probes.erase(it);
				break;
			}
		}
	}
}

//Add newly made worker to list
void ProbeManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == BWAPI::Broodwar->self()){
		probes.push_back(unit);
	}
}

//Add the first 4 probes to list
void ProbeManager::onStart(){
	for (auto &unit : BWAPI::Broodwar->self()->getUnits()) {
		if (unit->exists()		&&	  unit->getType().isWorker()){
			probes.push_back(unit);
		}
	}
}

//Remove worker from list and put into ScoutManager list
bool ProbeManager::becomeScout(){

	return true;
}

//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}

//Add building to queue
void ProbeManager::addBuilding(BWAPI::UnitType type){
	if (type.isBuilding()){
		pendingBuildings.push(type);
	}
}