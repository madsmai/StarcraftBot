#include "OffenseManager.h"
#include "InformationManager.h"

using namespace BWAPI;
/*
TODO:
- Sørg for at samle zealots sammen og angrib med dem sammen, det samlede antal skal varierer
	alt efter modstanderens forsvar
- En måde at få zealots til at prioriterer hvad de angriber
- Mangler BWTA
*/


void OffenseManager::onUnitDestroy(BWAPI::Unit unit){
	//if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->getPlayer() == BWAPI::Broodwar->self()){
	//	std::vector<BWAPI::Unit>::iterator it;

	//	//Loop through zealots
	//	for (it = zealots.begin(); it != zealots.end(); it++){
	//		if (*it == unit){
	//			zealots.erase(it);
	//			break;
	//		}
	//	}
	//}

	if (fighters.find(unit) != fighters.end()) {
		fighters.erase(fighters.find(unit));
	}

}

void OffenseManager::onUnitComplete(BWAPI::Unit unit){
	//if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->getPlayer() == BWAPI::Broodwar->self()){
	//	zealots.push_back(unit);
	//}
	
	if (unit->canAttack() && unit->canMove() && !unit->getType().isWorker()) {
		fighters.insert(unit);
	}


}

void OffenseManager::onFrame(){
	//Loop through zealots
	//std::vector<BWAPI::Unit>::iterator it;
	//for (it = zealots.begin(); it != zealots.end(); it++){
	//	BWAPI::Unit unit = *it;
	//	if (unit->isIdle()){
	//		unit->attack(unit->getClosestUnit((BWAPI::Filter::IsEnemy && BWAPI::Filter::CanAttack), 50));
	//	}
	//	if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->isUnderAttack()) {
	//		unit->attack(unit->getClosestUnit((BWAPI::Filter::IsEnemy && BWAPI::Filter::CanAttack), 4));
	//	}
	//}

	BWAPI::Unitset::iterator it;
	for (it = fighters.begin(); it != fighters.end(); it++) {
		BWAPI::Unit unit = *it;
		if (unit->isUnderAttack()) {
			fightBack(unit);
		}
		if (InformationManager::getInstance().enemyBase == BWTA::getNearestBaseLocation(unit->getPosition())) {
			searchAndDestroy(fighters);
		}
	}

	if (fighters.size() >= armySize) {
		rush(fighters);
	}

	
	/*if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= zealotMax){
		for (BWAPI::Unit zealot : zealots){
			zealot->attack(enemyBase->getPosition());
		}
		zealotMax = zealotMax * 2;
	}*/
}

OffenseManager& OffenseManager::getInstance(){ //Return ref to OffenseManager object
	static OffenseManager i; //Make static instance i
	return i;
}
bool OffenseManager::rush(BWAPI::Unitset attackers) {
	//Executes a rush with the gives units
	if (!attackers.empty()) {
		if (InformationManager::getInstance().enemyBase != NULL) {
			attackers.attack(InformationManager::getInstance().enemyBase->getPosition());
		}
		else {
			Broodwar << "Failed to rush enemyBase was null" << std::endl;
		}
	}
	else {
		Broodwar << "Failed to rush attackers was empty" << std::endl;
	}
	return true;

}

bool OffenseManager::fightBack(BWAPI::Unit attackedUnit) {
	BWAPI::Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding);
	if (attacker != NULL) {
		attackedUnit->attack(attacker);
		getHelp(attackedUnit, attacker);
		return true;
	}
	else {
		return false;
		Broodwar << "Failed to fightback, attacker was null" << std::endl;
	}
	
}
bool OffenseManager::getHelp(BWAPI::Unit victim, BWAPI::Unit badGuy) {
	BWAPI::Unit helper = victim->getClosestUnit(Filter::IsAlly && Filter::CanAttack && !Filter::IsWorker && !Filter::IsAttacking && Filter::CanMove);
	if (helper != NULL) {
		helper->attack(badGuy);
		return true;
	}
	else {
		return false;
		Broodwar << "Failed to get help, helper was null" << std::endl;
	}
}

bool OffenseManager::searchAndDestroy(BWAPI::Unitset attackers) {
	//Called when our fighters are idle in the enemy base
	BWAPI::Unitset temp;

	while (attackers.size() > 3) {
		BWAPI::Unit immigrant = *attackers.begin();
		temp.insert(immigrant);
		attackers.erase(immigrant);
	}
	if (!temp.empty()) {
		searchAndDestroy(temp);
	}

	//Finds units to kill and kills them in groups of around 3.
	if (!InformationManager::getInstance().enemyWorkers.empty()) {
		attackers.attack(InformationManager::getInstance().enemyWorkers.front());
		InformationManager::getInstance().enemyAttackers.push_back(InformationManager::getInstance().enemyWorkers.front());
		InformationManager::getInstance().enemyAttackers.erase(InformationManager::getInstance().enemyWorkers.begin());
		Broodwar << "Search and destroy targetting workers" << std::endl;
		return true;
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		attackers.attack(InformationManager::getInstance().enemyBarracks.front());
		InformationManager::getInstance().enemyBarracks.push_back(InformationManager::getInstance().enemyBarracks.front());
		InformationManager::getInstance().enemyBarracks.erase(InformationManager::getInstance().enemyBarracks.begin());
		Broodwar << "Search and destroy targetting barracks" << std::endl;
		return true;
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		attackers.attack(InformationManager::getInstance().enemyPassiveBuildings.front());
		InformationManager::getInstance().enemyPassiveBuildings.push_back(InformationManager::getInstance().enemyPassiveBuildings.front());
		InformationManager::getInstance().enemyPassiveBuildings.erase(InformationManager::getInstance().enemyPassiveBuildings.begin());
		Broodwar << "Search and destroy targetting passive buildings" << std::endl;
		return true;
	}
	else if (!InformationManager::getInstance().enemyAttackers.empty()) {
		attackers.attack(InformationManager::getInstance().enemyAttackers.front());
		InformationManager::getInstance().enemyAttackers.push_back(InformationManager::getInstance().enemyAttackers.front());
		InformationManager::getInstance().enemyAttackers.erase(InformationManager::getInstance().enemyAttackers.begin());
		Broodwar << "Search and destroy targetting attackers" << std::endl;
		return true;
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		attackers.attack(InformationManager::getInstance().enemyTowers.front());
		InformationManager::getInstance().enemyTowers.push_back(InformationManager::getInstance().enemyTowers.front());
		InformationManager::getInstance().enemyTowers.erase(InformationManager::getInstance().enemyTowers.begin());
		Broodwar << "Search and destroy targetting towers" << std::endl;
		return true;
	}
	else {
		Broodwar << "Nothing to search and destroy, did we win or is information wrong?" << std::endl;
		return false;
	}
	

}
