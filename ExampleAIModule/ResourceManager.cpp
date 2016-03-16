#include "ResourceManager.h"

//Currently only possible to reserve/release minerals for buildings
//Reserve
void ResourceManager::reserveMinerals(BWAPI::UnitType type){
	if (type.isBuilding()){
		reservedMinerals = reservedMinerals + type.mineralPrice();
		int  min = getReservedMinerals();
		BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	}
}

ResourceManager& ResourceManager::getInstance(){ //Return ref to ResourceManager object
	static ResourceManager i; //Make static instance i
	return i;
}

void ResourceManager::onUnitCreate(BWAPI::Unit unit){
	if (unit->getType().isBuilding() && unit->getPlayer() == BWAPI::Broodwar->self()){
		reservedMinerals = reservedMinerals - unit->getType().mineralPrice();
	}
}