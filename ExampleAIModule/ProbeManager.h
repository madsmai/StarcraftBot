#pragma once

#include <BWAPI.h>
#include "ResourceManager.h"
#include <vector>
#include <queue>

class ProbeManager
{
public:

	//BWAPI functions
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onStart();

	//Function for getting an instance
	static ProbeManager& getInstance();

	//Helper methods
	bool becomeScout();

	//Add building to list
	void addBuilding(BWAPI::UnitType);

private:
	//The lists
	std::vector<BWAPI::Unit> probes;
	std::queue<BWAPI::UnitType> pendingBuildings;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};
	~ProbeManager(){};
};

