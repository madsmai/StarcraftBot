#pragma once

#include <BWAPI.h>

enum requests { scoutRequest, gasworkerRequest };

class BuildOrderType
{
public:
	BuildOrderType(int request);
	BuildOrderType(BWAPI::UnitType unit);
	BuildOrderType(BWAPI::UpgradeType upgrade);

	bool isUnit();
	bool isUpgrade();
	bool isRequest();

	std::string getName();

	int getRequestType();
	BWAPI::UnitType getUnitType();
	BWAPI::UpgradeType getUpgradeType();

private:
	enum types { MetaUnitType, MetaUpgradeType, MetaRequestType };

	int type;

	int requestType;
	BWAPI::UnitType unitType;
	BWAPI::UpgradeType upgradeType;

};

