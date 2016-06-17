#include "OffenseManager.h"

using namespace BWAPI;
/*
TODO:

- Abuse andre bots dårlig micro

- Troops skal ikke bevæge sig ud af basen med mindre der ikke er nogle enemies i basen.

- Hvis vores scout dør kan vi ikke angribe

- EnemyAttackers liste bliver bugged når units går ind i bunkers fordi vi ikke længere kan se dem som attackers men de er ikke fjernet fra listen

- Færdiggøre metoden til at beskytte vores base.

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

	if (isFighter(unit) && unit->getPlayer() == Broodwar->self()){
		fighters.insert(unit);
		if (rushOngoing && InformationManager::getInstance().enemyBase != NULL) {
			unit->move(InformationManager::getInstance().enemyBase->getPosition());
		}
		else {
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
			Unitset readyFighters = unit->getUnitsInRadius(1240, (Filter::CanAttack || Filter::IsSpellcaster) && Filter::CanMove && Filter::IsAlly);
			if (InformationManager::getInstance().calculateArmyStrength(readyFighters)
				< InformationManager::getInstance().enemyArmyStrength
				&& InformationManager::getInstance().ourBase->getRegion() != BWTA::getRegion(unit->getTilePosition())) {

				if (rushOngoing) {
					if (squadSize >= 18){}
					else if (squadSize >= 12){
						squadSize += 3;
					}
					else {
						setSquadSize(squadSize + 6);
					}
				}
				rushOngoing = false;

				for (Unit fighter : fighters) {
					if (squad.find(unit) == squad.end()) {
						squad.insert(unit);
					}
				}
				if (debugging){
					Broodwar << "Their army is stronger" << std::endl;
				}

				readyFighters.move(InformationManager::getInstance().ourBase->getPosition());
			}
			else {
				fightBack(unit);
			}
		}
		else if (unit->getLastCommand().getType() != NULL && unit != NULL && InformationManager::getInstance().enemyBase != NULL
			&& (rushOngoing || !getEnemiesInOurRegion().empty())
			&& (unit->isIdle()
			|| (unit->getLastCommand().getType() == UnitCommandTypes::Move && unit->getLastCommand().getTargetPosition() != InformationManager::getInstance().ourBase->getPosition())
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
		rushOngoing = true;
		rush(squad);
		squad.clear();
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
				attackers.move(attackers.getPosition());
				attackers.move(InformationManager::getInstance().enemyBase->getPosition(), true);
				lastChecked = BWAPI::Broodwar->getFrameCount();
			}
			else {
				if (debugging){
					Broodwar << "Failed to rush, enemyBase was null " << std::endl;
				}

			}
		}
	}
	else {

		if (debugging){
			Broodwar << "Failed to rush attackers was empty " << std::endl;
		}

	}
	return true;

}

bool OffenseManager::fightBack(BWAPI::Unit attackedUnit) {
	if (attackedUnit != NULL) {
		BWAPI::Unit attacker = attackedUnit->getClosestUnit(Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding);
		BWAPI::Unitset nearbyEnemies = attackedUnit->getUnitsInRadius(128, Filter::IsEnemy && Filter::IsAttacking && !Filter::IsWorker && !Filter::IsBuilding && Filter::IsVisible);
		BWAPI::Unitset nearbyAllies = attackedUnit->getUnitsInRadius(128, !Filter::IsEnemy && !Filter::IsWorker && !Filter::IsBuilding);
		if (attacker != NULL) {
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
				if (bestTarget != NULL) {
					Broodwar->drawCircleMap(bestTarget->getPosition(), 15, Colors::Brown);
					smartAttackUnit(attackedUnit, bestTarget);
					getHelp(attackedUnit, bestTarget);
					return true;
				}
			}
			if (!attackedUnit->isAttacking()) {
				smartAttackUnit(attackedUnit, attacker);
			}
			getHelp(attackedUnit, attacker);
			return true;
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
			&& badGuy != NULL
			&& fighters.find(helper) != fighters.end()) {

			smartAttackUnit(helper, badGuy);
			return true;
		}
		else {
			return false;

			if (debugging){

				Broodwar << "Failed to get help, helper was null" << std::endl;
			}
		}
	}
	return false;
}

void OffenseManager::searchAndDestroy(BWAPI::Unit attacker) {
	//Called when our fighters are idle in the enemy base
	//Finds units to kill and kills them
	Unit closest;
	closest = attacker->getClosestUnit((Filter::CanAttack || Filter::IsSpellcaster) && Filter::CanMove && Filter::IsEnemy, 2240);
	if (closest) {
		smartAttackUnit(attacker, closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsBuilding && (Filter::CanAttack || Filter::GetType == UnitTypes::Terran_Bunker) && Filter::IsEnemy, 2240);
	if (closest) {
		smartAttackUnit(attacker, closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsWorker && Filter::IsEnemy, 2240);
	if (closest) {
		smartAttackUnit(attacker, closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::CanProduce && Filter::IsBuilding && !Filter::IsResourceDepot && Filter::IsEnemy, 2240);
	if (closest) {
		smartAttackUnit(attacker, closest);
		FixWrongPriority(closest);
		return;
	}
	closest = attacker->getClosestUnit(Filter::IsBuilding && !Filter::CanAttack && Filter::IsEnemy && Filter::IsVisible, 2240);
	if (closest) {
		smartAttackUnit(attacker, closest);
		FixWrongPriority(closest);
		return;
	}
	if (!InformationManager::getInstance().enemyAttackers.empty()) {
		smartAttackUnit(attacker, InformationManager::getInstance().enemyAttackers.front());
	}
	else if (!InformationManager::getInstance().enemyTowers.empty()) {
		smartAttackUnit(attacker, InformationManager::getInstance().enemyTowers.front());
	}
	else if (!InformationManager::getInstance().enemyWorkers.empty()) {
		smartAttackUnit(attacker, InformationManager::getInstance().enemyWorkers.front());
	}
	else if (!InformationManager::getInstance().enemyBarracks.empty()) {
		smartAttackUnit(attacker, InformationManager::getInstance().enemyBarracks.front());
	}
	else if (!InformationManager::getInstance().enemyPassiveBuildings.empty()) {
		smartAttackUnit(attacker, InformationManager::getInstance().enemyPassiveBuildings.front());
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
				return 100;
			}


			/*int damage = (enemy->getPlayer()->damage(enemy->getType().groundWeapon()) - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();*/

			int damage = (enemy->getType().groundWeapon().damageAmount() - Broodwar->self()->armor(ourUnit->getType())) * enemy->getType().maxGroundHits();
			int priority;
			if (hitsToKill != 0) {
				priority = damage / hitsToKill;
			}
			else {
				return 100;
			}
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
		|| unit->getType() == UnitTypes::Protoss_Carrier
		|| unit->getType() == UnitTypes::Protoss_Observer){

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

void OffenseManager::FixWrongPriority(BWAPI::Unit closest) {
	for (Unit troop : fighters) {
		if (closest != NULL && troop != NULL
			&& troop->getLastCommand().getType() != NULL
			&& troop->getLastCommand().getTarget() != NULL) {

			if (troop->getLastCommand().getType() == UnitCommandTypes::Attack_Unit
				&& calculatePriority(troop->getLastCommand().getTarget(), NULL) < calculatePriority(closest, NULL)) {
				smartAttackUnit(troop, closest);
			}
		}
	}
}


Unitset OffenseManager::getEnemiesInOurRegion(){
	Region ourRegion = Broodwar->getRegionAt(InformationManager::getInstance().ourBase->getPosition());
	return ourRegion->getUnits(Filter::IsEnemy);
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

			if (BWTA::getRegion(explorer->getTilePosition()) == InformationManager::getInstance().enemyBase->getRegion()
				|| checkedMainBase) {
				checkedMainBase = true;
				for (exploredPositions = edgeOfBase.begin(); exploredPositions != edgeOfBase.end(); exploredPositions++) {
					Position p = *exploredPositions;
					if (!Broodwar->isVisible(TilePosition(p))
						&& BWTA::isConnected(explorer->getTilePosition(), TilePosition(p))
						&& explorer->hasPath(p)
						&& p.isValid()) {
						foundPosition = true;
						explorer->move(p);
						break;
					}
				}
			}
			else {
				explorer->move(InformationManager::getInstance().enemyBase->getPosition());
				return;
			}

			for (exploredPositions = edgeOfBase.begin(); exploredPositions != edgeOfBase.end(); exploredPositions++) {
				Position p = *exploredPositions;
				if (!Broodwar->isExplored(TilePosition(p))
					&& BWTA::isConnected(explorer->getTilePosition(), TilePosition(p))
					&& explorer->hasPath(p)
					&& p.isValid()) {
					foundPosition = true;
					explorer->move(p);
					break;
				}
			}

			if (!foundPosition && checkedMainBase) {
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
	}
}

void OffenseManager::smartAttackUnit(BWAPI::Unit attacker, BWAPI::Unit target) {

	if (!attacker || !target)
	{
		return;
	}

	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		return;
	}

	// get the unit's current command
	BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

	// if we've already told this unit to attack this target, ignore this command
	if (currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit &&	currentCommand.getTarget() == target)
	{
		return;
	}

	// if nothing prevents it, attack the target
	attacker->attack(target);
}