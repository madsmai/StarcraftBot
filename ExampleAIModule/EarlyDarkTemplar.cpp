#include "EarlyDarkTemplar.h"


EarlyDarkTemplar::EarlyDarkTemplar() {

	setSquadSize(1);
	setStrategyVariables();

	buildOrder = { // base build
		probe, gateway, probe, scoutRequest, assimilator, probe,
		cybercore, gasworkerRequest, gasworkerRequest, probe, forge,
		citadel, probe, gasworkerRequest, cannon, cannon, archives,
		gateway, dtemplar, dtemplar, pylon,
		dtemplar, probe, probe, cannon,
		evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}
}


void EarlyDarkTemplar::evaluateStrategy(){

	//Broodwar << "Evaluating early dark templar" << std::endl;

	if (InformationManager::getInstance().invisSpottet
		&& !InformationManager::getInstance().hasInvisDetection){
		StrategyManager::getInstance().setNextStrategy(StrategyManager::observerTech);
	}
	else {
		StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);
	}
}

void EarlyDarkTemplar::setStrategyVariables(){
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




