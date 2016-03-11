#pragma once
#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>
#include <windows.h>

// Remember not to use "Broodwar" in any global class constructor!

DWORD WINAPI AnalyzeThread();

class ExampleAIModule : public BWAPI::AIModule
{
public:

	// Basic game variables
	int refineryWorkers;
	int mineralsReserved;
	bool scouting;
	BWAPI::Unit scout;
	std::vector<int> pendingBuildings;
	std::vector<BWAPI::Unit> enemyUnits;
	std::vector<BWAPI::Unit> ourZealots;

	// BWTA variables
	BWTA::BaseLocation* scoutedBase;
	BWTA::BaseLocation* enemyBase;
	BWTA::BaseLocation* expansion;
	BWTA::BaseLocation* ourBase;
	std::set<BWTA::BaseLocation*> baseLocations;

	// Upgrades variables
	int ground_weapons_count;
	int ground_armor_count;
	int air_weapons_count;
	int air_armor_count;
	int plasma_shields_count;
	bool singularity_Charge_Researched;

	// tactic variables
	bool zealot_rush;


	//zealot_rush variables
	int zealotMAX;


	// Build functions
	void callBuildFunctions(BWAPI::Unit worker);
	void buildSupply(BWAPI::Unit worker);
	void buildRefinery(BWAPI::Unit worker);
	void buildForge(BWAPI::Unit worker);
	void buildGateway(BWAPI::Unit worker);
	void buildCybernetics_Core(BWAPI::Unit worker);
	void buildCitadel_Of_Adun(BWAPI::Unit worker);
	void buildPhoton_Cannon(BWAPI::Unit worker);
	void constructBuilding(BWAPI::UnitType buildingType, BWAPI::Unit worker);


	// Auxiliary functions
	void releaseFromList(BWAPI::Unit unit, std::vector<BWAPI::Unit> list);
	void releaseMinerals(BWAPI::Unit unit); // removes minerals from the reserved list


	// Basic functions
	void goScout(BWAPI::Unit scout);
	void trainZealots(BWAPI::Unit gateway);
	bool isGasWorker(BWAPI::Unit worker);
	void researchForge(BWAPI::Unit forge);
	void researchCybernetics_Core(BWAPI::Unit forge);


	// BWTA functions
	void drawTerrainData();







	// Virtual functions for callbacks, leave these as they are.
	virtual void onStart();
	virtual void onEnd(bool isWinner);
	virtual void onFrame();
	virtual void onSendText(std::string text);
	virtual void onReceiveText(BWAPI::Player player, std::string text);
	virtual void onPlayerLeft(BWAPI::Player player);
	virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	virtual void onUnitEvade(BWAPI::Unit unit);
	virtual void onUnitShow(BWAPI::Unit unit);
	virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	virtual void onUnitRenegade(BWAPI::Unit unit);
	virtual void onSaveGame(std::string gameName);
	virtual void onUnitComplete(BWAPI::Unit unit);
	// Everything below this line is safe to modify.



};
