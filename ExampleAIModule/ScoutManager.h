#pragma once

#include <BWAPI.h>

class ScoutManager
{
public:
	ScoutManager();
	~ScoutManager();
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);
};

