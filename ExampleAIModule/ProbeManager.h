#pragma once

#include <BWAPI.h>
#include "ResourceManager.h"
#include "ScoutManager.h"
#include "BuildOrderManager.h"
#include "StrategyManager.h"
#include <utility>
#include <vector>
#include <queue>

class ProbeManager {
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

	TilePosition getNewBuildLocation(BWAPI::UnitType type, TilePosition position);

private:
	//The lists
	std::vector<BWAPI::Unit> mineralProbes;
	std::vector<BWAPI::Unit> gasProbes;

	// auxialliary functions
	void executeQueue();
	void nonIdle();
	void moveCloserTo(TilePosition& moving, const TilePosition& stationary, int dist);

	Unit builder;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};

};

