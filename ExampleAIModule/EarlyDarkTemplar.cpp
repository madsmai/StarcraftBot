#include "EarlyDarkTemplar.h"


EarlyDarkTemplar::EarlyDarkTemplar() {

	Broodwar << name << std::endl;

	setArmySize();

	buildOrder = { // base Build order
		probe, probe, gateway, scoutRequest, // 10 - scout
		probe, probe, assimilator, // 12 - assimilator
		gasworkerRequest, gasworkerRequest,
		probe, zealot, probe, pylon, // 16 - second pylon
		probe, probe, cybercore, // 18 - cybernetics core
		probe, zealot, probe, pylon, // 22
		probe, dragoon, probe, citadel, // 26 - citadel
		probe, dragoon, gateway, pylon, archives, // 29 archives
		probe, zealot, zealot, probe, pylon, // 35
		dtemplar, probe, dtemplar, // 2 dark templars
		forge, pylon, cannon, cannon };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

	StrategyManager::getInstance().setOngoingStrategy(true);
}



