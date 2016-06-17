#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>
#include <Windows.h>
#include "InformationManager.h"
#include "ProbeManager.h"


class ScoutManager
{
public:
	//BWAPI::Unit scout; This might not be needed any more

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);
	void goScout(BWAPI::Unit scout);

	bool retreating = false;
	bool scoutSent = false;
	bool doneScouting = false;

	void attackTheirWorkers(BWAPI::Unit unit);
	void keepAttackTheirWorkers(BWAPI::Unit unit);

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

