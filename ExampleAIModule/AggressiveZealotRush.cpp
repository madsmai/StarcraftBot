#include "AggressiveZealotRush.h"

AggressiveZealotRush::AggressiveZealotRush() {

	Broodwar << name << std::endl;

	setArmySize();

	buildOrder = { probe, gateway, gateway, //2 gateways on 9
		scoutRequest,
		probe, probe, zealot, pylon, // 13
		zealot, zealot, zealot, pylon,
		zealot, zealot, zealot, pylon };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

	StrategyManager::getInstance().setOngoingStrategy(true);

}



