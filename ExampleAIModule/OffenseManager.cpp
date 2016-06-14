#include "OffenseManager.h"

using namespace BWAPI;
/*
TODO:

- Abuse andre bots d�rlig micro

- Troops skal ikke bev�ge sig ud af basen med mindre der ikke er nogle enemies i basen.

- Hvis vores scout d�r kan vi ikke angribe

- EnemyAttackers liste bliver bugged n�r units g�r ind i bunkers fordi vi ikke l�ngere kan se dem som attackers men de er ikke fjernet fra listen

- F�rdigg�re metoden til at beskytte vores base.

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
		else {
			unit->patrol(InformationManager::getInstance().ourBase->getPosition());
			squad.insert(unit);
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
				fighters.move(InformationManager::getInstance().ourBase->getPosition());
			}
			else {
				fightBack(unit);
			}
		}
		else if (unit->getLastCommand().getType() != NULL && unit != NULL && InformationManager::getInstance().enemyBase != NULL
			&& (unit->isIdle() || unit->getLastCommand().getType() == UnitCommandTypes::Move 
			|| (unit->getLastCommand().getType() == UnitCommandTypes::Attack_Unit && !unit->isAttacking()))) {

			searchAndDestroy(unit);
			explore(unit);
		}
		if (unit->isMoving()
			&& unit->getLastCommand().getType() != NULL
			&& unit->getLastCommand().getTarget() != NULL
			&& unit->getLastCommand().getType() == UnitCommandTypes::Attack_Unit) {

			Broodwar->drawLine(CoordinateType::Enum::Map, unit->getPosition().x, unit->getPosition().y,
				unit->getLastCommand().getTarget()->getPosition().x, unit->getLastCommand().getTarget()->getPosition().y, Colors::Red);
		}
	}
	if (squad.size() >= squadSize && InformationManager::getInstance().enemyBase != NULL) {
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
				attackers.move(InformationManager::getInstance().enemyBase->getPosition(), true);
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
				if (InformationManager::getInstance().enemyBase != NULL
					&& InformationManager::getInstance().enemyBase->getRegion() != BWTA::getRegion(attackedUnit->getTilePosition())) {
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
	closest = attacker->getClosestUnit((Filter::CanAttack || Filter::IsSpellcaster) && Filter::CanMove && Filter::IsEnemy, 1240);
	if (properClosestTarget(closest, attacker)) {
		attacker->attack(closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsBuilding && (Filter::CanAttack || Filter::GetType == UnitTypes::Terran_Bunker) && Filter::IsEnemy, 1240);
	if (properClosestTarget(closest, attacker)) {
		attacker->attack(closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsWorker && Filter::IsEnemy, 1240);
	if (properClosestTarget(closest, attacker)) {
		attacker->attack(closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::CanProduce && Filter::IsBuilding && !Filter::IsResourceDepot && Filter::IsEnemy, 1240);
	if (properClosestTarget(closest, attacker)) {
		attacker->attack(closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsBuilding && !Filter::CanAttack && Filter::IsEnemy && Filter::IsVisible, 1240);
	if (properClosestTarget(closest, attacker)) {
		attacker->attack(closest);
		FixWrongPriority(closest);
		return;
	}


}

int OffenseManager::calculatePriority(Unit enemy, Unit ourUnit) {
	InformationManager::getInstance().writeToLog("Started calculatePriority");
	if (enemy != NULL)  {
		if (ourUnit != NULL && enemy->getType().canAttack() && enemy->getType().canMove() && ourUnit->getType().canAttack() && ourUnit->getType().canMove()) {

			//Is a fighter

			int effectiveHp = enemy->getHitPoints() + enemy->getShields();

			int ourDamage = (Broodwar->self()->damage(ourUnit->getType().groundWeapon()) - enemy->getPlayer()->armor(enemy->getType())) * ourUnit->getType().maxGroundHits();

			//Integer division round up
			int hitsToKill;
			if (ourDamage != 0) {
				hitsToKill = (effectiveHp + (ourDamage - 1)) / ourDamage;
			}
			else {
				hitsToKill = effectiveHp + (ourDamage - 1);
			}


			/*int damage = (enemy->getPlayer()->damage(enemy->getType().groundWeapon()) - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();*/

			int damage = (enemy->getType().groundWeapon().damageAmount() - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();

			int priority = damage / hitsToKill;

			return priority + 100;
		}
		else if (enemy->getType().isBuilding() && (enemy->getType().canAttack() || enemy->getType() == UnitTypes::Terran_Bunker)) {
			//Its a tower
			return 90;
		}
		else if (enemy->getType().isWorker()) {
			return 80;
		}
		else if (enemy->getType().isBuilding() && enemy->getType().canProduce() && !enemy->getType().isResourceDepot()) {
			//Its a factory
			return 70;
		}
		else if (enemy->getType().isBuilding() && !enemy->getType().canAttack()) {
			//Passivebuilding
			return 2;
		}
		else {
			//Unknown
			return 1;
		}
	}
	return 0;
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

	if (!unit->isConstructing()){
		// Unit is a reaver
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

		// Unit is a carrier
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
}

bool OffenseManager::properClosestTarget(BWAPI::Unit target, BWAPI::Unit attacker) {
	return target != NULL && attacker != NULL
		&& target->isVisible()
		&& BWTA::isConnected(attacker->getTilePosition(), target->getTilePosition())
		&& target->exists()
		&& target->getPosition().isValid();
}

void OffenseManager::FixWrongPriority(BWAPI::Unit closest) {
	for (Unit troop : fighters) {
		if (closest != NULL && troop != NULL
			&& troop->getLastCommand().getType() != NULL
			&& troop->getLastCommand().getTarget() != NULL) {

			if (troop->getLastCommand().getType() == UnitCommandTypes::Attack_Unit
				&& calculatePriority(troop->getLastCommand().getTarget(), NULL) < calculatePriority(closest, NULL)) {
				/*troop->attack(closest);*/
			}
		}
	}
}

bool OffenseManager::avoidTowers(BWAPI::Unit fighter) {
	//InformationManager::getInstance().writeToLog("Started avoidTowers");
	bool underTower = false;

	//std::vector<BWAPI::Unit>::iterator it;
	//if (!InformationManager::getInstance().enemyTowers.empty() && fighter != NULL){
	//	for (it = InformationManager::getInstance().enemyTowers.begin(); it != InformationManager::getInstance().enemyTowers.end(); it++) {
	//		BWAPI::Unit tower = *it;
	//		if (fighter->getPlayer() == Broodwar->self()) {
	//			if (fighter->isFlying()) {

	//				BWAPI::Unitset jeopardizedUnits = tower->getUnitsInWeaponRange(tower->getType().airWeapon(), Filter::IsEnemy);
	//				if (jeopardizedUnits.find(fighter) != jeopardizedUnits.end()) {
	//					//Its jeopardized
	//					underTower = true;
	//				}
	//				else {
	//					underTower = false;
	//				}
	//			}
	//			else {
	//				BWAPI::Unitset jeopardizedUnits = tower->getUnitsInWeaponRange(tower->getType().groundWeapon(), Filter::IsEnemy);
	//				if (jeopardizedUnits.find(fighter) != jeopardizedUnits.end()) {
	//					//Its jeopardized
	//					underTower = true;
	//				}
	//				else {
	//					underTower = false;
	//				}
	//			}
	//		}
	//		else {
	//			if (fighter->isFlying()) {
	//				BWAPI::Unitset defendedUnits = tower->getUnitsInWeaponRange(tower->getType().airWeapon(), Filter::IsAlly);
	//				if (defendedUnits.find(fighter) != defendedUnits.end()) {
	//					//Its defended
	//					underTower = true;
	//				}
	//				else {
	//					underTower = false;
	//				}
	//			}
	//			else {
	//				BWAPI::Unitset defendedUnits = tower->getUnitsInWeaponRange(tower->getType().groundWeapon(), Filter::IsAlly);
	//				if (defendedUnits.find(fighter) != defendedUnits.end()) {
	//					//Its defended
	//					underTower = true;
	//				}
	//				else {
	//					underTower = false;
	//				}
	//			}
	//		}
	//	}
	//}

	return underTower;
}

Unitset OffenseManager::getEnemiesInOurRegion(){
	Region ourRegion = Broodwar->getRegionAt(InformationManager::getInstance().ourBase->getPosition());
	return ourRegion->getUnits(Filter::IsEnemy);
}

void OffenseManager::defendOurBase(){
	//if (!fighters.empty()) {
	//	fighters.move(InformationManager::getInstance().ourBase->getPosition());
	//}
}

void OffenseManager::explore(Unit explorer) {
	if (explorer != NULL 
		&& explorer->getTilePosition().x != NULL && explorer->getTilePosition().y != NULL
		&& explorer->isIdle()
		&& BWTA::getRegion(explorer->getTilePosition()) != NULL) {
		if (InformationManager::getInstance().enemyBase != NULL
			&& BWTA::getRegion(explorer->getTilePosition()) != InformationManager::getInstance().ourBase->getRegion()) {

			/*std::vector<Position> edgeOfBase = InformationManager::getInstance().enemyBase->getRegion()->getPolygon();*/

			std::vector<Position> edgeOfBase = BWTA::getRegion(explorer->getTilePosition())->getPolygon();

			std::vector<Position>::iterator exploredPositions;
			bool foundPosition = false;

			for (exploredPositions = edgeOfBase.begin(); exploredPositions != edgeOfBase.end(); exploredPositions++) {
				Position p = *exploredPositions;
				if (!Broodwar->isExplored(TilePosition(p)) 
					&& BWTA::isConnected(explorer->getTilePosition(), TilePosition(p))
					&& p.isValid()) {
					foundPosition = true;
					explorer->move(p);
					break;
				}
			}
			if (!foundPosition) {
				goScout(explorer);
			}
			//Position randomEdge = edgeOfBase[rand() % edgeOfBase.size()];
			//explorer->move(edgeOfBase[rand() % edgeOfBase.size()]);
		}
	}
}
void OffenseManager::goScout(BWAPI::Unit scout){
	InformationManager::getInstance().baseLocations = BWTA::getBaseLocations();
	double minDistance = 9999999;
	BWTA::BaseLocation* currentBase = BWTA::getNearestBaseLocation(scout->getPosition());

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = InformationManager::getInstance().baseLocations.begin(); it != InformationManager::getInstance().baseLocations.end(); it++){
		BWTA::BaseLocation* nextBase = *it;
		Broodwar->pingMinimap(nextBase->getPosition());
		// Remove our baselocation
		if (!Broodwar->isExplored(nextBase->getTilePosition())) {
			scout->move(nextBase->getPosition());
		}
		//if (nextBase->getPosition() == InformationManager::getInstance().ourBase->getPosition()
		//	|| currentBase->getPosition() == InformationManager::getInstance().nextBase->getPosition()){
		//	continue;
		//}
		//else if (currentBase->getGroundDistance(InformationManager::getInstance().nextBase) < minDistance) {
		//	minDistance = currentBase->getGroundDistance(InformationManager::getInstance().nextBase);
		//	Broodwar->sendText("Moving to enemy base");
		//	scout->move(nextBase->getPosition());
		//}
	}
}