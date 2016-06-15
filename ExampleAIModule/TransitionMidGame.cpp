#include "TransitionMidGame.h"


TransitionMidGame::TransitionMidGame() {

	Broodwar << name << std::endl;

	setSquadSize(6);
	setStrategyVariables();

	if (StrategyManager::getInstance().transistionMidCalled){
		// Transistion mid has already been run
		buildOrder = { evaluateStrategyRequest };
	}
	else if (Broodwar->self()->allUnitCount(assimilator) == 0
		&& Broodwar->self()->allUnitCount(forge) == 0
		&& Broodwar->self()->allUnitCount(cybercore) == 0){

		// after a zealot rush
		buildOrder = { probe, assimilator, probe, probe,
			gasworkerRequest, gasworkerRequest,
			probe, probe, probe,
			forge, cybercore, probe, cannon,
			cannon, probe, probe, cannon,
			probe, probe,
			evaluateStrategyRequest };
	}
	else if (Broodwar->self()->allUnitCount(forge) > 0
		&& Broodwar->self()->allUnitCount(assimilator) > 0
		&& Broodwar->self()->allUnitCount(cybercore) > 0){

		// after a DT rush or observerTech
		buildOrder = { cannon, probe, zealot, zealot,
			probe, dragoon, dragoon,
			probe, cannon, zealot,
			probe, probe, ground_armor,
			probe, dragoon, probe, zealot, dragoon,
			probe, cannon,
			evaluateStrategyRequest };
	}
	else if (Broodwar->self()->allUnitCount(forge) == 0
		&& Broodwar->self()->allUnitCount(assimilator) > 0
		&& Broodwar->self()->allUnitCount(cybercore) > 0){

		// after a rush with goons
		buildOrder = { probe, zealot, zealot,
			probe, dragoon, zealot, probe,
			probe, probe, probe,
			forge, probe, cannon,
			cannon, probe, probe, cannon,
			probe, probe,
			evaluateStrategyRequest };
	}

	else {
		Broodwar << "Add more cases to midGame transition strategy" << std::endl;
		abort();
	}


	// queue it up
	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}
}


void TransitionMidGame::evaluateStrategy(){

	Broodwar << "Evaluating mid game strategy" << std::endl;

	StrategyManager::getInstance().transistionMidCalled = true;

	if (InformationManager::getInstance().invisSpottet
		&& !InformationManager::getInstance().hasInvisDetection){
		StrategyManager::getInstance().setNextStrategy(StrategyManager::observerTech);
	}
	else {
		if (StrategyManager::getInstance().carriersCalled){
			// Carriers have already been run, go back to spamming
			StrategyManager::getInstance().setNextStrategy(StrategyManager::carrier_spam);
			return;
		}
		else {
			// go tech carriers
			StrategyManager::getInstance().setNextStrategy(StrategyManager::carriers);
		}
	}
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
