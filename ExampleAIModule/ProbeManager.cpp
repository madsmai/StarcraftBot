#include "ProbeManager.h"

/*
TODO:
- FIX: building two things in a row produces an error where the buildings want to be placed on same spot
- There might be null pointer on line 127 (where we get a new buildPosition), since we only sometimes get one
*/

using namespace BWAPI;

void ProbeManager::onFrame(){

	////DEBUGGING////
	const char* isConstr;
	if (builder != NULL){
		if (builder->isConstructing()){
			isConstr = "Constructing";
		}
		else{
			isConstr = "Mining/Moving";
		}
		Broodwar->drawTextMap(builder->getPosition(), isConstr);
	}

	//Broodwar->drawCircleMap(Position(buildingPosition), 30, Colors::Purple, true);
	////DEBUGGING////

	//If probe under attack, fight back
	std::vector<Unit>::iterator it;
	for (it = mineralProbes.begin(); it != mineralProbes.end(); it++){
		Unit unit = *it;
		if (unit->isUnderAttack() && AttackingProbes.size() < 2) {
			probeFightBack(unit);
		}
		else if (AttackingProbes.find(unit) != AttackingProbes.end() && (BWTA::getRegion(unit->getPosition()) != InformationManager::getInstance().ourBase->getRegion() || runFrames + 300 < Broodwar->getFrameCount())) {
			unit->gather(unit->getClosestUnit(Filter::IsMineralField));
			AttackingProbes.erase(AttackingProbes.find(unit));
		}
	}
	std::vector<Unit>::iterator itt;
	for (itt = gasProbes.begin(); itt != gasProbes.end(); itt++){
		Unit unit = *itt;
		if (unit->isUnderAttack() && AttackingProbes.size() < 2) {
			probeFightBack(unit);
		}
		else if (AttackingProbes.find(unit) != AttackingProbes.end() && (BWTA::getRegion(unit->getPosition()) != InformationManager::getInstance().ourBase->getRegion() || runFrames + 300 < Broodwar->getFrameCount())) {
			unit->gather(unit->getClosestUnit(Filter::IsMineralField));
			AttackingProbes.erase(AttackingProbes.find(unit));
		}
	}

	checkAndAddSupply();
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
	if (builder != NULL && builder->isIdle() && !builder->isMoving()) {
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

		if (builder->isConstructing()){
			return;
		}

		int minPrice = type.mineralPrice(); //Price of building
		int gasPrice = type.gasPrice(); //Price of building

		if (!(Broodwar->self()->minerals() - ResourceManager::getInstance().getReservedMinerals() >= minPrice &&
			Broodwar->self()->gas() - ResourceManager::getInstance().getReservedGas() >= gasPrice)){

			return; //If we dont have the resources to build the building
		}

		//Find new building position
		if(builderMoving == false){
			//buildingPosition = Broodwar->getBuildLocation(type, builder->getTilePosition());
			buildingPosition = PlacementManager::getInstance().getBuildingPlacement(type, builder->getTilePosition());
			const char* mess = "NEW LOCATION";
			Broodwar->drawTextMap(InformationManager::getInstance().ourBase->getPosition(), mess);
		}

		//// Debugging STARTED ////

		//Broodwar->drawCircleMap(Position(buildingPosition), 30, Colors::Purple, true);

		//// Debugging ENDED ////

		if (!Broodwar->isVisible(buildingPosition)){
			builder->move(Position(buildingPosition));
			builderMoving = true;
		}
		else if (builder->build(type, buildingPosition)){
			builderMoving = false;
			if (Broodwar->getLastError() != Errors::Unbuildable_Location ||
				Broodwar->getLastError() != Errors::Invalid_Tile_Position){
				
				/*
				Broodwar->registerEvent([buildingPosition, type](Game*)
				{Broodwar->drawBoxMap(Position(buildingPosition), Position(buildingPosition + type.tileSize()), Colors::Yellow); }
				, nullptr, type.buildTime() + 100);
				*/

				PlacementManager::getInstance().reserveSpace(type, buildingPosition);
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

/*TilePosition ProbeManager::getNewBuildLocation(UnitType type, TilePosition position){
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
			Broodwar->drawCircleMap(Position(newPos), 30, Colors::Green, false);
		}

		// Fencepost problem: we need to not be on the edge of vision, but fully in it.
		moveCloserTo(newPos, ourBase, 2);

		return Broodwar->getBuildLocation(type, newPos, 3); //dist of 3 is really good
	}
	else {
		return Broodwar->getBuildLocation(type, position);
	}
}*/

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


bool ProbeManager::checkAndAddSupply(){
	UnitType pylon = UnitTypes::Protoss_Pylon;
	static int lastChecked = 0;

	
	if (Broodwar->self()->supplyTotal() / 2 + Broodwar->self()->incompleteUnitCount(pylon) * 8 - 6
		<= Broodwar->self()->supplyUsed() / 2
		&& !InformationManager::getInstance().starter
		&& !builder->isConstructing()
		&& lastChecked + pylon.buildTime() < Broodwar->getFrameCount()){

		lastChecked = Broodwar->getFrameCount();
		std::vector<BuildOrderType>::iterator it;
		it = BuildOrderManager::getInstance().getNewFixedOrderQueue().begin();
		BuildOrderManager::getInstance().getNewFixedOrderQueue().insert(it, pylon);

		return true;
	}
	else {
		return false;
	}
}


//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}

void ProbeManager::probeFightBack(Unit attackedUnit) {
	InformationManager::getInstance().writeToLog("Started probeFightBack");
	if (attackedUnit != NULL) {
		Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking);
		if (attacker != NULL && !attackedUnit->isAttacking()) {
			if (attacker->getType().isWorker()) {
				attackedUnit->attack(attacker);

				if (AttackingProbes.find(attackedUnit) == AttackingProbes.end()) {
					AttackingProbes.insert(attackedUnit);
				}

				Unit helper = attackedUnit->getClosestUnit(Filter::IsAlly
					&& Filter::CanAttack);

				if (helper != NULL
					&& attacker != NULL
					&& attacker->isVisible()) {

					helper->attack(attacker);

					if (AttackingProbes.find(helper) == AttackingProbes.end()) {
						AttackingProbes.insert(helper);
					}
				}
				runFrames = Broodwar->getFrameCount();
			}
			else {
				Unit helper = attackedUnit->getClosestUnit(Filter::IsAlly
					&& Filter::CanAttack && !Filter::IsWorker);

				if (helper != NULL
					&& attacker != NULL
					&& attacker->isVisible()) {

					attackedUnit->move(helper->getPosition());
					helper->attack(attacker);
				}
			}
		}
	}
}
