#include "ContinueZealotRush.h"


ContinueZealotRush::ContinueZealotRush() {

	Broodwar << name << std::endl;

	setArmySize();

	if (Broodwar->self()->supplyTotal() / 2 - 5 <= Broodwar->self()->supplyUsed() / 2 + 8){

		buildOrder = { zealot, zealot, zealot, zealot, pylon, evaluateStrategyRequest };

	}
	else {
		buildOrder = { zealot, zealot, zealot, zealot, evaluateStrategyRequest };
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void ContinueZealotRush::evaluateStrategy(){

	Broodwar << "Evaluating aggressive zealot rush" << std::endl;

	if (InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy())
		>= InformationManager::getInstance().calculateArmyStrength(Broodwar->self())
		|| InformationManager::getInstance().enemyTowers.size() >= 2){

		// something endgame

	}
	else {

		StrategyManager::getInstance().setNextStrategy(StrategyManager::continueZealotRush);

	}


}



