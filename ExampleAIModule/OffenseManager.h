#pragma once

#include <BWAPI.h>

class OffenseManager
{
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

private:
	//Zealots
	int zealotMax = 6;
	int armySize = 3;
	std::vector<BWAPI::Unit> zealots;
	BWAPI::Unitset fighters;

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
	bool searchAndDestroy(BWAPI::Unitset);

	OffenseManager() {};
	~OffenseManager() {};
};
