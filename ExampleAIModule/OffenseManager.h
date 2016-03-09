#pragma once

#include <BWAPI.h>

class OffenseManager
{
public:
	OffenseManager();
	~OffenseManager();

	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);

	OffenseManager& OffenseManager::getInstance();
};

