#pragma once

#include <BWAPI.h>

enum types { UnitType, UpgradeType, RequestType };
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

	int getRequestType();
	BWAPI::UnitType getUnitType();
	BWAPI::UpgradeType getUpgradeType();

private:
	int type;

	int requestType;
	BWAPI::UnitType unitType;
	BWAPI::UpgradeType upgradeType;

};

