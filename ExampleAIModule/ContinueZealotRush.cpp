#include "ContinueZealotRush.h"


ContinueZealotRush::ContinueZealotRush() {

	Broodwar << name << std::endl;
	setSquadSize(6);
	setStrategyVariables();

	if (Broodwar->self()->allUnitCount(cybercore) >= 1){
		buildOrder = { zealot, dragoon, dragoon, 
			zealot, zealot, zealot, probe, 
			evaluateStrategyRequest };
	}
	else {
		buildOrder = { zealot, zealot, zealot, 
			probe, zealot, zealot, zealot, 
			evaluateStrategyRequest };
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void ContinueZealotRush::evaluateStrategy(){

	Broodwar << "Evaluating the continued zealot rush" << std::endl;

	if (InformationManager::getInstance().invisSpottet
		&& !InformationManager::getInstance().hasInvisDetection){
		StrategyManager::getInstance().setNextStrategy(StrategyManager::observerTech);
	}
	else {
		if (InformationManager::getInstance().calculateArmyStrength(OffenseManager::getInstance().fighters)
			>= InformationManager::getInstance().enemyArmyStrength
			|| InformationManager::getInstance().enemyTowers.size() >= 2){

			// convert to midgame
			StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);
		}
		else {
			if (rand() % 2 > 0.5
				&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Cybernetics_Core) == 0
				&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Assimilator) == 0){
				Broodwar << "going goons from continue" << std::endl;
				StrategyManager::getInstance().setNextStrategy(StrategyManager::addGoons);
			}
			else {
				// continue with the zealot rush
				StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);
			}
		}
	}
}

void ContinueZealotRush::setStrategyVariables(){
	if (starter){
		InformationManager::getInstance().starter = true;
		InformationManager::getInstance().endgame = false;
	}
	else {
		InformationManager::getInstance().starter = false;
		InformationManager::getInstance().endgame = true;
	}

	if (antiAir){
		InformationManager::getInstance().antiAir = true;
	}
	else {
		InformationManager::getInstance().antiAir = false;
	}

	if (antiInvis){
		InformationManager::getInstance().antiInvis = true;
	}
	else {
		InformationManager::getInstance().antiInvis = false;
	}

	if (antiRush){
		InformationManager::getInstance().antiRush = true;
	}
	else {
		InformationManager::getInstance().antiRush = false;
	}

	if (antiTurtle){
		InformationManager::getInstance().antiTurtle = true;
	}
	else {
		InformationManager::getInstance().antiTurtle = false;
	}

}




