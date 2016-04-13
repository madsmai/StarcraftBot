#include "ScoutManager.h"

/*
TODO:
- Scouten bevæger sig ud og scouter igen. Nok pga onDiscover
- Scouten laver ikke noget når den kommer tilbage
*/

using namespace BWAPI;

void ScoutManager::onFrame(){
	std::vector<BWAPI::Unit>::iterator it;
	if (!inactiveScouts.empty()){
		Broodwar << "inactive scouts was not empty" << std::endl;
		for (BWAPI::Unit unit : inactiveScouts){
			goScout(unit);
			activeScouts.push_back(unit);
		}
		inactiveScouts.clear();
	}

	if (!activeScouts.empty()){
		for (BWAPI::Unit unit : activeScouts){
			if (unit->isUnderAttack()) {
				unit->move(InformationManager::getInstance().ourBase->getPosition());
			}
			else if (unit -> isIdle()
				&& BWTA::getNearestBaseLocation(unit->getPosition()) == InformationManager::getInstance().ourBase){
				removeScout(unit);
				Broodwar << "removed unit as scout" << std::endl;
				ProbeManager::getInstance().addMineralProbe(unit);
			}
		}
	}
}

void ScoutManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isWorker() && unit->getPlayer() == Broodwar->self()){
		removeScout(unit);
	}
}

void ScoutManager::onUnitDiscover(BWAPI::Unit unit){
	if (!activeScouts.empty()){
		if (unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			Broodwar->sendText("Test enemy base scout");

			InformationManager::getInstance().enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			std::vector<BWAPI::Unit>::iterator it;
			for (it = activeScouts.begin(); it != activeScouts.end(); it++) { //Change where the active scouts are going
				BWAPI::Unit u = *it;
				u->move(unit->getPosition());
				u->attack(unit);
			}
			Broodwar->sendText("Done scouting, found mainbase");
		}

		else if (unit->getPlayer() != Broodwar->self() 
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& !unit->getPlayer()->isEnemy(Broodwar->self())) {

			Broodwar->sendText("Test empty start base");

			InformationManager::getInstance().enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			std::vector<BWAPI::Unit>::iterator it;
			for (it = activeScouts.begin(); it != activeScouts.end(); it++) { //Change where the active scouts are going
				BWAPI::Unit u = *it;
				u->move(unit->getPosition());
				goScout(u);
			}

			Broodwar->sendText("Found Empty base");
		}

		else if (unit->getPlayer() != Broodwar->self()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {

			Broodwar->sendText("Test expasion");

			//InformationManager::getInstance().expansion = BWTA::getNearestBaseLocation(unit->getPosition());
			//std::vector<BWAPI::Unit>::iterator it;
			//for (it = activeScouts.begin(); it != activeScouts.end(); it++) { //Change where the active scouts are going
			//	BWAPI::Unit u = *it;
			//	u->move(unit->getPosition());
			//	goScout(u);
			//}
			Broodwar->sendText("Found expansion");
		}
	}
}

void ScoutManager::goScout(BWAPI::Unit scout){
	InformationManager::getInstance().baseLocations = BWTA::getStartLocations();
	double minDistance = 9999999;

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = InformationManager::getInstance().baseLocations.begin(); it != InformationManager::getInstance().baseLocations.end(); ++it){
		InformationManager::getInstance().scoutedBase = *it;
		// Remove our baselocation
		if (InformationManager::getInstance().scoutedBase->getPosition() == InformationManager::getInstance().ourBase->getPosition()){
			InformationManager::getInstance().baseLocations.erase(*it);
			Broodwar->sendText("Removed our base");
		}	
		else if (InformationManager::getInstance().ourBase->getGroundDistance(InformationManager::getInstance().scoutedBase) < minDistance) {
			minDistance = InformationManager::getInstance().ourBase->getGroundDistance(InformationManager::getInstance().scoutedBase);
			Broodwar->sendText("Moving to enemy base");
			scout->move(InformationManager::getInstance().scoutedBase->getPosition());
			InformationManager::getInstance().baseLocations.erase(*it);
		}	
	}
}

void ScoutManager::addScout(BWAPI::Unit scout){
	inactiveScouts.push_back(scout);
	BWAPI::Broodwar->sendText("Added scout to inactive scouts");
	BWAPI::Broodwar << inactiveScouts.front()->getType().getName() << std::endl;
	BWAPI::Broodwar << inactiveScouts.size() << std::endl;
}

void ScoutManager::removeScout(BWAPI::Unit scout){
	std::vector<BWAPI::Unit>::iterator it;
	for (it = activeScouts.begin(); it != activeScouts.end(); ) { //If it is in activeScout
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

ScoutManager& ScoutManager::getInstance(){ //Return ref to ScoutManager object
	static ScoutManager i; //Make static instance i
	return i;
}