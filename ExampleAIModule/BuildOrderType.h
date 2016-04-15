#pragma once

#include <BWAPI.h>

class BuildOrderType
{
public:
	BuildOrderType(int request);
	BuildOrderType(BWAPI::UnitType unit);
	BuildOrderType(BWAPI::UpgradeType upgrade);

	enum requests { scoutRequest, gasworkerRequest };

	bool isUnit();
	bool isUpgrade();
	bool isRequest();

	std::string getName();

	int getRequestType();
	BWAPI::UnitType getUnitType();
	BWAPI::UpgradeType getUpgradeType();

private:
	enum types { MetaUnitType, MetaUpgradeType, MetaRequestType };

	int type = -1;

	int requestType = -1;
	BWAPI::UnitType unitType;
	BWAPI::UpgradeType upgradeType;

};

