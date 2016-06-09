#include "TransitionMidGame.h"


TransitionMidGame::TransitionMidGame() {

	Broodwar << name << std::endl;

	OffenseManager::getInstance().rushOngoing = false;
	setSquadSize(3);
	setStrategyVariables();

	if (Broodwar->self()->allUnitCount(assimilator) == 0
		&& Broodwar->self()->allUnitCount(forge) == 0
		&& Broodwar->self()->allUnitCount(cybercore) == 0){

		// after a zealot rush
		buildOrder = { probe, assimilator, probe, probe,
			gasworkerRequest, gasworkerRequest,
			probe, pylon, probe, probe,
			forge, cybercore, probe, cannon,
			cannon, probe, probe, cannon,
			probe, pylon, probe,
			evaluateStrategyRequest };
	}
	else if (Broodwar->self()->allUnitCount(forge) != 0
		&& Broodwar->self()->allUnitCount(assimilator) != 0
		&& Broodwar->self()->allUnitCount(cybercore) != 0){

		// after a DT rush
		buildOrder = { cannon, probe, probe, probe, probe,
			pylon, probe, probe, probe,
			probe, probe, probe, pylon,
			cannon,
			evaluateStrategyRequest };

	}
	else {
		Broodwar << "Add more cases to midGame transition strategy" << std::endl;
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}
}


void TransitionMidGame::evaluateStrategy(){

	Broodwar << "Evaluating mid game strategy" << std::endl;
	StrategyManager::getInstance().setNextStrategy(StrategyManager::reavers);

}

void TransitionMidGame::setStrategyVariables(){
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
