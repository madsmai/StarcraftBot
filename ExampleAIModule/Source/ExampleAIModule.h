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

	bool refinery;
	bool refineryFinished;
	int refineryWorkers;
	int workers;
	int mineralsReserved;

	bool scouting;
	BWAPI::Unit scout;

	std::set<BWTA::BaseLocation*> baseLocations;
	BWTA::BaseLocation* scoutedBase;
	BWTA::BaseLocation* enemyBase;
	BWTA::BaseLocation* expansion;
	BWTA::BaseLocation* ourBase;

	void releaseMinerals(BWAPI::Unit unit);
	void constructBuilding(BWAPI::UnitType buildingType, BWAPI::Unit worker);
	void supplyCheckAndBuild(BWAPI::Unit worker);
	void goScout(BWAPI::Unit scout);

	void drawTerrainData();

	std::vector<int> pendingBuildings;
	std::vector<BWAPI::Unit> enemyUnits;

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
