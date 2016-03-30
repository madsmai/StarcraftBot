#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>
#include <Windows.h>
#include "InformationManager.h"


class ScoutManager
{
public:
	//BWAPI::Unit scout; This might not be needed any more

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);
	void goScout(BWAPI::Unit scout);

	static ScoutManager& getInstance();

	void addScout(BWAPI::Unit scout);
	void removeScout(BWAPI::Unit Scout);
	std::vector<BWAPI::Unit> const &getActiveScouts() { return activeScouts; }
	std::vector<BWAPI::Unit> const &getInactiveScouts() { return inactiveScouts; }

private:

	std::vector<BWAPI::Unit> activeScouts;
	std::vector<BWAPI::Unit> inactiveScouts;

	ScoutManager() {};
	~ScoutManager() {};
};

