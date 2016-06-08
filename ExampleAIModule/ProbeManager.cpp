#include "ProbeManager.h"

/*
TODO:
- FIX: building two things in a row produces an error where the buildings want to be placed on same spot
*/

using namespace BWAPI;

void ProbeManager::onFrame(){

	executeQueue();
	nonIdle();

}


//Make sure that no-one is idle
void ProbeManager::nonIdle(){
	//Make idle mineralWorkers do stuff
	std::vector<Unit>::iterator it;
	for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
		Unit unit = *it;
		if (unit->exists() && unit->isIdle() && unit->getPlayer() == Broodwar->self()){
			if (unit->isCarryingGas() || unit->isCarryingMinerals()) {
				unit->returnCargo();
			}
			else if (!unit->gather(unit->getClosestUnit(Filter::IsMineralField))) {
				Broodwar << Broodwar->getLastError() << std::endl;
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

	if (queue.empty()){
		//If the queue is empty make sure to empty the the strategy
		StrategyManager::getInstance().setCurrentStrategy(StrategyManager::none);
		return;
	}

	//If front of queue is not a unit or a request this class doesn't have to do anything with it
	if (!queue.front().isUnit() && !queue.front().isRequest()){
		return;
	}

	//Find unit type
	UnitType type = queue.front().getUnitType();

	//Handle next building in queue
	if (type.isBuilding()){

		if (builder == NULL || !builder->exists()) {
			builder = mineralProbes.front(); //Assign a probe as designated builder
		}

		int minPrice = type.mineralPrice(); //Price of building
		int gasPrice = type.gasPrice(); //Price of building

		TilePosition position = getNewBuildLocation(type, builder->getTilePosition()); //Buildposition
		//TilePosition position = Broodwar->getBuildLocation(type, builder->getTilePosition()); //Buildposition

		if (!(Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice) &&
			!(Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice) &&
			!builder->isConstructing()){

			return; //If we dont have the resources to build the building
		}

		//// TESTING STARTED ////

		//const char* check = "False";
		//if (Broodwar->isVisible(position) && Broodwar->isBuildable(position)){
		//	check = "True";
		//}

		//Broodwar->drawTextMap(Position(position), check, Colors::Green);
		Broodwar->drawCircleMap(Position(position), 30, Colors::Purple, true);

		//// TESTING ENDED ////

			if (builder->build(type, position)){
				if (Broodwar->getLastError() != Errors::Unbuildable_Location ||
					Broodwar->getLastError() != Errors::Invalid_Tile_Position){

					Broodwar->registerEvent([position, type](Game*)
					{Broodwar->drawBoxMap(Position(position), Position(position + type.tileSize()), Colors::Yellow); }
					, nullptr, type.buildTime() + 100);

					ResourceManager::getInstance().reserveMinerals(type);
					ResourceManager::getInstance().reserveGas(type);
					queue.erase(queue.begin()); //Remove building from queue

				}
			}
		
	}

	//Handle next request in queue
	else if (queue.front().isRequest()){
		int request = queue.front().getRequestType();
		//Make a scout
		if (request == BuildOrderType::scoutRequest 
			&& !builder->isConstructing() && (builder->isIdle() || builder->isGatheringMinerals())){
			ScoutManager::getInstance().addScout(mineralProbes.front());

			// sets the builder to NULL if scout is builder
			if (builder == mineralProbes.front()){
				builder = NULL;
			}

			mineralProbes.erase(mineralProbes.begin());
			queue.erase(queue.begin()); //Remove the request from the queue

		}

		//Make a gasworker
		else if (request == BuildOrderType::gasworkerRequest){
			std::vector<Unit>::iterator it;
			for (it = mineralProbes.begin(); it != mineralProbes.end();){
				Unit unit = *it;
				if (unit->isGatheringMinerals() && unit != builder && Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Assimilator) >= 1){
					if (unit->gather(unit->getClosestUnit(Filter::IsRefinery))){
						mineralProbes.erase(it); //Remove probe from list by number in array
						gasProbes.push_back(unit); //Add unit to gasWorkerList
						queue.erase(queue.begin()); //Remove the request from the queue
					}
					else {
						Broodwar << Broodwar->getLastError() << std::endl;
					}
					break;
				}
				it++;
			}
		}

		// evalute the strategy
		else if (request == BuildOrderType::evaluateStrategyRequest){

			StrategyManager::getInstance().evaluateStrategies();

			queue.erase(queue.begin()); //Remove the request from the queue
		}
	}
}

TilePosition ProbeManager::getNewBuildLocation(UnitType type, TilePosition position){
	if (type == UnitTypes::Protoss_Photon_Cannon || 
		type == UnitTypes::Protoss_Gateway || 
		type == UnitTypes::Protoss_Pylon){

		//Cannons, Gateways and pylons are placed near the nearest chokepoint
		BWTA::Chokepoint* chokepoint = BWTA::getNearestChokepoint(position);
		TilePosition newPos = Broodwar->getBuildLocation(type, TilePosition(chokepoint->getCenter()));

		TilePosition ourBase = Broodwar->self()->getStartLocation();
		while (!(Broodwar->isVisible(newPos) && Broodwar->isBuildable(newPos))){
			//NOTE: We move in a straight line and might not be on the playing field after an iteration

			moveCloserTo(newPos, ourBase, 1);
}

		// Fencepost problem: we need to not be on the edge of vision, but fully in it.
		moveCloserTo(newPos, ourBase, 2);

		return Broodwar->getBuildLocation(type, newPos);
	} else {
		return Broodwar->getBuildLocation(type, position);
	}
}

//Function moves the first tileposition closer to the second one
void ProbeManager::moveCloserTo(TilePosition& moving, const TilePosition& stationary, int dist){
	if (moving.x > stationary.x){
		moving.x = moving.x - dist;
	}
	else {
		moving.x = moving.x + dist;
	}

	if (moving.y > stationary.y){
		moving.y = moving.y - dist;
	}
	else {
		moving.y = moving.y + dist;
	}
}

void ProbeManager::addMineralProbe(Unit probe){
	mineralProbes.push_back(probe);
}

//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}
