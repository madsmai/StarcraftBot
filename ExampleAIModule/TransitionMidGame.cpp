#include "TransitionMidGame.h"


TransitionMidGame::TransitionMidGame() {

	Broodwar << name << std::endl;

	setSquadSize(3);

	

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
	else if (Broodwar->self()->allUnitCount(forge) == 0
		&& Broodwar->self()->allUnitCount(assimilator) != 0
		&& Broodwar->self()->allUnitCount(cybercore) != 0){

		// after a DT rush
		buildOrder = { probe, probe, probe, probe, 
			pylon, forge, probe, probe, probe, 
			probe, probe, probe, pylon, 
			cannon, cannon,
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
