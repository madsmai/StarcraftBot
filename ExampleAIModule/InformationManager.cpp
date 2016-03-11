#include "InformationManager.h"
#include <vector>
#include <BWTA.h>

// Author: Sami Ghali s144443

/*
TODO:
- Gemme information om hvor modstanderen er og hvor godt deres forsvar er, 
	dette skal v�re tilg�ngeligt for de andre klasser
*/

using namespace BWAPI;

//Adding enemy units to vectors
void InformationManager::addEnemyBarracks(BWAPI::Unit barracks){

	enemyBarracks.push_back(barracks);

}

void InformationManager::addEnemyAttackers(BWAPI::Unit attacker){

	enemyAttackers.push_back(attacker);

}

void InformationManager::addEnemyWorkers(BWAPI::Unit worker){

	enemyWorkers.push_back(worker);

}

void InformationManager::addEnemyTowers(BWAPI::Unit tower){

	enemyTowers.push_back(tower);

}

void InformationManager::addEnemyPassiveBuildings(BWAPI::Unit passiveBuilding){

	enemyPassiveBuildings.push_back(passiveBuilding);

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

	Broodwar->sendText("Current Status: \n"
						<< enemyAttackers.size << "enemy attacker(s) \n"
						<< enemyBarracks.size << "enemy barrack(s) \n"
						<< enemyWorkers.size << "enemy worker(s) \n"
						<< enemyTowers.size << "enemy tower(s) \n"
						<< enemyPassiveBuildings.size << "passive enemy building(s) \n");
}

InformationManager& InformationManager::getInstance(){ //Return ref to InformationManager object
	static InformationManager i; //Make static instance i
	return i;
}