#include "ObserverTech.h"

//Author of class: Kasper

ObserverTech::ObserverTech() {

	setSquadSize(6);
	setStrategyVariables();

	if (Broodwar->self()->allUnitCount(cybercore) > 0){
		if (Broodwar->self()->allUnitCount(robotics) > 0){
			if (Broodwar->self()->allUnitCount(observatory) > 0){
				buildOrder = { observer, 
					evaluateStrategyRequest };
			}
			else {
				// observatory missing
				buildOrder = { observatory, zealot, dragoon, observer, 
					evaluateStrategyRequest };
			}
		}
		else {
			// robotics facillity and observatory missing
			buildOrder = { robotics, zealot, dragoon, 
				observatory, zealot, dragoon, observer, 
				evaluateStrategyRequest };
		}
	}
	else {
		// core, robotics and observatory missing
		buildOrder = { assimilator, cybercore, zealot, zealot,
			gasworkerRequest, gasworkerRequest,
			robotics, zealot, dragoon,
			observatory, zealot, dragoon, observer,
			evaluateStrategyRequest };
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void ObserverTech::evaluateStrategy(){

	//Broodwar << "Evaluating observer teching" << std::endl;
	InformationManager::getInstance().hasInvisDetection = true;
	StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);

}

void ObserverTech::setStrategyVariables(){
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




