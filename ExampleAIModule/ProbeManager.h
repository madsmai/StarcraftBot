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

	//Add probe to the list of mineralProbes
	void addMineralProbe(BWAPI::Unit probe);

private:
	//The lists
	std::vector<BWAPI::Unit> mineralProbes;
	std::vector<BWAPI::Unit> gasProbes;

	BWAPI::Unit builder;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};
	~ProbeManager(){};
};

