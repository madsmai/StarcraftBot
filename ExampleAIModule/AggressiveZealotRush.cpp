#include "AggressiveZealotRush.h"

AggressiveZealotRush::AggressiveZealotRush() {

	Broodwar << name << std::endl;

	setSquadSize();

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

		// something endgame

	}
	else {

		// continue with the zealot rush
		StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);

	}



}



