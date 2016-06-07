#include "BaseBuild.h"


BaseBuild::BaseBuild() {

	Broodwar << name << std::endl;

	setSquadSize(999);

	buildOrder = { probe, probe, probe, probe, pylon};
	// ending with 8 / 16 supply

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}


