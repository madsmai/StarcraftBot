#include "ProbeManager.h"

/*
TODO:
- FIX: building two things in a row produces an error where the buildings want to be placed on same spot
*/

using namespace BWAPI;

void ProbeManager::onFrame(){

	executeQueue();

	//Make idle mineralWorkers do stuff
	std::vector<Unit>::iterator it;
	for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
		Unit unit = *it;
		if (unit->exists() && unit->isIdle() && unit->getPlayer() == Broodwar->self()){
			if (unit->isCarryingGas() || unit->isCarryingMinerals()) {
				unit->returnCargo();
			}
			else if (!unit->gather(unit->getClosestUnit(Filter::IsMineralField))) {
				//Broodwar << Broodwar->getLastError() << std::endl;
			}
		}
	}

	//Make an idle builder do stuff
	if (builder != NULL && builder->isIdle()) {
		if (!builder->gather(builder->getClosestUnit(Filter::IsMineralField))) {
			Broodwar << Broodwar->getLastError() << std::endl;
		}
	}

	//Make idle gasWorkers do stuff
	for (it = gasProbes.begin(); it != gasProbes.end(); it++){
		Unit u = *it;
		if (u->exists() && (u->isIdle() || u->isGatheringMinerals()) && u->getPlayer() == Broodwar->self()){
			if (u->isCarryingGas() || u->isCarryingMinerals()) {
				u->returnCargo();
			}
			else if (!u->gather(u->getClosestUnit(Filter::IsRefinery))){
				Broodwar << Broodwar->getLastError() << std::endl;
			}
		}
	}
}






//Remove destroyed worker
void ProbeManager::onUnitDestroy(Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == Broodwar->self()){
		std::vector<Unit>::iterator it;

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
void ProbeManager::onUnitComplete(Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == Broodwar->self()){
		addMineralProbe(unit);
	}
}


void ProbeManager::executeQueue(){

	std::vector<BuildOrderType>& queue = BuildOrderManager::getInstance().getNewFixedOrderQueue();
	if (!queue.empty()){
		//Do things with the next unit in the buildOrder
		if (queue.front().isUnit()){
			UnitType type = queue.front().getUnitType(); //Find building type
			if (type.isBuilding()){
				if (builder == NULL || !builder->exists()) {
					builder = mineralProbes.front(); //Assign a probe as designated builder
				}
				int minPrice = type.mineralPrice(); //Price of building
				int gasPrice = type.gasPrice(); //Price of building

				TilePosition position = Broodwar->getBuildLocation(type, builder->getTilePosition()); //Buildposition
				UnitType pylon = UnitTypes::Protoss_Pylon;
				if (Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice
					&& Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice
					&& !builder->isConstructing()){

					if (builder->build(type, position)){
						if (Broodwar->getLastError() != Errors::Unbuildable_Location || Broodwar->getLastError() != Errors::Invalid_Tile_Position){
							Broodwar->registerEvent([position, type](Game*) {
								Broodwar->drawBoxMap(Position(position),
									Position(position + type.tileSize()),
									Colors::Yellow);
							}, nullptr, type.buildTime() + 100);

							Broodwar << "building " << type << std::endl;
							ResourceManager::getInstance().reserveMinerals(type);
							ResourceManager::getInstance().reserveGas(type);
							queue.erase(queue.begin()); //Remove building from queue

						}
					}
				}
			}
		}

		//Do things with the next request in the buildOrder
		if (queue.front().isRequest()){
			int request = queue.front().getRequestType();
			//Make a scout
			if (request == BuildOrderType::scoutRequest
				&& !builder->isConstructing()){
				ScoutManager::getInstance().addScout(mineralProbes.front());

				// sets the builder to NULL if scout is builder
				if (builder == mineralProbes.front()){
					builder = NULL;
				}

				mineralProbes.erase(mineralProbes.begin());
				queue.erase(queue.begin()); //Remove the request from the queue
				Broodwar << "Removed a scout request";

			}

			//Make a gasworker
			else if (request == BuildOrderType::gasworkerRequest){
				std::vector<Unit>::iterator it;
				for (it = mineralProbes.begin(); it != mineralProbes.end();){
					Unit unit = *it;
					if (unit->isGatheringMinerals() && unit != builder){
						if (unit->gather(unit->getClosestUnit(Filter::IsRefinery))){
							mineralProbes.erase(it); //Remove probe from list by number in array
							gasProbes.push_back(unit); //Add unit to gasWorkerList
							queue.erase(queue.begin()); //Remove the request from the queue
							Broodwar << "Removed a gasworker request";
						}
						else {
							Broodwar << Broodwar->getLastError() << std::endl;
						}
						break;
					}
					it++;
				}
			}
		}
	} //End of !queue.empty()

}

void ProbeManager::addMineralProbe(Unit probe){
	mineralProbes.push_back(probe);
}

//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}
