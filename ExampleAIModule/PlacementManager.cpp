#include "PlacementManager.h"

using namespace BWAPI;

void PlacementManager::onFrame(){
	drawReservedSpace();
}

void PlacementManager::onUnitDestroy(BWAPI::Unit unit){
	UnitType type = unit->getType();
	if (!(type.isBuilding() && unit->getPlayer() == Broodwar->self())){
		return;
	}
	TilePosition pos = TilePosition(unit->getPosition());
	releaseSpace(type, pos);
}

void PlacementManager::reserveSpace(UnitType type, TilePosition pos){
	//Add all the points that are in the area to the reservedPlaces table
	for (int x = pos.x; x < (type.tileWidth() + pos.x) && x < mapWidth ; x++){
		for (int y = pos.y; y < (type.tileHeight() + pos.y) && y < mapHeight; y++){
			reservedPlaces[x][y] = true;
		}
	}
}

void PlacementManager::releaseSpace(UnitType type, TilePosition pos){
	//Remove all the points that are in the area to the reservedPlaces table
	for (int x = pos.x; x < (type.tileWidth() + pos.x) && x < mapWidth; x++){
		for (int y = pos.y; y < (type.tileHeight() + pos.y) && y < mapHeight; y++){
			reservedPlaces[x][y] = false;
		}
	}
}

void PlacementManager::drawReservedSpace(){
	for (int x = 0; x < mapWidth; x++){
		for (int y = 0; y < mapHeight; y++){
			if (reservedPlaces[x][y]){

				int x1 = x * 32;
				int y1 = y * 32;
				int x2 = (x + 1) * 32;
				int y2 = (y + 1) * 32;

				Broodwar->drawBoxMap(x1, y1, x2, y2, Colors::Teal, false);
			}
		}
	}
}

//Check if there is space for a unit on a tile
bool PlacementManager::isReserved(UnitType type, TilePosition pos){
	for (int x = pos.x; x < (type.tileWidth() + pos.x) && x < mapWidth; x++){
		for (int y = pos.y; y < (type.tileHeight() + pos.y) && y < mapHeight; y++){
			if (reservedPlaces[x][y]){
				return true;
			}
		}
	}
	return false;
}

TilePosition PlacementManager::getBuildingPlacement(UnitType type, TilePosition pos){

	TilePosition newPos = Broodwar->getBuildLocation(type, pos);

	reserveSpace(type, newPos);
	return newPos;
}

//Get a static instance of class
PlacementManager& PlacementManager::getInstance(){ //Return ref to probemanager object
	static PlacementManager i; //Make static instance i
	return i;
}