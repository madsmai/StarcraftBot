#include "Carriers.h"

Carriers::Carriers() {

	Broodwar << name << std::endl;

	setSquadSize(3);

	buildOrder = { stargate, probe, probe, pylon,
		fleet_beacon, stargate, zealot, zealot, pylon,
		carrier, zealot, carrier, pylon, carrier_capacity, carrier,
		evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void Carriers::evaluateStrategy(){

	Broodwar << "Evaluating reaver strategy" << std::endl;

	if (InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
		>= InformationManager::getInstance().calculateArmyStrength(Broodwar->self())
		|| InformationManager::getInstance().enemyTowers.size() >= 2){

		// convert to midgame
		//StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);

	}
	else {

		// continue with the zealot rush
		//StrategyManager::getInstance().setNextStrategy(StrategyManager::none);

	}



}



