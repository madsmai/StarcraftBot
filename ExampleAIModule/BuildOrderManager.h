#pragma once

#include <BWAPI.h>
#include "ProbeManager.h"
#include "BuildingManager.h"
#include "OffenseManager.h"
#include <queue>

class BuildOrderManager
{
public:
	void onFrame();
	void onStart();

	static BuildOrderManager& getInstance();
	void supplyInQueueExecuted(int amount){ supplyInQueue -= amount / 2; }
	void pylonInQueueExecuted() { pylonsInQueue--; }

	bool &getFixedOrder() { return fixedOrder; }
	std::vector<BWAPI::UnitType>  &getFixedOrderQueue() { return fixedOrderQueue; }

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
	void makeScout();

	bool fixedOrder = false;
	int supplyInQueue;
	int pylonsInQueue;

	std::vector<BWAPI::UnitType> fixedOrderQueue;
};

