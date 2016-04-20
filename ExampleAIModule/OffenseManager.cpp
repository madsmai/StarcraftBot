#include "OffenseManager.h"
#include "InformationManager.h"

using namespace BWAPI;
/*
TODO:
- S�rg for at samle zealots sammen og angrib med dem sammen, det samlede antal skal varierer
alt efter modstanderens forsvar
- En m�de at f� zealots til at prioriterer hvad de angriber
-B�r search and destroy faktisk bruge et unitset, m�ske skal de blot bruge 1 unit

*/


void OffenseManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getPlayer() == Broodwar->self()) {
		if (fighters.find(unit) != fighters.end()) {
			fighters.erase(fighters.find(unit));
		}
	}
}

void OffenseManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->canAttack() && unit->canMove() && !unit->getType().isWorker()) {
		fighters.insert(unit);
		if (rushOngoing) {
			unit->move(InformationManager::getInstance().enemyBase->getPosition());
		}
	}

}

void OffenseManager::onFrame(){
	for (Unit unit : fighters) {
		if (unit->isUnderAttack()) {
			if (InformationManager::getInstance().calculateArmyStrength(Broodwar->self()) < InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())) {
				Broodwar << "Their army is stronger" << std::endl;
				rushOngoing = false;
				fighters.move(InformationManager::getInstance().ourBase->getPosition());
			}
			else {
				fightBack(unit);
			}
		}
		else if (InformationManager::getInstance().ourBase != BWTA::getNearestBaseLocation(unit->getPosition())
			&& unit->isIdle()) {	
			searchAndDestroy(unit);
		}

		if (!rushOngoing && unit->isIdle()) {
			squad.insert(unit);
		}
	}
	if (squad.size() >= armySize) {
		rush(squad);
		squad.empty();
	}
}

OffenseManager& OffenseManager::getInstance(){ //Return ref to OffenseManager object
	static OffenseManager i; //Make static instance i
	return i;
}
bool OffenseManager::rush(BWAPI::Unitset attackers) {
	//Executes a rush with the given units
	static int lastChecked = 0;
	if (!attackers.empty()) {
		if (lastChecked + 400 < BWAPI::Broodwar->getFrameCount()) {
			if (InformationManager::getInstance().enemyBase != NULL) {
				//Trying to use move instead of attack, hoping SearchAndDestroy will take over
				attackers.move(InformationManager::getInstance().enemyBase->getPosition());
				rushOngoing = true;
				lastChecked = BWAPI::Broodwar->getFrameCount();
			}
			else {
				Broodwar << "Failed to rush, enemyBase was null " << std::endl;
			}
		}
	}
	else {
		Broodwar << "Failed to rush attackers was empty " << std::endl;
	}
	return true;

}

bool OffenseManager::fightBack(BWAPI::Unit attackedUnit) {
	if (attackedUnit != NULL) {
		BWAPI::Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding);
		BWAPI::Unitset nearbyEnemies = attackedUnit->getUnitsInRadius(128, Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding && Filter::IsVisible);
		if (attacker != NULL) {
			calculatePriority(attacker, attackedUnit);
			//Broodwar << "Nearby Enemies size = " << nearbyEnemies.size() << std::endl;
			//Broodwar << "Range of " << attacker->getType() << " is " << attacker->getType().groundWeapon().maxRange() << std::endl;
			if (nearbyEnemies.size() >= 2) {
				int maxPrio = 0;
				Unit bestTarget;
				for (Unit troop : nearbyEnemies) {
					int currentPrio = calculatePriority(troop, attackedUnit);
					if (currentPrio > maxPrio) {
						maxPrio = currentPrio;
						bestTarget = troop;
					}
				}
				Broodwar->drawCircleMap(bestTarget->getPosition(),15,Colors::Brown);
				attackedUnit->attack(bestTarget);
				getHelp(attackedUnit, bestTarget);
				return true;
			}
			else {
				if (attackedUnit->isAttacking()) {
					attackedUnit->attack(attacker);
				}
				getHelp(attackedUnit, attacker);
				return true;
			}
			
		}
		else {
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

void OffenseManager::searchAndDestroy(BWAPI::Unit attacker) {
	//Called when our fighters are idle in the enemy base

	//Unitset temp;

	/*while (attackers.size() > armySize) {
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
	}*/

	//Finds units to kill and kills them in groups of around 3.
	std::vector<BWAPI::Unit>::iterator it;
	if (!InformationManager::getInstance().enemyWorkers.empty()) {
		if (!attacker->attack(attacker->getClosestUnit(Filter::IsWorker && Filter::IsEnemy))) {
			attacker->attack(InformationManager::getInstance().enemyWorkers.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		if (attacker->attack(attacker->getClosestUnit((Filter::GetType == UnitTypes::Protoss_Gateway
			|| Filter::GetType == UnitTypes::Terran_Barracks
			|| Filter::GetType == UnitTypes::Zerg_Spawning_Pool) && Filter::IsEnemy))) {
			attacker->attack(InformationManager::getInstance().enemyBarracks.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		if (!attacker->attack(attacker->getClosestUnit(Filter::IsBuilding && !Filter::CanAttack && Filter::IsEnemy))) {
			attacker->attack(InformationManager::getInstance().enemyPassiveBuildings.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyAttackers.empty()) {
		if (!attacker->attack(attacker->getClosestUnit(Filter::CanAttack && Filter::CanMove && Filter::IsEnemy))) {
			attacker->attack(InformationManager::getInstance().enemyAttackers.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		if (!attacker->attack(attacker->getClosestUnit(Filter::IsBuilding && Filter::CanAttack && Filter::IsEnemy))) {
			attacker->attack(InformationManager::getInstance().enemyTowers.front()->getPosition());
		}
	}
	else {
		Broodwar << "Nothing to search and destroy, did we win or is information wrong?" << std::endl;
	}


}


bool OffenseManager::avoidTowers(BWAPI::Unit fighter) {
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

	int effectiveHp = enemy->getHitPoints() + enemy->getShields();

	int ourDamage = (Broodwar->self()->damage(ourUnit->getType().groundWeapon()) - enemy->getPlayer()->armor(enemy->getType())) * ourUnit->getType().maxGroundHits();

	//Integer division round up, hopefully
	int hitsToKill = (effectiveHp + (ourDamage - 1)) / ourDamage;

	int damage = (enemy->getPlayer()->damage(enemy->getType().groundWeapon()) - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();

	int priority = damage / hitsToKill;


	//Broodwar << "Effectivehp of " << enemy->getType() << " is: " << effectiveHp << std::endl;
	//Broodwar << "ourDamage of " << ourUnit->getType() << " is: " << ourDamage << std::endl;
	//Broodwar << "hitsToKill of " << enemy->getType() << " is: " << hitsToKill << std::endl;
	//Broodwar << "damage of " << enemy->getType() << " is: " << damage << std::endl;
	//Broodwar << "Priority of " << enemy->getType() << " is: " << priority << std::endl;

	return priority;
}
