#include "BuildOrderType.h"


BuildOrderType::BuildOrderType(int request){
	type = types::MetaRequestType;
	if (request == requests::scoutRequest){
		requestType = requests::scoutRequest;
	}
	else if (request == requests::gasworkerRequest){
		requestType = requests::gasworkerRequest;
	}
	else {
		BWAPI::Broodwar << "Error. No such request";
	}
}

BuildOrderType::BuildOrderType(BWAPI::UnitType unit){
	type = types::MetaUnitType;
	unitType = unit;
}

BuildOrderType::BuildOrderType(BWAPI::UpgradeType upgrade){
	type = types::MetaUpgradeType;
	upgradeType = upgrade;
}

std::string BuildOrderType::getName(){
	if (type == types::MetaUnitType){
		return getUnitType().getName();
	}
	else if (type == types::MetaUpgradeType){
		return getUpgradeType().getName();
	}
	else if (type == types::MetaRequestType){
		if (getRequestType() == 0){
			return "scoutRequest";
		}
		else if (getRequestType() == 1){
			return "gasworkerRequest";
		}
		else {
			return "Error. No such request type";
		}
	}
	else {
		return "Error. No name for type";
	}
}

bool BuildOrderType::isUnit(){
	return type == types::MetaUnitType;
}

bool BuildOrderType::isUpgrade(){
	return type == types::MetaUpgradeType;
}

bool BuildOrderType::isRequest(){
	return type == types::MetaRequestType;
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