#pragma once

#include <BWAPI.h>
#include "ResourceManager.h"
#include "BuildOrderManager.h"
#include <vector>
#include <queue>

class BuildingManager
{
public:

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);

	static BuildingManager& getInstance();

	//void addUnit(BWAPI::UnitType);
	void addUpgrade(BWAPI::UpgradeType);

private:
	//The lists
	std::vector<BWAPI::Unit> buildings;
	//std::queue<BWAPI::UnitType> pendingUnits;
	std::queue<BWAPI::UpgradeType> pendingUpgrades;

	BuildingManager() {};
	~BuildingManager() {};
};

