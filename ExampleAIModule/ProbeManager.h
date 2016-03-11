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
	bool becomeScout(BWAPI::Unit);

private:
	//The lists
	std::vector<BWAPI::Unit> probes;
	std::queue<BWAPI::Unit> pendingBuildings;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};
	~ProbeManager(){};
};

