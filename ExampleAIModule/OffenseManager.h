#pragma once

#include <BWAPI.h>
#include "InformationManager.h"

class OffenseManager {
public:

	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onFrame();

	static OffenseManager& getInstance();


	int runFrames;

	BWAPI::Unitset fighters;
	BWAPI::Unit coward;
	BWAPI::Unitset squad;

	

	bool rushOngoing = false;

	void setSquadSize(int size){ squadSize = size; }

private:
	//Zealots
	unsigned int squadSize;

	//Upgrades

	bool rush(BWAPI::Unitset);
	bool fightBack(BWAPI::Unit);
	bool getHelp(BWAPI::Unit, BWAPI::Unit);
	void searchAndDestroy(BWAPI::Unit);
	bool avoidTowers(BWAPI::Unit);
	int calculatePriority(BWAPI::Unit, BWAPI::Unit);
	bool isFighter(BWAPI::Unit);
	void fillReaverOrCarrier(BWAPI::Unit);
	bool properClosestTarget(BWAPI::Unit, BWAPI::Unit);

	bool enemiesInOurRegion();
	void defendOurBase();


	OffenseManager(){};
	~OffenseManager(){};
};

