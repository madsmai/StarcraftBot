#include "Carriers.h"

Carriers::Carriers() {

	Broodwar << name << std::endl;

	setSquadSize(6);
	setStrategyVariables();

	buildOrder = { stargate, probe, cannon, probe,
		fleet_beacon, stargate, zealot, dragoon,
		carrier, zealot, dragoon, carrier, carrier_capacity, stargate,
		evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void Carriers::evaluateStrategy(){

	Broodwar << "Evaluating carrier strategy" << std::endl;

	if (InformationManager::getInstance().invisSpottet
		&& !InformationManager::getInstance().hasInvisDetection){
		StrategyManager::getInstance().setNextStrategy(StrategyManager::observerTech);
	}
	else {
		// continue with carrier spam
		StrategyManager::getInstance().setNextStrategy(StrategyManager::carrier_spam);
	}

}

void Carriers::setStrategyVariables(){
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




