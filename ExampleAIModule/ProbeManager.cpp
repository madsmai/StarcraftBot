#include "ProbeManager.h"

/*
TODO:
- FIX: building two things in a row produces an error where the buildings want to be placed on same spot
*/

using namespace BWAPI;

void ProbeManager::onFrame(){
	//Construct the next building in the queue
	std::vector<BuildOrderType>& queue = BuildOrderManager::getInstance().getNewFixedOrderQueue();
	if (!queue.empty()){
		//Do things with the next unit in the buildOrder
		if (queue.front().isUnit()){
			BWAPI::UnitType type = queue.front().getUnitType(); //Find building type
			if (type.isBuilding()){
				if (builder == NULL || !builder->exists()) {
					builder = mineralProbes.front(); //Assign a probe as designated builder
				}
		BWAPI::UnitType type = queue.front(); //Find building type
				int minPrice = type.mineralPrice(); //Price of building
				int gasPrice = type.gasPrice(); //Price of building

		TilePosition position = Broodwar->getBuildLocation(type, builder->getTilePosition()); //Buildposition
		UnitType pylon = UnitTypes::Protoss_Pylon;
				if (BWAPI::Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice
					&& BWAPI::Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice
					&& (type == pylon || BWAPI::Broodwar->self()->completedUnitCount(pylon) >= 1)
					&& !builder->isConstructing()){
			

			if (builder->build(type, position)){

				Broodwar->registerEvent([position, type](BWAPI::Game*) {
					Broodwar->drawBoxMap(BWAPI::Position(position),
						Position(position + type.tileSize()),
						Colors::Yellow);
					},
						nullptr,
						type.buildTime() + 100);

					BWAPI::Broodwar << "building " << type << std::endl;
				ResourceManager::getInstance().reserveMinerals(queue.front());
				ResourceManager::getInstance().reserveGas(queue.front());
					queue.erase(queue.begin()); //Remove building from queue
				}
			
		}
		//Do things with the next request in the buildOrder
		if (queue.front().isRequest()){
			int request = queue.front().getRequestType();
	//Make a scout
			if (request == requests::scoutRequest && builder != NULL){
		ScoutManager::getInstance().addScout(builder);
		mineralProbes.erase(mineralProbes.begin());
				queue.erase(queue.begin()); //Remove the request from the queue
		builder = NULL;
	}
			//Make a gasworker
			else if (request == requests::gasworkerRequest){
		std::vector<BWAPI::Unit>::iterator it;
		for (it = mineralProbes.begin(); it != mineralProbes.end();){
			BWAPI::Unit unit = *it;
			if (unit->isGatheringMinerals()){
				mineralProbes.erase(it); //Remove probe from list by number in array
				gasProbes.push_back(unit); //Add unit to gasWorkerList
						queue.erase(queue.begin()); //Remove the request from the queue
				if (!unit->gather(unit->getClosestUnit(BWAPI::Filter::IsRefinery))){
					BWAPI::Broodwar << BWAPI::Broodwar->getLastError() << std::endl;
				}
				break;
			}
			else {
				it++;
			}
		}
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
		for (it = mineralProbes.begin(); it != mineralProbes.end();){
			if (*it == unit){
				mineralProbes.erase(it);
				break;
			}
			else {
				it++;
			}
		}

		//Loop through gasProbes
		for (it = gasProbes.begin(); it != gasProbes.end();){
			if (*it == unit){
				gasProbes.erase(it);
				break;
			}
			else {
				it++;
			}
		}
	}
}

//Add newly made worker to list
void ProbeManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == BWAPI::Broodwar->self()){
		addMineralProbe(unit);
	}
}


void ProbeManager::addMineralProbe(BWAPI::Unit probe){
	mineralProbes.push_back(probe);
}

//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}
