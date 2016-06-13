#include "BuildOrderType.h"


BuildOrderType::BuildOrderType(int request){
	type = types::MetaRequestType;

	switch (request){

	case BuildOrderType::scoutRequest:
		requestType = requests::scoutRequest;
		break;

	case BuildOrderType::gasworkerRequest:
		requestType = requests::gasworkerRequest;
		break;

	case BuildOrderType::evaluateStrategyRequest:
		requestType = requests::evaluateStrategyRequest;
		break;

	default:
		BWAPI::Broodwar << "Error. No such request";
		break;

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

BuildOrderType::BuildOrderType(BWAPI::TechType tech){
	type = types::MetaTechType;
	techType = tech;
}

std::string BuildOrderType::getName(){
	if (type == types::MetaUnitType){
		return getUnitType().getName();
	}
	else if (type == types::MetaUpgradeType){
		return getUpgradeType().getName();
	}
	else if (type == types::MetaTechType){
		return getTechType().getName();
	}
	else if (type == types::MetaRequestType){
		if (getRequestType() == scoutRequest){
			return "scoutRequest";
		}
		else if (getRequestType() == gasworkerRequest){
			return "gasworkerRequest";
		}
		else if (getRequestType() == evaluateStrategyRequest){
			return "evaluateStrategyRequest";
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

bool BuildOrderType::isTech(){
	return type == types::MetaTechType;
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

BWAPI::TechType BuildOrderType::getTechType(){
	return techType;
}