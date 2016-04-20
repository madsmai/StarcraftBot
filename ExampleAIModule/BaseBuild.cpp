#include "BaseBuild.h"


BaseBuild::BaseBuild() {

	Broodwar << name << std::endl;

	setArmySize();

	buildOrder = { probe, probe, probe, probe, pylon };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

	StrategyManager::getInstance().setOngoingStrategy(true);
}
