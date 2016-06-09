#include "OffenseManager.h"

using namespace BWAPI;
/*
TODO:
- Expansions bugger os, helt vildt.
- De bev�ger sig frem og tilbage
- Dr�ber ikke workers i expansion f�rst
- Tror det er fordi de bliver sat til at bev�ge sig mod hovedbasen, n�r den, bliver idle, laver search and destroy.

- Abuse andre bots d�rlig micro
- Vi g�r ikke noget mod et rush der er hurtigere end vores
-Ikke nogle metoder til at f� vores zealots til at g�re noget i basen

- Early dark templars strategien bugger

- Shift queue squad til at move et sted og s� move til basen s� de grupper


*/

void OffenseManager::onUnitDestroy(Unit unit){
	if (unit->getPlayer() == Broodwar->self()) {
		InformationManager::getInstance().writeToLog("One of our dudes died");
		if (fighters.find(unit) != fighters.end()) {
			fighters.erase(fighters.find(unit));
		}
		else if (unit = coward) {
			coward = NULL;
		}
	}
}

void OffenseManager::onUnitComplete(Unit unit){

	if (isFighter(unit)){
		fighters.insert(unit);
		if (rushOngoing && InformationManager::getInstance().enemyBase != NULL) {
			unit->move(InformationManager::getInstance().enemyBase->getPosition());
		}
	}

}

void OffenseManager::onFrame(){

	for (Unit unit : fighters) {

		if (unit->getType() == UnitTypes::Protoss_Reaver || unit->getType() == UnitTypes::Protoss_Carrier){
			fillReaverOrCarrier(unit);
		}


		if (unit != NULL && unit->isUnderAttack()) {
			InformationManager::getInstance().writeToLog("Under attack");
			if (InformationManager::getInstance().calculateArmyStrength(Broodwar->self())
				< InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
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
			&& (unit->isIdle() || unit->getLastCommand().getType() != UnitCommandTypes::Attack_Unit )) {

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
	if (runFrames + 18 <= Broodwar->getFrameCount() && coward != NULL) {
		InformationManager::getInstance().writeToLog("Started Coward purging");
		searchAndDestroy(coward);
		coward = NULL;
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
				attackers.move(attackers.getPosition());
				attackers.move(InformationManager::getInstance().enemyBase->getPosition(),true);
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
				if (coward = NULL) {
					coward = attackedUnit;
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
	Unit closest;
	if (!InformationManager::getInstance().enemyAttackers.empty()) {
		closest = attacker->getClosestUnit(Filter::CanAttack && Filter::CanMove && Filter::IsEnemy, 1240);
		if (properClosestTarget(closest, attacker)) {
			attacker->attack(closest);
		}
	}
	else if (!InformationManager::getInstance().enemyWorkers.empty()) {
		closest = attacker->getClosestUnit(Filter::IsWorker && Filter::IsEnemy,1240);
		if (properClosestTarget(closest, attacker)) {
			attacker->attack(closest);
		}
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		closest = attacker->getClosestUnit((Filter::GetType == UnitTypes::Protoss_Gateway
			|| Filter::GetType == UnitTypes::Terran_Barracks
			|| Filter::GetType == UnitTypes::Zerg_Spawning_Pool) && Filter::IsEnemy, 1240);
		if (properClosestTarget(closest, attacker)) {
			attacker->attack(closest);
		}
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		closest = attacker->getClosestUnit(Filter::IsBuilding && !Filter::CanAttack && Filter::IsEnemy && Filter::IsVisible, 1240);
		if (properClosestTarget(closest, attacker)) {
			attacker->attack(closest);
		}
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		closest = attacker->getClosestUnit(Filter::IsBuilding && Filter::CanAttack && Filter::IsEnemy, 1240);
		if (properClosestTarget(closest,attacker)) {
			attacker->attack(closest);
		}
	}
	//Finding other units that should also attack this
	for (Unit u : fighters) {
		if (u->getLastCommand().getType() != NULL
			&& u->getLastCommand().getTarget() != NULL
			&& closest != NULL
			&& u->getLastCommand().getType() == UnitCommandTypes::Attack_Unit
			&& u->getLastCommand().getTarget()->getType() != closest->getType()
			&& !u->isUnderAttack()
			&& u->getLastCommand().getTarget()->getType().canAttack()
			&& u->getLastCommand().getTarget()->getType().canMove()){

				u->attack(closest);
		}
		else if (u->getLastCommand().getType() != NULL 
			&& closest != NULL
			&& u->getLastCommand().getType() != UnitCommandTypes::Attack_Unit) {

			u->attack(closest);
		}
	}
	if (!attacker->isMoving() && !attacker->isAttacking() ) {
		attacker->move(InformationManager::getInstance().enemyBase->getPosition());
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

bool OffenseManager::isFighter(Unit unit){

	if (unit->getType() == UnitTypes::Protoss_Zealot
		|| unit->getType() == UnitTypes::Protoss_Dragoon
		|| unit->getType() == UnitTypes::Protoss_Dark_Templar
		|| unit->getType() == UnitTypes::Protoss_High_Templar
		|| unit->getType() == UnitTypes::Protoss_Dark_Archon
		|| unit->getType() == UnitTypes::Protoss_Archon
		|| unit->getType() == UnitTypes::Protoss_Reaver
		|| unit->getType() == UnitTypes::Protoss_Carrier){

		return true;
	}
	return false;
}

void OffenseManager::fillReaverOrCarrier(Unit unit){

	// if it is a reaver
	if (unit->getType() == UnitTypes::Protoss_Reaver){
		if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Reaver_Capacity)
			== Broodwar->self()->getMaxUpgradeLevel(UpgradeTypes::Reaver_Capacity)
			&& unit->getScarabCount() < 10){
			unit->train(UnitTypes::Protoss_Scarab);
		}
		else if (unit->getScarabCount() < 5){
			unit->train(UnitTypes::Protoss_Scarab);
		}

	}

	// if it is a carrier
	else if (unit->getType() == UnitTypes::Protoss_Carrier){
		if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Carrier_Capacity)
			== Broodwar->self()->getMaxUpgradeLevel(UpgradeTypes::Carrier_Capacity)
			&& unit->getScarabCount() < 8){
			unit->train(UnitTypes::Protoss_Interceptor);
		}
		else if (unit->getInterceptorCount() < 4){
			unit->train(UnitTypes::Protoss_Interceptor);
		}
	}

}

bool OffenseManager::properClosestTarget(BWAPI::Unit target, BWAPI::Unit attacker) {
	return target != NULL && target->isVisible() && BWTA::isConnected(attacker->getTilePosition(), target->getTilePosition())
		&& target->exists() && target->getPosition().isValid();
}