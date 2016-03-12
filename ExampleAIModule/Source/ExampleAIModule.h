#pragma once
#include <BWAPI.h>
#include "../ProbeManager.h"
#include "../ResourceManager.h"

class ExampleAIModule : public BWAPI::AIModule
{
public:
	virtual void onStart();
	virtual void onFrame();
	virtual void onUnitComplete(BWAPI::Unit);

	//virtual void onEnd(bool isWinner);
	virtual void onSendText(std::string);
	//virtual void onReceiveText(BWAPI::Player player, std::string text);
	//virtual void onPlayerLeft(BWAPI::Player player);
	//virtual void onNukeDetect(BWAPI::Position target);
	//virtual void onUnitDiscover(BWAPI::Unit unit);
	//virtual void onUnitEvade(BWAPI::Unit unit);
	//virtual void onUnitShow(BWAPI::Unit unit);
	//virtual void onUnitHide(BWAPI::Unit unit);
	virtual void onUnitCreate(BWAPI::Unit unit);
	//virtual void onUnitDestroy(BWAPI::Unit unit);
	//virtual void onUnitMorph(BWAPI::Unit unit);
	//virtual void onUnitRenegade(BWAPI::Unit unit);
	//virtual void onSaveGame(std::string gameName);
	//virtual void onUnitComplete(BWAPI::Unit unit);
};
