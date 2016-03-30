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

	bool &getFixedOrder() { return fixedOrder; }
	std::queue<BWAPI::UnitType>  &getFixedOrderQueue() { return fixedOrderQueue; }

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

	bool fixedOrder = true;

	std::queue<BWAPI::UnitType> fixedOrderQueue;
};

