#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>
#include <Windows.h>
#include "InformationManager.h"


class ScoutManager
{
public:
	BWAPI::Unit scout;

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);
	void goScout(BWAPI::Unit scout);


	static ScoutManager& getInstance();

	void addScout(BWAPI::Unit scout);
	void removeScout(BWAPI::Unit Scout);
	std::vector<BWAPI::Unit> scouts;

private:
	ScoutManager() {};
	~ScoutManager() {};
};

