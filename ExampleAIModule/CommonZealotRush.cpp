#include "CommonZealotRush.h"


CommonZealotRush::CommonZealotRush(){

	Broodwar << name << std::endl;

	setArmySize();

	buildOrder = {  // 8
		scoutRequest,
		probe, probe, gateway, // 10
		probe, probe, gateway, // 12
		probe, zealot, probe, pylon, // 16
		zealot, zealot, zealot, pylon, // 22
		zealot, zealot, zealot, pylon };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

	StrategyManager::getInstance().setOngoingStrategy(true);

}
