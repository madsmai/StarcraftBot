#pragma once

#include <BWAPI.h>

class BuildOrderType {
public:
	BuildOrderType(int request);
	BuildOrderType(BWAPI::UnitType unit);
	BuildOrderType(BWAPI::UpgradeType upgrade);
	BuildOrderType(BWAPI::TechType upgrade);

	enum requests { scoutRequest, gasworkerRequest, evaluateStrategyRequest };

	bool isUnit();
	bool isUpgrade();
	bool isTech();
	bool isRequest();

	std::string getName();

	int getRequestType();
	BWAPI::UnitType getUnitType();
	BWAPI::UpgradeType getUpgradeType();
	BWAPI::TechType getTechType();

private:
	enum types { MetaUnitType, MetaUpgradeType, MetaTechType, MetaRequestType };

	int type = -1;

	int requestType = -1;
	BWAPI::UnitType unitType;
	BWAPI::UpgradeType upgradeType;
	BWAPI::TechType techType;

};

