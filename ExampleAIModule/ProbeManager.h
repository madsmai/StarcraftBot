#pragma once

#include <BWAPI.h>
#include "ResourceManager.h"
#include "ScoutManager.h"
#include "BuildOrderManager.h"
#include <vector>
#include <queue>

class ProbeManager
{
public:

	//BWAPI functions
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	//void onStart();

	//Function for getting an instance
	static ProbeManager& getInstance();

	//Add scoutRequest
	void addScoutRequest();

	//Add gas worker
	void addGasWorkerRequest();

	//Add building to list
	//void addBuilding(BWAPI::UnitType);

private:
	//The lists
	std::vector<BWAPI::Unit> mineralProbes;
	std::vector<BWAPI::Unit> gasProbes;
	//std::queue<BWAPI::UnitType> pendingBuildings;

	//Amount of scoutRequests
	int scoutRequests = 0;
	int gasWorkerRequests = 0;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};
	~ProbeManager(){};
};

