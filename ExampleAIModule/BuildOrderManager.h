#pragma once

#include <BWAPI.h>
#include "BuildingManager.h"
#include "BuildOrderType.h"
#include "StrategyManager.h"
#include <queue>
#include "BaseBuild.h"


using namespace BWAPI;
class BuildOrderManager {
public:
	void onFrame();
	void onStart();

	static BuildOrderManager& getInstance();
	void supplyInQueueExecuted(int amount){ supplyInQueue -= amount / 2; }
	void pylonInQueueExecuted() { pylonsInQueue--; }

	bool &getFixedOrder() { return fixedOrder; }
	std::vector<BuildOrderType>  &getNewFixedOrderQueue() { return newFixedOrderQueue; }

private:
	BuildOrderManager() {};
	~BuildOrderManager() {};

	//Building functions
	bool buildGateway();
	bool buildSupply();
	bool buildForge();
	bool buildRefinery();
	bool buildCitadelOfAdun();
	bool buildCyberneticsCore();
	bool buildPhotonCannon();
	void researchForge();
	void researchCyberneticsCore();
	void trainZealot();
	void trainProbe();

	bool fixedOrder = true;
	int supplyInQueue;
	int pylonsInQueue;

	std::vector<BuildOrderType> newFixedOrderQueue;


};

