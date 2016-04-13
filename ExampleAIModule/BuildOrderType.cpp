#include "BuildOrderType.h"


BuildOrderType::BuildOrderType(int request){
	type = types::RequestType;
	if (request == 0){
		requestType = requests::scoutRequest;
	}
	else if (request == 1){
		requestType = requests::gasworkerRequest;
	}
	else {
		BWAPI::Broodwar << "No such request";
	}
}

BuildOrderType::BuildOrderType(BWAPI::UnitType unit){
	type = types::UnitType2;
	unitType = unit;
}

BuildOrderType::BuildOrderType(BWAPI::UpgradeType upgrade){
	type = types::UpgradeType;
	upgradeType = upgrade;
}

bool BuildOrderType::isUnit(){
	return type == types::UnitType2;
}

bool BuildOrderType::isUpgrade(){
	return type == types::UpgradeType;
}

bool BuildOrderType::isRequest(){
	return type == types::RequestType;
}

int BuildOrderType::getRequestType(){
	return requestType;
}

BWAPI::UnitType BuildOrderType::getUnitType(){
	return unitType;
}

BWAPI::UpgradeType BuildOrderType::getUpgradeType(){
	return upgradeType;
}