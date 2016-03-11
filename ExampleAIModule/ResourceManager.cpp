#include "ResourceManager.h"

/*
TODO:
	
*/

//Currently only possible to reserve/release minerals for buildings
//Reserve
void ResourceManager::reserveMinerals(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedMinerals = reservedMinerals + type.mineralPrice();
	}
}

ResourceManager& ResourceManager::getInstance(){ //Return ref to ResourceManager object
	static ResourceManager i; //Make static instance i
	return i;
}

void ResourceManager::onUnitCreated(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		reservedMinerals = reservedMinerals - unit->getType().mineralPrice();
	}
}