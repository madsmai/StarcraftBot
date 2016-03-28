#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>

class ScoutManager
{
public:

	BWAPI::Unit scout;



	ScoutManager();
	~ScoutManager();

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitDiscover(BWAPI::Unit unit);
	void goScout(BWAPI::Unit scout);

	//BWTA variables
	BWTA::BaseLocation* scoutedBase;
	BWTA::BaseLocation* enemyBase;
	BWTA::BaseLocation* expansion;
	BWTA::BaseLocation* ourBase;
	std::set<BWTA::BaseLocation*> baseLocations;

	ScoutManager& ScoutManager::getInstance();
};

