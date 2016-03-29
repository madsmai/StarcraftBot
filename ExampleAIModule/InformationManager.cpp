#include "InformationManager.h"
#include <vector>

// Author: Sami Ghali s144443

/*
TODO:
- Gemme information om hvor modstanderen er og hvor godt deres forsvar er, 
	dette skal være tilgængeligt for de andre klasser
*/

using namespace BWAPI;

//Discover and destroy methods  

void InformationManager::onUnitDiscover(BWAPI::Unit unit){
	if (unit->getPlayer()->isEnemy(Broodwar->self())){
		
		if (unit->getType().isWorker()){
			addEnemyWorkers(unit);
		}

		if (unit->getType().canAttack() && unit->getType().canMove() && unit->getType().isWorker()){
			addEnemyAttackers(unit);
		}

		if (unit->getType() == UnitTypes::Protoss_Gateway || 
			unit->getType() == UnitTypes::Zerg_Spawning_Pool || 
			unit->getType() == UnitTypes::Terran_Barracks){

			addEnemyBarracks(unit);
		}

		if (unit->getType().isBuilding() && unit->getType().canAttack()){
			addEnemyTowers(unit);
		}

		if (unit->getType().isBuilding() && !unit->getType().canAttack()){
			addEnemyPassiveBuildings(unit);
		}


	}

}

void InformationManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getPlayer()->isEnemy(Broodwar->self())){

		if (unit->getType().isWorker()){
			removeEnemyWorkers(unit);
		}

		if (unit->getType().canAttack() && unit->getType().canMove() && unit->getType().isWorker()){
			removeEnemyAttackers(unit);
		}

		if (unit->getType() == UnitTypes::Protoss_Gateway ||
			unit->getType() == UnitTypes::Zerg_Spawning_Pool ||
			unit->getType() == UnitTypes::Terran_Barracks){

			removeEnemyBarracks(unit);
		}

		if (unit->getType().isBuilding() && unit->getType().canAttack()){
			removeEnemyTowers(unit);
		}

		if (unit->getType().isBuilding() && !unit->getType().canAttack()){
			removeEnemyPassiveBuildings(unit);
		}


	}
}


//Adding enemy units to vectors
void InformationManager::addEnemyBarracks(BWAPI::Unit barracks){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyBarracks.begin(); it != enemyBarracks.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == barracks->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		enemyBarracks.push_back(barracks);
	}
		
}

void InformationManager::addEnemyAttackers(BWAPI::Unit attacker){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyAttackers.begin(); it != enemyAttackers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == attacker->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		enemyAttackers.push_back(attacker);
	}

}

void InformationManager::addEnemyWorkers(BWAPI::Unit worker){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyWorkers.begin(); it != enemyWorkers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == worker->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		enemyWorkers.push_back(worker);
	}

}

void InformationManager::addEnemyTowers(BWAPI::Unit tower){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyTowers.begin(); it != enemyTowers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == tower->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		enemyTowers.push_back(tower);
	}

}

void InformationManager::addEnemyPassiveBuildings(BWAPI::Unit passiveBuilding){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyPassiveBuildings.begin(); it != enemyPassiveBuildings.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == passiveBuilding->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		enemyPassiveBuildings.push_back(passiveBuilding);
	}

}


//Removing enemy units to vectors
void InformationManager::removeEnemyBarracks(BWAPI::Unit barracks){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyBarracks.begin(); it != enemyBarracks.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == barracks->getID()){
			enemyBarracks.erase(it);
		}
	}

}

void InformationManager::removeEnemyAttackers(BWAPI::Unit attacker){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyAttackers.begin(); it != enemyAttackers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == attacker->getID()){
			enemyAttackers.erase(it);
		}
	}
}

void InformationManager::removeEnemyWorkers(BWAPI::Unit worker){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyWorkers.begin(); it != enemyWorkers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == worker->getID()){
			enemyWorkers.erase(it);
		}
	}

}

void InformationManager::removeEnemyTowers(BWAPI::Unit tower){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyTowers.begin(); it != enemyTowers.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == tower->getID()){
			enemyTowers.erase(it);
		}
	}

}

void InformationManager::removeEnemyPassiveBuildings(BWAPI::Unit passiveBuilding){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyPassiveBuildings.begin(); it != enemyPassiveBuildings.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == passiveBuilding->getID()){
			enemyPassiveBuildings.erase(it);
		}
	}

}

//Prints the current count of enemy units
void InformationManager::currentStatus(){

	Broodwar << ("Current Status: \n"
						<< enemyAttackers.size() << "enemy attacker(s) \n"
						<< enemyBarracks.size() << "enemy barrack(s) \n"
						<< enemyWorkers.size() << "enemy worker(s) \n"
						<< enemyTowers.size() << "enemy tower(s) \n"
						<< enemyPassiveBuildings.size() << "passive enemy building(s) \n");
}

InformationManager& InformationManager::getInstance(){ //Return ref to InformationManager object
	static InformationManager i; //Make static instance i
	return i;
}
