#include "Reavers.h"

Reavers::Reavers() {

	Broodwar << name << std::endl;

	setSquadSize(3);

	buildOrder = { robotics, probe, probe, pylon, 
		supportbay,robotics, zealot, zealot, pylon,
		reaver, zealot, reaver, pylon,
		evaluateStrategyRequest }; 

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void Reavers::evaluateStrategy(){

	Broodwar << "Evaluating reaver strategy" << std::endl;

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



