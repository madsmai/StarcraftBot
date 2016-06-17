#include "ResourceManager.h"

// Author of class: Kasper

using namespace BWAPI;
//Currently only possible to reserve/release minerals for buildings
//Reserve mins
void ResourceManager::reserveMinerals(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedMinerals = reservedMinerals + type.mineralPrice();
		int  min = getReservedMinerals();
	}
}

//Reserve gas
void ResourceManager::reserveGas(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedGas = reservedGas + type.gasPrice();
		int  gas = getReservedGas();
	}
}

ResourceManager& ResourceManager::getInstance(){ //Return ref to ResourceManager object
	static ResourceManager i; //Make static instance i
	return i;
}

//Release the gas and minerals that are reserved
void ResourceManager::onUnitCreate(Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == Broodwar->self()){

		reservedMinerals = reservedMinerals - unit->getType().mineralPrice();
		reservedGas = reservedGas - unit->getType().gasPrice();
	}
}

//Release the gas and minerals that are reserved
void ResourceManager::onUnitMorph(Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == Broodwar->self()){

		reservedMinerals = reservedMinerals - unit->getType().mineralPrice();
		reservedGas = reservedGas - unit->getType().gasPrice();
	}
}