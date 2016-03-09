#pragma once

#include <BWAPI.h>

class BuildingManager
{
public:
	BuildingManager();
	~BuildingManager();
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	BuildingManager& BuildingManager::getInstance();
};

