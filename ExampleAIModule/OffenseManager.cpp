#include "OffenseManager.h"



using namespace BWAPI;
/*
TODO:
- Expansions bugger os, helt vildt.
- De bevæger sig frem og tilbage
- Dræber ikke workers i expansion først
- Tror det er fordi de bliver sat til at bevæge sig mod hovedbasen, når den, bliver idle, laver search and destroy.

- Abuse andre bots dårlig micro
- Vi gør ikke noget mod et rush der er hurtigere end vores
-Ikke nogle metoder til at få vores zealots til at gøre noget i basen

- Early dark templars strategien bugger

- Shift queue squad til at move et sted og så move til basen så de grupper


*/

void OffenseManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getPlayer() == Broodwar->self()) {
		InformationManager::getInstance().writeToLog("One of our dudes died");
		if (fighters.find(unit) != fighters.end()) {
			fighters.erase(fighters.find(unit));
		}
		else if (cowards.find(unit) != cowards.end()) {
			cowards.erase(cowards.find(unit));
		}
	}
}

void OffenseManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->canAttack() && unit->canMove() && !unit->getType().isWorker() && unit != NULL) {
		fighters.insert(unit);
		if (rushOngoing && InformationManager::getInstance().enemyBase != NULL) {
			unit->move(InformationManager::getInstance().enemyBase->getPosition());
		}
	}

}

void OffenseManager::onFrame(){
	for (Unit unit : fighters) {
		if (unit != NULL && unit->isUnderAttack()) {
			InformationManager::getInstance().writeToLog("Under attack");
			if (InformationManager::getInstance().calculateArmyStrength(Broodwar->self()) < InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
				&& InformationManager::getInstance().ourBase->getRegion() != BWTA::getRegion(unit->getTilePosition())) {

				Broodwar << "Their army is stronger" << std::endl;
				rushOngoing = false;
				fighters.move(InformationManager::getInstance().ourBase->getPosition());
			}
			else {
				fightBack(unit);
			}
		}
		else if (InformationManager::getInstance().ourBase != NULL
			&& InformationManager::getInstance().ourBase->getRegion() != NULL
			&& BWTA::getRegion(unit->getTilePosition()) != NULL
			&& InformationManager::getInstance().ourBase->getRegion() != BWTA::getRegion(unit->getTilePosition())
			&& unit->isIdle()) {
			Broodwar << "Before search and destroy" << std::endl;
				searchAndDestroy(unit);
		}
		else if (InformationManager::getInstance().enemyBase != NULL
			&& unit != NULL
			&& InformationManager::getInstance().enemyBase->getRegion() != NULL
			&& BWTA::getRegion(unit->getTilePosition()) != NULL
			&& InformationManager::getInstance().enemyBase->getRegion() != BWTA::getRegion(unit->getTilePosition())
			&& unit->isIdle()
			&& !rushOngoing){
				unit->move(InformationManager::getInstance().ourBase->getPosition());
				squad.insert(unit);
		}
		if (unit->isMoving()
			&& unit->getLastCommand().getType() != NULL
			&& unit->getLastCommand().getType() == UnitCommandTypes::Attack_Move || unit->getLastCommand().getType() == UnitCommandTypes::Attack_Unit) {
			Broodwar->drawLine(CoordinateType::Enum::Map, unit->getPosition().x, unit->getPosition().y,
				unit->getLastCommand().getTarget()->getPosition().x, unit->getLastCommand().getTarget()->getPosition().y, Colors::Red);
		}
	}
	if (squad.size() >= squadSize) {
		rush(squad);
		squad.clear();
	}
	if (cowards.size() > 0 && runFrames + 18 <= Broodwar->getFrameCount()) {
		InformationManager::getInstance().writeToLog("Started Coward purging");
		for (Unit coward : cowards) {
			searchAndDestroy(coward);
		}
		cowards.clear();
	}

}

OffenseManager& OffenseManager::getInstance(){ //Return ref to OffenseManager object
	static OffenseManager i; //Make static instance i
	return i;
}

bool OffenseManager::rush(BWAPI::Unitset attackers) {
	InformationManager::getInstance().writeToLog("Started rush");
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
	InformationManager::getInstance().writeToLog("Started fightBack");
	if (attackedUnit != NULL) {
		BWAPI::Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding);
		BWAPI::Unitset nearbyEnemies = attackedUnit->getUnitsInRadius(128, Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding && Filter::IsVisible);
		BWAPI::Unitset nearbyAllies = attackedUnit->getUnitsInRadius(128, !Filter::IsEnemy && !Filter::IsWorker && !Filter::IsBuilding);
		if (attacker != NULL) {
			if (attackedUnit->getShields() < attackedUnit->getType().maxShields() / 10
				&& nearbyAllies.size() > 0
				&& nearbyEnemies.size() > 1) {
				if (cowards.find(attackedUnit) == cowards.end()) {
					cowards.insert(attackedUnit);
				}
				runFrames = Broodwar->getFrameCount();
				if (InformationManager::getInstance().enemyBase->getRegion() != BWTA::getRegion(attackedUnit->getTilePosition())) {
					attackedUnit->move(InformationManager::getInstance().ourBase->getPosition());
				}
				else {
					attackedUnit->move(BWTA::getNearestChokepoint(attackedUnit->getPosition())->getCenter());
				}
				InformationManager::getInstance().writeToLog("Got past it");
			}
			else if (nearbyEnemies.size() >= 2) {
				int maxPrio = 0;
				Unit bestTarget;
				for (Unit troop : nearbyEnemies) {
					int currentPrio = calculatePriority(troop, attackedUnit);
					if (currentPrio > maxPrio) {
						maxPrio = currentPrio;
						bestTarget = troop;
					}
				}
				Broodwar->drawCircleMap(bestTarget->getPosition(), 15, Colors::Brown);
				attackedUnit->attack(bestTarget);
				getHelp(attackedUnit, bestTarget);
				return true;
			}
			else {
				if (!attackedUnit->isAttacking()) {
					attackedUnit->attack(attacker);
				}
				getHelp(attackedUnit, attacker);
				return true;
			}
		}
	}
	return false;
}

bool OffenseManager::getHelp(BWAPI::Unit victim, BWAPI::Unit badGuy) {
	InformationManager::getInstance().writeToLog("Started getHelp");
	if (victim != NULL) {
		BWAPI::Unit helper = victim->getClosestUnit(Filter::IsAlly
			&& Filter::CanAttack
			&& !Filter::IsWorker
			&& !Filter::IsAttacking
			&& Filter::CanMove);

		if (helper != NULL
			&& badGuy->isVisible()
			&& badGuy != NULL
			&& fighters.find(helper) != fighters.end()) {

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
	//Finds units to kill and kills them
	InformationManager::getInstance().writeToLog("Started searchAndDestroy");
	Unit closest;
	if (!InformationManager::getInstance().enemyWorkers.empty()) {
		closest = attacker->getClosestUnit(Filter::IsWorker && Filter::IsEnemy);
		if (closest != NULL && closest->isVisible()) {
			attacker->attack(closest);
		}
		else {
			attacker->attack(InformationManager::getInstance().enemyWorkers.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		closest = attacker->getClosestUnit((Filter::GetType == UnitTypes::Protoss_Gateway
			|| Filter::GetType == UnitTypes::Terran_Barracks
			|| Filter::GetType == UnitTypes::Zerg_Spawning_Pool) && Filter::IsEnemy);
		if (closest != NULL && closest->isVisible()) {
			attacker->attack(closest);
		}
		else {
			attacker->attack(InformationManager::getInstance().enemyBarracks.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		closest = attacker->getClosestUnit(Filter::IsBuilding && !Filter::CanAttack && Filter::IsEnemy && Filter::IsVisible);
		if (closest != NULL && closest->isVisible()) {
			attacker->attack(closest);
		}
		else {
			attacker->attack(InformationManager::getInstance().enemyPassiveBuildings.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyAttackers.empty()) {
		closest = attacker->getClosestUnit(Filter::CanAttack && Filter::CanMove && Filter::IsEnemy);
		if (closest != NULL && closest->isVisible()) {
			attacker->attack(closest);
		}
		else {
			attacker->attack(InformationManager::getInstance().enemyAttackers.front()->getPosition());
		}
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		closest = attacker->getClosestUnit(Filter::IsBuilding && Filter::CanAttack && Filter::IsEnemy);
		if (closest != NULL && closest->isVisible()) {
			attacker->attack(closest);
		}
		else {
			attacker->attack(InformationManager::getInstance().enemyTowers.front()->getPosition());
		}
	}
}

bool OffenseManager::avoidTowers(BWAPI::Unit fighter) {
	InformationManager::getInstance().writeToLog("Started avoidTowers");
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
	InformationManager::getInstance().writeToLog("Started calculatePriority");
	int effectiveHp = enemy->getHitPoints() + enemy->getShields();

	int ourDamage = (Broodwar->self()->damage(ourUnit->getType().groundWeapon()) - enemy->getPlayer()->armor(enemy->getType())) * ourUnit->getType().maxGroundHits();

	//Integer division round up
	int hitsToKill = (effectiveHp + (ourDamage - 1)) / ourDamage;

	int damage = (enemy->getPlayer()->damage(enemy->getType().groundWeapon()) - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();

	int priority = damage / hitsToKill;

	return priority;
}