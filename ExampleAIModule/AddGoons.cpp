#include "AddGoons.h"

AddGoons::AddGoons() {

	Broodwar << name << std::endl;

	setSquadSize(6);
	setStrategyVariables();


	if (Broodwar->self()->allUnitCount(cybercore) >= 1
		&& Broodwar->self()->allUnitCount(assimilator) >= 1){

		buildOrder = { probe, probe, gateway,
			zealot, probe, zealot, dragoon, dragoon, probe, dragoon,
			evaluateStrategyRequest };
	}
	else {
		buildOrder = { assimilator, probe, cybercore,
			gasworkerRequest, gasworkerRequest,
			probe, probe, gateway, zealot, zealot,
			dragoon, dragoon, dragoon, goon_range,
			evaluateStrategyRequest };
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void AddGoons::evaluateStrategy(){

	Broodwar << "Evaluating AddGoon strategy" << std::endl;

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
			// continue with the zealot rush
			StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);

		}
	}


	
}

void AddGoons::setStrategyVariables(){
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




