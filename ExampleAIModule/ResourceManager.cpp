#include "ResourceManager.h"

/*
TODO:

*/

//Currently only possible to reserve/release minerals for buildings
//Reserve mins
void ResourceManager::reserveMinerals(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedMinerals = reservedMinerals + type.mineralPrice();
		int  min = getReservedMinerals();
		BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	}
}

//Reserve gas
void ResourceManager::reserveGas(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedGas = reservedGas + type.gasPrice();
		int  gas = getReservedGas();
		BWAPI::Broodwar->sendText(std::to_string(gas).c_str());
	}
}

ResourceManager& ResourceManager::getInstance(){ //Return ref to ResourceManager object
	static ResourceManager i; //Make static instance i
	return i;
}

//Release the gas and minerals that are reserved
void ResourceManager::onUnitCreate(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		reservedMinerals = reservedMinerals - unit->getType().mineralPrice();
		reservedGas = reservedGas - unit->getType().gasPrice();
	}
}