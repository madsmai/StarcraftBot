#include "ContinueZealotRush.h"


ContinueZealotRush::ContinueZealotRush() {

	Broodwar << name << std::endl;
	setSquadSize(3);
	setStrategyVariables();
	OffenseManager::getInstance().rushOngoing = true;

	buildOrder = { zealot, zealot, zealot, zealot, evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void ContinueZealotRush::evaluateStrategy(){

	Broodwar << "Evaluating the continued zealot rush" << std::endl;

	if (InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
		>= InformationManager::getInstance().calculateArmyStrength(Broodwar->self())
		|| InformationManager::getInstance().enemyTowers.size() >= 2){

		// convert to midgame
		StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);

	}
	else {
		StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);
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




