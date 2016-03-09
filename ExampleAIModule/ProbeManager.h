#pragma once

#include <BWAPI.h>

class ProbeManager
{
public:
	ProbeManager();
	~ProbeManager();
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
};

