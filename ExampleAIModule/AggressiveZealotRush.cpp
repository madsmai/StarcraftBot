#include "AggressiveZealotRush.h"

AggressiveZealotRush::AggressiveZealotRush() {

	Broodwar << name << std::endl;

	setArmySize();

	buildOrder = { // base Build order
		probe, gateway, gateway, //2 gateways on 9
		scoutRequest,
		probe, probe, zealot, pylon, // 13/16-24
		zealot, zealot, zealot, pylon, // 19/24-32
		zealot, zealot, zealot, zealot, pylon, // 27/32-40
		zealot, zealot, zealot, zealot }; // 35

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

	StrategyManager::getInstance().setOngoingStrategy(true);

}



