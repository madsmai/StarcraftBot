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
	if (unit->getPlayer() == Broodwar->self()) {
		if (fighters.find(unit) != fighters.end()) {
			fighters.erase(fighters.find(unit));
		}
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
	static int lastChecked = 0;
	for (it = fighters.begin(); it != fighters.end(); it++) {
		BWAPI::Unit unit = *it;
		if (unit->isUnderAttack()) {
			fightBack(unit);
		}
		else if (InformationManager::getInstance().enemyBase == BWTA::getNearestBaseLocation(unit->getPosition())
			&& unit->isIdle()
			&& lastChecked + 75 < BWAPI::Broodwar->getFrameCount()) {
			searchAndDestroy(fighters);
			lastChecked = Broodwar->getFrameCount();
		}
	}

	if (fighters.size() >= armySize) {
		rush(fighters);
		//armySize += armySize;
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
	static int lastChecked = 0;
	if (!attackers.empty()) {
		if (lastChecked + 400 < BWAPI::Broodwar->getFrameCount()
			&& !rushFinished) {
			if (InformationManager::getInstance().enemyBase != NULL) {
				//Trying to use move instead of attack, hoping SearchAndDestroy will take over
				attackers.move(InformationManager::getInstance().enemyBase->getPosition());
				lastChecked = BWAPI::Broodwar->getFrameCount();
			}
			else {
				Broodwar << "Failed to rush enemyBase was null" << std::endl;
			}
		}
	}
	else {
		Broodwar << "Failed to rush attackers was empty" << std::endl;
	}
	return true;

}

bool OffenseManager::fightBack(BWAPI::Unit attackedUnit) {
	if (attackedUnit != NULL) {
		BWAPI::Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding);
		if (attacker != NULL) {
			calculatePriority(attacker, attackedUnit);
			if (attackedUnit->isAttacking()) {
				attackedUnit->attack(attacker);
			}
			getHelp(attackedUnit, attacker);
			return true;
		}
		else {
			Broodwar << "Failed to fightback, attacker was null" << std::endl;
			return false;
		}
	}
	return false;
}
bool OffenseManager::getHelp(BWAPI::Unit victim, BWAPI::Unit badGuy) {

	if (victim != NULL) {
		BWAPI::Unit helper = victim->getClosestUnit(Filter::IsAlly
			&& Filter::CanAttack
			&& !Filter::IsWorker
			&& !Filter::IsAttacking
			&& Filter::CanMove);

		if (helper != NULL
			&& badGuy->isVisible()
			&& badGuy != NULL) {

			helper->attack(badGuy);
			return true;
		}
		else {
			return false;
			Broodwar << "Failed to get help, helper was null" << std::endl;
		}
	}
	return false;
}

void OffenseManager::searchAndDestroy(BWAPI::Unitset attackers) {
	//Called when our fighters are idle in the enemy base

	Unitset temp;

	while (attackers.size() > armySize) {
		BWAPI::Unit immigrant = *attackers.begin();
		if (immigrant != NULL) {
			temp.insert(immigrant);
			attackers.erase(immigrant);
		}
		else {
			Broodwar << "DEPLOY THE BRAKES!!!" << std::endl;
			break;
		}
	}
	if (!temp.empty()) {
		searchAndDestroy(temp);
	}

	//Finds units to kill and kills them in groups of around 3.
	//Possible bug, it might not reach anything below workers

	std::vector<BWAPI::Unit>::iterator it;
	if (!InformationManager::getInstance().enemyWorkers.empty()) {
		for (it = InformationManager::getInstance().enemyWorkers.begin(); it != InformationManager::getInstance().enemyWorkers.end();) {
			Unit unit = *it;
			if (!avoidTowers(unit)) {
				attackers.attack(unit);
				InformationManager::getInstance().removeEnemyWorkers(unit);
				InformationManager::getInstance().enemyWorkers.push_back(unit);
				Broodwar << "Search and destroy targetting enemyWorkers" << std::endl;
				break;
			}
			else {
				it++;
				Broodwar << "Unit was under tower" << std::endl;
			}
		}
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		Broodwar << "enemyBarracks was not empty" << std::endl;
		for (it = InformationManager::getInstance().enemyBarracks.begin(); it != InformationManager::getInstance().enemyBarracks.end();) {
			Unit unit = *it;
			if (!avoidTowers(unit)) {
				attackers.attack(unit);
				InformationManager::getInstance().removeEnemyBarracks(unit);
				InformationManager::getInstance().enemyBarracks.push_back(unit);
				Broodwar << "Search and destroy targetting enemyBarracks" << std::endl;
				break;
			}
			else {
				it++;
				Broodwar << "Unit was under tower" << std::endl;
			}
		}
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		Broodwar << "enemyPassiveBuildings was not empty" << std::endl;
		for (it = InformationManager::getInstance().enemyPassiveBuildings.begin(); it != InformationManager::getInstance().enemyPassiveBuildings.end();) {
			Unit unit = *it;
			if (!avoidTowers(unit)) {
				attackers.attack(unit);
				InformationManager::getInstance().removeEnemyPassiveBuildings(unit);
				InformationManager::getInstance().enemyPassiveBuildings.push_back(unit);
				Broodwar << "Search and destroy targetting enemyPassiveBuildings" << std::endl;
				break;
			}
			else {
				it++;
				Broodwar << "Unit was under tower" << std::endl;
			}
		}
	}
	else if (!InformationManager::getInstance().enemyAttackers.empty()) {
		Broodwar << "enemyAttackers was not empty" << std::endl;
		for (it = InformationManager::getInstance().enemyAttackers.begin(); it != InformationManager::getInstance().enemyAttackers.end();) {
			Unit unit = *it;
			if (!avoidTowers(unit)) {
				if (!unit->isVisible()) {
					attackers.move(unit->getPosition());
					attackers.attack(unit,true);
					InformationManager::getInstance().removeEnemyAttackers(unit);
					InformationManager::getInstance().enemyAttackers.push_back(unit);
					Broodwar << "Search and destroy targetting enemyAttackers" << std::endl;
					break;
				}
				else {
					attackers.attack(unit);
					InformationManager::getInstance().removeEnemyAttackers(unit);
					InformationManager::getInstance().enemyAttackers.push_back(unit);
					Broodwar << "Search and destroy targetting enemyAttackers" << std::endl;
					break;
				}
				
			}
			else {
				it++;
				Broodwar << "Unit was under tower" << std::endl;
			}
		}
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		Broodwar << "enemyTowers was not empty" << std::endl;
		attackers.attack(InformationManager::getInstance().enemyTowers.front());
		InformationManager::getInstance().enemyTowers.push_back(InformationManager::getInstance().enemyTowers.front());
		InformationManager::getInstance().removeEnemyTowers(*InformationManager::getInstance().enemyTowers.begin());
		Broodwar << "Search and destroy targetting towers" << std::endl;
	}
	else {
		Broodwar << "Nothing to search and destroy, did we win or is information wrong?" << std::endl;
	}


}


bool OffenseManager::avoidTowers(BWAPI::Unit fighter) {
	Broodwar << "Starting avoidTowers" << std::endl;
	bool underTower = false;

	std::vector<BWAPI::Unit>::iterator it;
	if (!InformationManager::getInstance().enemyTowers.empty() && fighter != NULL){
		for (it = InformationManager::getInstance().enemyTowers.begin(); it != InformationManager::getInstance().enemyTowers.end(); it++) {
			BWAPI::Unit tower = *it;
			if (fighter->getPlayer() == Broodwar->self()) {
				if (fighter->isFlying()) {
					BWAPI::Unitset jeopardizedUnits = tower->getUnitsInWeaponRange(tower->getType().airWeapon(), Filter::IsAlly);
					if (jeopardizedUnits.find(fighter) != jeopardizedUnits.end()) {
						//Its jeopardized
						underTower = true;
					}
					else {
						underTower = false;
					}
				}
				else {
					BWAPI::Unitset jeopardizedUnits = tower->getUnitsInWeaponRange(tower->getType().groundWeapon(), Filter::IsAlly);
					if (jeopardizedUnits.find(fighter) != jeopardizedUnits.end()) {
						//Its jeopardized
						underTower = true;
					}
					else {
						underTower = false;
					}
				}
			}
			else {
				if (fighter->isFlying()) {
					BWAPI::Unitset defendedUnits = tower->getUnitsInWeaponRange(tower->getType().airWeapon(), Filter::IsEnemy);
					if (defendedUnits.find(fighter) != defendedUnits.end()) {
						//Its defended
						underTower = true;
					}
					else {
						underTower = false;
					}
				}
				else {
					BWAPI::Unitset defendedUnits = tower->getUnitsInWeaponRange(tower->getType().groundWeapon(), Filter::IsEnemy);
					if (defendedUnits.find(fighter) != defendedUnits.end()) {
						//Its defended
						underTower = true;
					}
					else {
						underTower = false;
					}
				}
			}
		}
	}

	return underTower;
}
int OffenseManager::calculatePriority(Unit enemy, Unit ourUnit) {

	Broodwar << "Calculating priority" << std::endl;

	int effectiveHp = enemy->getHitPoints() + enemy->getShields();
	int ourDamage = (Broodwar->self()->damage(ourUnit->getType().groundWeapon()) - enemy->getPlayer()->armor(enemy->getType())); //* ourUnit->getType().maxGroundHits();

	//Integer division round up, hopefully
	int hitsToKill = effectiveHp + (ourDamage - 1) / ourDamage;

	int damage = (enemy->getPlayer()->damage(enemy->getType().groundWeapon()) - Broodwar->self()->armor(ourUnit->getType())); //* enemy->getType().maxGroundHits();

	int priority = damage / hitsToKill;

	Broodwar << "Priority of " << enemy->getType() << "is: " << priority << std::endl;
	return priority;
}
