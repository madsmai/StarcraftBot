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
	if (unit->getPlayer()->isEnemy(Broodwar->self()) && !unit->getPlayer()->isNeutral()){
		
		if (unit->getType().isWorker()){
			addEnemyWorkers(unit);
		}

		else if (unit->getType().canAttack() && unit->getType().canMove()){
			addEnemyAttackers(unit);
		}
		else if (unit->getType() == UnitTypes::Protoss_Dark_Templar ){
			addDarkTemplar(unit);

		}

		else if (unit->getType() == UnitTypes::Protoss_Gateway || 
			unit->getType() == UnitTypes::Zerg_Spawning_Pool || 
			unit->getType() == UnitTypes::Terran_Barracks){

			addEnemyBarracks(unit);
		}

		else if (unit->getType().isBuilding() && unit->getType().canAttack()){
			addEnemyTowers(unit);
		}

		else if (unit->getType().isBuilding() && !unit->getType().canAttack()){
			addEnemyPassiveBuildings(unit);
		} 
		
		else if (unit->getType() == UnitTypes::Protoss_Citadel_of_Adun){
			citadelOfAdun = true;
		}

		else if (unit->getType() == UnitTypes::Protoss_Templar_Archives){
			templarArchives = true;
		}


	}

}

void InformationManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getPlayer()->isEnemy(Broodwar->self()) && !unit->getPlayer()->isNeutral()){

		if (unit->getType().isWorker()){
			removeEnemyWorkers(unit);
		}

		else if (unit->getType().canAttack() && unit->getType().canMove()){
			removeEnemyAttackers(unit);
		}

		else if (unit->getType() == UnitTypes::Protoss_Dark_Templar){
			removeDarkTemplar(unit);
		}

		else if (unit->getType() == UnitTypes::Protoss_Gateway ||
			unit->getType() == UnitTypes::Zerg_Spawning_Pool ||
			unit->getType() == UnitTypes::Terran_Barracks){

			removeEnemyBarracks(unit);
		}

		else if (unit->getType().isBuilding()){
			if (unit->getType().canAttack()){
				removeEnemyTowers(unit);
			}
			else if (!unit->getType().canAttack()){
				removeEnemyPassiveBuildings(unit);
			}
			if (unit->getType().isResourceDepot()
				&& unit->getTilePosition() == enemyBase->getTilePosition()){
			}
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


void InformationManager::addDarkTemplar(BWAPI::Unit darkTemplar){

	bool exists = false;
	std::vector<BWAPI::Unit>::iterator it;
	for (it = darkTemplars.begin(); it != darkTemplars.end(); it++) {
		BWAPI::Unit u = *it;
		if (u->getID() == darkTemplar->getID()){
			exists = true;
			break;
		}
	}
	if (!exists) {
		darkTemplars.push_back(darkTemplar);
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
	for (it = enemyBarracks.begin(); it != enemyBarracks.end(); ) {
		BWAPI::Unit u = *it;
		if (u->getID() == barracks->getID()){
			enemyBarracks.erase(it);
		}
		else {
			it++;
		}
	}

}

void InformationManager::removeEnemyAttackers(Unit attacker){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyAttackers.begin(); it != enemyAttackers.end(); ) {
		Unit u = *it;
		if (u->getID() == attacker->getID()){
			Broodwar << "unit removed from enemyAttackers list" << std::endl;
			enemyAttackers.erase(it);
		} else{
it++;
		}
	}
}

void InformationManager::removeEnemyWorkers(BWAPI::Unit worker){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyWorkers.begin(); it != enemyWorkers.end();) {
		BWAPI::Unit u = *it;
		if (u->getID() == worker->getID()){
			enemyWorkers.erase(it);
		}
		else {
			it++;
		}
	}

}


void InformationManager::removeDarkTemplar(BWAPI::Unit darkTemplar){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = darkTemplars.begin(); it != darkTemplars.end();) {
		BWAPI::Unit u = *it;
		if (u->getID() == darkTemplar->getID()){
			darkTemplars.erase(it);
		}
		else {
			it++;
		}
	}

}

void InformationManager::removeEnemyTowers(BWAPI::Unit tower){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyTowers.begin(); it != enemyTowers.end();) {
		BWAPI::Unit u = *it;
		if (u->getID() == tower->getID()){
			enemyTowers.erase(it);
		}
		else {
			it++;
		}
	}

}

void InformationManager::removeEnemyPassiveBuildings(BWAPI::Unit passiveBuilding){

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyPassiveBuildings.begin(); it != enemyPassiveBuildings.end();) {
		BWAPI::Unit u = *it;
		if (u->getID() == passiveBuilding->getID()){
			enemyPassiveBuildings.erase(it);
		}
		else {
			it++;
		}
	}
}

//Prints the current count of enemy units
void InformationManager::currentStatus(){
	Broodwar << "Current Status: \n" << enemyAttackers.size() << "  enemy attacker(s) \n"
		<< enemyBarracks.size() << "  enemy barrack(s) \n"
		<< enemyWorkers.size() << "  enemy worker(s) \n"
		<< enemyTowers.size() << "  enemy tower(s) \n"
		" Any Citadel of Adun? " << citadelOfAdun << "\n"
		" Any Templar Archives? " << templarArchives << "\n"
		<< enemyTowers.size() << "  enemy dark templars(s) \n"
		<< enemyPassiveBuildings.size() << "  passive enemy building(s) \n" << std::endl;

}

InformationManager& InformationManager::getInstance(){ //Return ref to InformationManager object
	static InformationManager i; //Make static instance i
	return i;
}

int InformationManager::calculateArmyStrength(BWAPI::Player player) {
	Unitset fighters;
	if (player == Broodwar->self()) {
		fighters = OffenseManager::getInstance().fighters;
	}
	else if (player->isEnemy(Broodwar->self())) {
		std::vector<BWAPI::Unit>::iterator it;
		for (it = enemyAttackers.begin(); it != enemyAttackers.end(); it++) {
			Unit u = *it;
			fighters.insert(u);
		}
	}
	int armyStrength = 0;
	for (Unit troop : fighters) {
		int effectiveHp = troop->getHitPoints() + troop->getShields();
		int damage = (troop->getPlayer()->damage(troop->getType().groundWeapon())) * troop->getType().maxGroundHits();
		if (troop->getType().groundWeapon().maxRange() > 100) {
			damage = (damage * 6)/5;
		}
		int strength = effectiveHp * damage;
		armyStrength = armyStrength + strength;
	}
	return armyStrength;
}

void InformationManager::enemyArmyStatus(){
	Unitset fighters;

	std::vector<BWAPI::Unit>::iterator it;
	for (it = enemyAttackers.begin(); it != enemyAttackers.end(); it++) {
		Unit u = *it;
		fighters.insert(u);
	
	}
	int armyStrength = 0;
	int damage = 0;
	int strength = 0;
	int effectiveHp = 0;
	for (Unit troop : fighters) {
		int effectiveHp = troop->getHitPoints() + troop->getShields();
		int damage = (troop->getPlayer()->damage(troop->getType().groundWeapon())) * troop->getType().maxGroundHits();
		if (troop->getType().groundWeapon().maxRange() > 100) {
			damage = (damage * 6) / 5;
		}
		int strength = effectiveHp * damage;
		armyStrength = armyStrength + strength;
	}

	Broodwar << "Enemy Army Status: \n" << armyStrength << "  = Enemy Army Strenght \n"
		<< damage << "  Enemy damage \n"
		<< strength << "  Enemy strength \n"
		<< effectiveHp << "  Enemy effective hp" << std::endl;

}

void InformationManager::ourArmyStatus(){
	Unitset fighters;
	
	fighters = OffenseManager::getInstance().fighters;
	
	int armyStrength = 0;
	int damage = 0;
	int strength = 0;
	int effectiveHp = 0;
	for (Unit troop : fighters) {
		int effectiveHp = troop->getHitPoints() + troop->getShields();
		int damage = (troop->getPlayer()->damage(troop->getType().groundWeapon())) * troop->getType().maxGroundHits();
		if (troop->getType().groundWeapon().maxRange() > 100) {
			damage = (damage * 6) / 5;
		}
		int strength = effectiveHp * damage;
		armyStrength = armyStrength + strength;
	}

	Broodwar << "Our Army Status: \n" << armyStrength << "  = Our Army Strenght \n"
		<< damage << "  Our damage \n"
		<< strength << "  Our strength \n"
		<< effectiveHp << "  Our effective hp" << std::endl;
}

int InformationManager::writeToLog(std::string text) {
	if (logging) {
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		std::ofstream log;
		log.open("bwapi-data//AI/log.txt", std::ios::app);
		log << std::put_time(&tm, "%d-%m-%y %H-%M-%S") << ":: " << text << "\n";
		log.flush();
		log.close();
	}
		return 42;
}
