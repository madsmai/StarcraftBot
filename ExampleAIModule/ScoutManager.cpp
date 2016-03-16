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


ScoutManager::ScoutManager()
{
}


ScoutManager::~ScoutManager()
{
}

void ScoutManager::onFrame(){

}

void ScoutManager::onUnitDestroy(BWAPI::Unit unit){

}

void ScoutManager::onUnitDiscover(BWAPI::Unit unit){
	if (scout != NULL){
		if (unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			scout->attack(unit);
			Broodwar->sendText("Done scouting, found mainbase");

		}

		else if (unit->getPlayer() != Broodwar->self() && unit->getType().isResourceDepot()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {

			expansion = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			Broodwar->sendText("Found expansion");
			goScout(scout);
		}
	}
}



void ScoutManager::goScout(BWAPI::Unit scout){
	baseLocations = BWTA::getStartLocations();
	double minDistance = 9999999;

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = baseLocations.begin(); it != baseLocations.end(); ++it){
		scoutedBase = *it;
		// Remove our baselocation
		if (scoutedBase->getPosition() == ourBase->getPosition()){
			baseLocations.erase(*it);
			Broodwar->sendText("Removed our base");
		}	
		else if (ourBase->getGroundDistance(scoutedBase) < minDistance) {
			minDistance = ourBase->getGroundDistance(scoutedBase);
			Broodwar->sendText("Moving to enemy base");
			scout->move(scoutedBase->getPosition());
			baseLocations.erase(*it);
		}	
		else if (scout->isUnderAttack()) {
			scout->move(ourBase->getPosition());
		}
	}
}

ScoutManager& ScoutManager::getInstance(){ //Return ref to ScoutManager object
	static ScoutManager i; //Make static instance i
	return i;
}