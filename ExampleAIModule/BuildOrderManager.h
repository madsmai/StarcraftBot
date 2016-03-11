#pragma once

#include <BWAPI.h>

class BuildOrderManager
{
public:
	BuildOrderManager();
	~BuildOrderManager();
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	BuildOrderManager& BuildOrderManager::getInstance();
};

