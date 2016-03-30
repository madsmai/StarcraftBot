#include "ProbeManager.h"

/*
TODO:
- FIX: building two things in a row produces an error where the buildings want to be placed on same spot
*/

void ProbeManager::onFrame(){
	//Construct the next building in the 





	std::queue<BWAPI::UnitType>& queue = BuildOrderManager::getInstance().getFixedOrderQueue();
	if (!queue.empty() && queue.front().isBuilding()){
		if (builder == NULL || !builder->exists()) {
			builder = mineralProbes.front(); //Assign a probe as designated builder
		}
		BWAPI::UnitType type = queue.front(); //Find building type
		int minPrice = type.mineralPrice(); //Price of building
		int gasPrice = type.gasPrice(); //Price of building

		BWAPI::TilePosition position = BWAPI::Broodwar->getBuildLocation(type, builder->getTilePosition()); //Buildposition
		BWAPI::UnitType pylon = BWAPI::UnitTypes::Protoss_Pylon;
		if (BWAPI::Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice
			&& BWAPI::Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice
			&& (type == pylon || BWAPI::Broodwar->self()->completedUnitCount(pylon) >= 1)
			&& !builder->isConstructing()){
			BWAPI::Broodwar->registerEvent([position, type](BWAPI::Game*) {
				BWAPI::Broodwar->drawBoxMap(BWAPI::Position(position),
					BWAPI::Position(position + type.tileSize()),
					BWAPI::Colors::Yellow);
			},
				nullptr,
				type.buildTime() + 100);
			BWAPI::Broodwar << "building " << type << std::endl;
			builder->build(type, position);
			ResourceManager::getInstance().reserveMinerals(queue.front());
			queue.pop(); //Remove building from queue
		}
	}

	//Make a scout
	if (!scoutRequests == 0){
		std::vector<BWAPI::Unit>::iterator it;
		for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
			BWAPI::Unit unit = *it;
			if (unit->isGatheringMinerals()){
				mineralProbes.erase(it); //Remove probe from list
				scoutRequests--; //Remove a scoutRequest
				ScoutManager::getInstance().addScout(unit); //Add a scout
				break;
			}
		}
	}

	//Make a gas worker
	if (!gasWorkerRequests == 0){
		std::vector<BWAPI::Unit>::iterator it;
		for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
			BWAPI::Unit unit = *it;
			if (unit->isGatheringMinerals()){
				mineralProbes.erase(it); //Remove probe from list by number in array
				gasWorkerRequests--; //Remove a  gasWorkerRequest
				gasProbes.push_back(unit); //Add unit to gasWorkerList
				break;
			}
		}
	}

	//Make idle mineralWorkers do stuff
	std::vector<BWAPI::Unit>::iterator it;
	for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
		BWAPI::Unit unit = *it;
		if (unit->exists() && unit->isIdle() && unit->getPlayer() == BWAPI::Broodwar->self()){
			if (unit->isCarryingGas() || unit->isCarryingMinerals()) {
				unit->returnCargo();
			}
			else if (!unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField))) {
				BWAPI::Broodwar << BWAPI::Broodwar->getLastError() << std::endl;
			}
		}
	}
	if (builder != NULL && builder->isIdle()) {
		if (!builder->gather(builder->getClosestUnit(BWAPI::Filter::IsMineralField))) {
			BWAPI::Broodwar << BWAPI::Broodwar->getLastError() << std::endl;
		}
	}

	//Make idle gasWorkers do stuff
	for (it = gasProbes.begin(); it != gasProbes.end(); it++){
		BWAPI::Unit u = *it;
		if (u->exists() && u->isIdle() && u->getPlayer() == BWAPI::Broodwar->self()){
			if (u->isCarryingGas() || u->isCarryingMinerals()) {
				u->returnCargo();
			}
			if (!u->gather(u->getClosestUnit(BWAPI::Filter::IsRefinery))){
				BWAPI::Broodwar << BWAPI::Broodwar->getLastError() << std::endl;
			}

		}
	}
}

//Remove destroyed worker
void ProbeManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == BWAPI::Broodwar->self()){
		std::vector<BWAPI::Unit>::iterator it;

		//Loop through mineralsProbes
		for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
			if (*it == unit){
				mineralProbes.erase(it);
				break;
			}
		}

		//Loop through gasProbes
		for (it = gasProbes.begin(); it != gasProbes.end(); it++){
			if (*it == unit){
				gasProbes.erase(it);
				break;
			}
		}
	}
}

//Add newly made worker to list
void ProbeManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == BWAPI::Broodwar->self()){
		mineralProbes.push_back(unit);
	}
}


//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}

//Add scoutRequest
void ProbeManager::addScoutRequest(){
	scoutRequests++;
}

//Add gasWorkerRequest
void ProbeManager::addGasWorkerRequest(){
	gasWorkerRequests++;
}

