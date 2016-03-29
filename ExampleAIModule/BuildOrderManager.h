#pragma once

#include <BWAPI.h>
#include "ProbeManager.h"
#include "BuildingManager.h"
#include "OffenseManager.h"

class BuildOrderManager
{
public:
	void onFrame();

	static BuildOrderManager& getInstance();

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
	bool researchForge();
	bool researchCyberneticsCore();
	bool trainZealot();
	bool trainProbe();
	bool makeScout();
};

