#include "ScoutManager.h"
#include "ProbeManager.h"

/*
TODO:
- Scouten bevæger sig ud og scouter igen. Nok pga onDiscover
- Scouten laver ikke noget når den kommer tilbage
*/

using namespace BWAPI;

void ScoutManager::onFrame(){


	std::vector<BWAPI::Unit>::iterator it;
	if (!inactiveScouts.empty()){
		//Broodwar << "inactive scouts was not empty" << std::endl;
		for (BWAPI::Unit unit : inactiveScouts){
			goScout(unit);
			activeScouts.push_back(unit);
		}
		inactiveScouts.clear();
	}

	if (!activeScouts.empty()){

		for (BWAPI::Unit unit : activeScouts){
			if (unit->isUnderAttack()
				&& doneScouting) {
				retreating = true;
				unit->move(InformationManager::getInstance().ourBase->getPosition());
			}
			else if (unit->isIdle()
				&& BWTA::getNearestBaseLocation(unit->getPosition()) == InformationManager::getInstance().ourBase){
				removeScout(unit);
				//Broodwar << "removed unit as scout" << std::endl;
				ProbeManager::getInstance().addMineralProbe(unit);
			}
			else if (unit->isIdle()
				&& BWTA::getNearestBaseLocation(unit->getPosition()) == InformationManager::getInstance().emptyMainBase){
				double minDistance = 9999999;
				std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
				for (it = InformationManager::getInstance().baseLocations.begin(); it != InformationManager::getInstance().baseLocations.end(); ++it){
					InformationManager::getInstance().nextBase = *it;
					// Remove main base if it is empty
					if (unit->getPosition() == InformationManager::getInstance().nextBase->getPosition()){
						InformationManager::getInstance().currentBase = InformationManager::getInstance().nextBase;
						InformationManager::getInstance().baseLocations.erase(*it);
						//Broodwar->sendText("Removed empty main base");
					}
					else if (InformationManager::getInstance().currentBase->getGroundDistance(InformationManager::getInstance().nextBase) < minDistance
						&& BWTA::getRegion(unit->getTilePosition()) != BWTA::getRegion(InformationManager::getInstance().nextBase->getPosition())) {
						minDistance = InformationManager::getInstance().currentBase->getGroundDistance(InformationManager::getInstance().nextBase);
						//Broodwar->sendText("Moving to enemy base");
						unit->move(InformationManager::getInstance().nextBase->getPosition());
					}
				}
			}
		}
	}
}

void ScoutManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == Broodwar->self()){
		removeScout(unit);

		if (InformationManager::getInstance().enemyBase == NULL){
			std::vector<BuildOrderType>::iterator it;
			it = BuildOrderManager::getInstance().getNewFixedOrderQueue().begin();
			BuildOrderManager::getInstance().getNewFixedOrderQueue().insert(it, BuildOrderType::scoutRequest);
		}
	}

	if (!activeScouts.empty()){
		if (unit->getType().isWorker()
			&& unit->getPlayer()->isEnemy(Broodwar->self())
			&& !retreating){

			BWAPI::Unit scout = activeScouts.front();
			UnitCommand currentCommand = scout->getLastCommand();

			// ignore if the scout already is attacking
			if (currentCommand.getType() == UnitCommandTypes::Attack_Unit
				&& currentCommand.getTarget()->exists()){
				return;
			}

			BWAPI::Unit closestEnemy = scout->getClosestUnit(Filter::IsWorker && Filter::IsEnemy);
			if (!closestEnemy){
				// no workers, getting another closest target
				closestEnemy = scout->getClosestUnit(Filter::IsEnemy);
			}
			scout->attack(closestEnemy);
		}
	}
}

void ScoutManager::onUnitDiscover(BWAPI::Unit unit){
	if (!activeScouts.empty()){
		if (unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			//Broodwar->sendText("Test enemy base scout");

			InformationManager::getInstance().enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			std::vector<BWAPI::Unit>::iterator it;
			for (it = activeScouts.begin(); it != activeScouts.end(); it++) { //Change where the active scouts are going
				BWAPI::Unit u = *it;
				u->move(unit->getPosition());
			}
			//Broodwar->sendText("Done scouting, found mainbase");
			doneScouting = true;
		}

		if (unit->getType().isWorker()
			&& unit->getPlayer()->isEnemy(Broodwar->self())
			&& !retreating
			&& doneScouting){

			BWAPI::Unit scout = activeScouts.front();
			UnitCommand currentCommand = scout->getLastCommand();

			// ignore if the scout already is attacking
			if (currentCommand.getType() == UnitCommandTypes::Attack_Unit){
				return;
			}
			scout->attack(unit);
		}

		else if (unit->getPlayer() != Broodwar->self()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& !unit->getPlayer()->isEnemy(Broodwar->self())) {

			InformationManager::getInstance().emptyMainBase = BWTA::getNearestBaseLocation(unit->getPosition());

		}
		else if (unit->getPlayer() != Broodwar->self()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {
			InformationManager::getInstance().expansion = BWTA::getNearestBaseLocation(unit->getPosition());
		}
	}
}

void ScoutManager::goScout(BWAPI::Unit scout){
	InformationManager::getInstance().baseLocations = BWTA::getStartLocations();
	double minDistance = 9999999;

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = InformationManager::getInstance().baseLocations.begin(); it != InformationManager::getInstance().baseLocations.end(); ++it){
		InformationManager::getInstance().nextBase = *it;
		// Remove our baselocation
		if (InformationManager::getInstance().nextBase->getPosition() == InformationManager::getInstance().ourBase->getPosition()
			|| scout->getPosition() == InformationManager::getInstance().nextBase->getPosition()){
			InformationManager::getInstance().baseLocations.erase(*it);
			//Broodwar->sendText("Removed our base");
		}
		else if (InformationManager::getInstance().ourBase->getGroundDistance(InformationManager::getInstance().nextBase) < minDistance) {
			minDistance = InformationManager::getInstance().ourBase->getGroundDistance(InformationManager::getInstance().nextBase);
			//Broodwar->sendText("Moving to enemy base");
			scout->move(InformationManager::getInstance().nextBase->getPosition());
		}
	}
}

void ScoutManager::addScout(BWAPI::Unit scout){
	inactiveScouts.push_back(scout);
	//Broodwar->sendText("Added scout to inactive scouts");
	//Broodwar << inactiveScouts.front()->getType().getName() << std::endl;
	//Broodwar << inactiveScouts.size() << std::endl;
}

void ScoutManager::removeScout(BWAPI::Unit scout){
	std::vector<BWAPI::Unit>::iterator it;
	for (it = activeScouts.begin(); it != activeScouts.end();) { //If it is in activeScout
		BWAPI::Unit u = *it;
		if (u->getID() == scout->getID()){
			activeScouts.erase(it);
		}
		else {
			it++;
		}
	}
	for (it = inactiveScouts.begin(); it != inactiveScouts.end();) { //If it is in inactiveScouts
		BWAPI::Unit u = *it;
		if (u->getID() == scout->getID()){
			inactiveScouts.erase(it);
		}
		else {
			it++;
		}
	}
}


bool ScoutManager::checkAgain(){
	static int lastChecked = 0;
	if (lastChecked + 600 < BWAPI::Broodwar->getFrameCount()
		&& scoutSent && activeScouts.empty() && inactiveScouts.empty()
		&& !doneScouting){

		// a scout was sent but never got to the enemy base. Make a new request.
		lastChecked = Broodwar->getFrameCount();
		
		// sending a new scout!
		BuildOrderManager::getInstance().getNewFixedOrderQueue().insert
			(BuildOrderManager::getInstance().getNewFixedOrderQueue().begin(), BuildOrderType::requests::scoutRequest);
		return true;
	}
	return false;


}

ScoutManager& ScoutManager::getInstance(){ //Return ref to ScoutManager object
	static ScoutManager i; //Make static instance i
	return i;
}