#include "ProbeManager.h"

/*
TODO:

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
			buildingPosition = PlacementManager::getInstance().getBuildingPlacement(type, builder->getTilePosition());
		}

		if (!Broodwar->isVisible(buildingPosition)){
			builder->move(Position(buildingPosition));
			builderMoving = true;
		}
		else if (builder->build(type, buildingPosition)){ //MOVE THIS AROUND
			builderMoving = false;
			if (Broodwar->getLastError() != Errors::Unbuildable_Location ||
				Broodwar->getLastError() != Errors::Invalid_Tile_Position){

				PlacementManager::getInstance().reserveSpace(type, buildingPosition);
				ResourceManager::getInstance().reserveMinerals(type);
				ResourceManager::getInstance().reserveGas(type);
				queue.erase(queue.begin()); //Remove building from queue
				return;
			}
		}
	}

	//Handle next request in queue
	else if (queue.front().isRequest()){
		int request = queue.front().getRequestType();
		//Make a scout

		if (request == BuildOrderType::scoutRequest && builder != NULL
			&& builder->getLastCommand().getType() != UnitCommandTypes::Build){

			ScoutManager::getInstance().scoutSent = true;
			ScoutManager::getInstance().addScout(mineralProbes.front());

			// sets the builder to NULL if scout is builder
			if (builder == mineralProbes.front()){
				builder = NULL;
			}

			mineralProbes.erase(mineralProbes.begin());
			
				queue.erase(queue.begin()); //Remove the request from the queue
				return;
			

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
						return;
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
			return;
		}
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
		&& (!InformationManager::getInstance().starter 
		|| Broodwar->getLastError() == BWAPI::Errors::Insufficient_Supply) // something went wrong in a starter
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
