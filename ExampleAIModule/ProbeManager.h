#pragma once

#include <BWAPI.h>
#include "ResourceManager.h"
#include "ScoutManager.h"
#include "BuildOrderManager.h"
#include "StrategyManager.h"
#include "PlacementManager.h"
#include "InformationManager.h"
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
	int runFrames = 0;
	int tagged = 0;
	Unitset AttackingProbes;

	// auxialliary functions
	void executeQueue();
	void nonIdle();
	bool checkAndAddSupply();
	void probeFightBack(Unit);

	Unit builder;
	bool builderMoving = false;
	TilePosition buildingPosition;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};

};

