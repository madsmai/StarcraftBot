#include "CommonZealotRush.h"


CommonZealotRush::CommonZealotRush(){

	Broodwar << name << std::endl;

	setSquadSize();

	buildOrder = { // base Build order
		scoutRequest,
		probe, probe, gateway, // 10
		probe, probe, gateway, // 12
		probe, zealot, probe, pylon, // 16
		zealot, zealot, zealot, pylon, // 22
		zealot, zealot, zealot, pylon, evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}


void CommonZealotRush::evaluateStrategy(){

	Broodwar << "Evaluating common zealot rush" << std::endl;

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
