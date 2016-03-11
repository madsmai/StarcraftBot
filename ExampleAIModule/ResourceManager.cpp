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

//Release
void ResourceManager::releaseMinerals(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedMinerals = reservedMinerals - type.mineralPrice();
	}
}

ResourceManager& ResourceManager::getInstance(){ //Return ref to ResourceManager object
	static ResourceManager i; //Make static instance i
	return i;
}

void ResourceManager::onUnitComplete(BWAPI::Unit unit){
	releaseMinerals(unit->getType());
}