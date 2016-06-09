#include "AggressiveZealotRush.h"

AggressiveZealotRush::AggressiveZealotRush() {

	Broodwar << name << std::endl;

	setSquadSize(3);
	setStrategyVariables();
	OffenseManager::getInstance().rushOngoing = false;

	buildOrder = { // base Build order
		probe, gateway, gateway, //2 gateways on 9
		scoutRequest,
		probe, probe, zealot, pylon, // 13/16-24
		zealot, zealot, zealot, pylon, // 19/24-32
		zealot, zealot, zealot, zealot, pylon, // 27/32-40
		zealot, zealot, zealot, zealot, pylon, evaluateStrategyRequest }; // 35/40-48

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void AggressiveZealotRush::evaluateStrategy(){

	Broodwar << "Evaluating aggressive zealot rush" << std::endl;

	if (InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
		>= InformationManager::getInstance().calculateArmyStrength(Broodwar->self())
		|| InformationManager::getInstance().enemyTowers.size() >= 2){

		// convert to midgame
		StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);

	}
	else {

		// continue with the zealot rush
		StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);

	}

}

void AggressiveZealotRush::setStrategyVariables(){
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




