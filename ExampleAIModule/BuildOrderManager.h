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
	void buildGateway();
	void buildSupply();
	void buildForge();
	void buildRefinery();
	void buildCitadelOfAdun();
	void buildCyberneticsCore();
	void buildPhotonCannon();
	void researchForge();
	void researchCyberneticsCore();
	void trainZealot();
	void trainProbe();
	void makeScout();
};

