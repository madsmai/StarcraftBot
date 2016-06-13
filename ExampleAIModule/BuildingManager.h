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

	//Getter
	std::vector<BWAPI::Unit> getBuildings(){ return buildings; }

private:
	//The lists
	std::vector<BWAPI::Unit> buildings;

	BuildingManager() {};
	~BuildingManager() {};
};

