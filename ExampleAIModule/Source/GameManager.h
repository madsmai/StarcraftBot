#pragma once
#include "../ProbeManager.h"
#include "../ResourceManager.h"
#include "../BuildingManager.h"
#include "../BuildOrderManager.h"
#include "../InformationManager.h"
#include "../OffenseManager.h"
#include "../ScoutManager.h"
#include "../PlacementManager.h"
#include <BWAPI.h>
#include <BWTA.h>
#include <Windows.h>
#include <fstream>

DWORD WINAPI AnalyzeThread();

class GameManager : public BWAPI::AIModule
{
public:

	bool debugging = false;

	virtual void onStart();
	virtual void onFrame();
	virtual void onUnitComplete(BWAPI::Unit unit);

	virtual void onEnd(bool isWinner);
	virtual void onSendText(std::string string);
	//virtual void onReceiveText(BWAPI::Player player, std::string text);
	//virtual void onPlayerLeft(BWAPI::Player player);
	//virtual void onNukeDetect(BWAPI::Position target);
	virtual void onUnitDiscover(BWAPI::Unit unit);
	//virtual void onUnitEvade(BWAPI::Unit unit);
	//virtual void onUnitShow(BWAPI::Unit unit);
	//virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	virtual void onUnitDestroy(BWAPI::Unit unit);
	virtual void onUnitMorph(BWAPI::Unit unit);
	//virtual void onUnitRenegade(BWAPI::Unit unit);
	//virtual void onSaveGame(std::string gameName);

	BWTA::BaseLocation* nextBase;
	BWTA::BaseLocation* enemyBase;
	BWTA::BaseLocation* expansion;
	BWTA::BaseLocation* ourBase;
	std::set<BWTA::BaseLocation*> baseLocations;

	void drawTerrainData();

};
