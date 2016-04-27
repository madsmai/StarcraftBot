#pragma once

#include <BWAPI.h>

class OffenseManager {
public:

	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onFrame();

	static OffenseManager& getInstance();

	int &getZealotMax(){ return zealotMax; }

	int &getGroundWeapons(){ return ground_weapons_count; }
	int &getGroundArmor(){ return ground_armor_count; }
	int &getPlasmaShields(){ return plasma_shields_count; }
	int &getAirWeapons(){ return air_weapons_count; }
	int &getAirArmor(){ return air_armor_count; }
	bool &getSingularityCharge(){ return singularity_charge; }


	int runFrames;

	BWAPI::Unitset fighters;
	BWAPI::Unitset cowards;

	bool rushOngoing = false;

	void setArmySize(int size){ armySize = size; }

private:
	//Zealots
	int zealotMax = 6;
	unsigned int armySize;

	std::vector<BWAPI::Unit> zealots;

	BWAPI::Unitset squad;

	//Upgrades
	int ground_weapons_count = 0;
	int ground_armor_count = 0;
	int plasma_shields_count = 0;
	int air_weapons_count = 0;
	int air_armor_count = 0;
	bool singularity_charge = false;

	bool rush(BWAPI::Unitset);
	bool fightBack(BWAPI::Unit);
	bool getHelp(BWAPI::Unit, BWAPI::Unit);
	void searchAndDestroy(BWAPI::Unit);
	bool avoidTowers(BWAPI::Unit);
	int calculatePriority(BWAPI::Unit, BWAPI::Unit);

	OffenseManager(){};
	~OffenseManager(){};
};

