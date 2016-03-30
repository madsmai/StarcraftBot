#include "ScoutManager.h"

/*
TODO:
- Liste af alle scouts der ikke laver noget
- Liste af alle scouts der laver noget
- metode onFrame:
	- Alle units der ikke scouter bliver sendt ud for at scoute
- metode onUnitDestroy:
	- Fjern unit fra listen hvis den er der
- metode onUnitComplete:
	- Sætter completed unit ind i liste, hvis den hører til
*/

using namespace BWAPI;

void ScoutManager::onFrame(){

}

void ScoutManager::onUnitDestroy(BWAPI::Unit unit){

	if (unit->getType().isWorker()){
		removeScout(unit);
	}

}

void ScoutManager::onUnitDiscover(BWAPI::Unit unit){
	if (scout != NULL){
		if (unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			InformationManager::getInstance().enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			scout->attack(unit);
			Broodwar->sendText("Done scouting, found mainbase");

		}

		else if (unit->getPlayer() != Broodwar->self() && unit->getType().isResourceDepot()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {

			InformationManager::getInstance().expansion = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			Broodwar->sendText("Found expansion");
			goScout(scout);
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
		else if (scout->isUnderAttack()) {
			scout->move(InformationManager::getInstance().ourBase->getPosition());
		}
	}
}

void ScoutManager::addScout(BWAPI::Unit scout){
	scouts.push_back(scout);
}

void ScoutManager::removeScout(BWAPI::Unit scout){
	std::vector<BWAPI::Unit>::iterator it;
	for (it = scouts.begin(); it != scouts.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == scout->getID()){
			scouts.erase(it);
		}
	}
}

ScoutManager& ScoutManager::getInstance(){ //Return ref to ScoutManager object
	static ScoutManager i; //Make static instance i
	return i;
}