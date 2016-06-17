#pragma once

#include <BWAPI.h>
#include "InformationManager.h"

class OffenseManager {
public:

	bool debugging = false;

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
	int getSquadSize(){ return squadSize; }

private:
	//Zealots
	unsigned int squadSize;

	//Upgrades

	bool rush(BWAPI::Unitset);
	bool fightBack(BWAPI::Unit);
	bool getHelp(BWAPI::Unit, BWAPI::Unit);
	void searchAndDestroy(BWAPI::Unit);
	int calculatePriority(BWAPI::Unit, BWAPI::Unit);
	bool isFighter(BWAPI::Unit);
	void fillReaverOrCarrier(BWAPI::Unit);
	bool checkedMainBase = false;


	//Based on the method of the same name by Dave Churchils in his UAlbertoBot
	void smartAttackUnit(BWAPI::Unit, BWAPI::Unit);

	void explore(BWAPI::Unit);


	BWAPI::Unitset getEnemiesInOurRegion();
	void defendOurBase();


	//Rename me
	void FixWrongPriority(BWAPI::Unit);

	//Taken from scoutManager because we can't include more to this class
	void goScout(BWAPI::Unit);

	OffenseManager(){};
	~OffenseManager(){};
};

