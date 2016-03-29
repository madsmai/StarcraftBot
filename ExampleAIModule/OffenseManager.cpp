#include "OffenseManager.h"

/*
TODO:
- Sørg for at samle zealots sammen og angrib med dem sammen, det samlede antal skal varierer
	alt efter modstanderens forsvar
- En måde at få zealots til at prioriterer hvad de angriber
- Mangler BWTA
*/


void OffenseManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->getPlayer() == BWAPI::Broodwar->self()){
		std::vector<BWAPI::Unit>::iterator it;

		//Loop through zealots
		for (it = zealots.begin(); it != zealots.end(); it++){
			if (*it == unit){
				zealots.erase(it);
				break;
			}
		}
	}
}

void OffenseManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->getPlayer() == BWAPI::Broodwar->self()){
		zealots.push_back(unit);
	}
}

void OffenseManager::onFrame(){
	//Loop through zealots
	std::vector<BWAPI::Unit>::iterator it;
	for (it = zealots.begin(); it != zealots.end(); it++){
		BWAPI::Unit unit = *it;
		if (unit->isIdle()){
			unit->attack(unit->getClosestUnit((BWAPI::Filter::IsEnemy && BWAPI::Filter::CanAttack), 50));
		}
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot && unit->isUnderAttack()) {
			unit->attack(unit->getClosestUnit((BWAPI::Filter::IsEnemy && BWAPI::Filter::CanAttack), 4));
		}
	}

	//Need BWTA or whatever it's called
	//if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot) >= zealotMax){
		//for (BWAPI::Unit zealot : zealots){
			//zealot->attack(enemyBase->getPosition());
		//}
		//zealotMax = zealotMax * 2;
	//}
}

OffenseManager& OffenseManager::getInstance(){ //Return ref to OffenseManager object
	static OffenseManager i; //Make static instance i
	return i;
}
