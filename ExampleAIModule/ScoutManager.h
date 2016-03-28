#pragma once

#include <BWAPI.h>

class ScoutManager
{
public:

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);

	void makeScout(BWAPI::Unit unit);

	static ScoutManager& getInstance();
private:
	ScoutManager() {};
	~ScoutManager() {};
};

