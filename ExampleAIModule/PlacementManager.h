#pragma once
#include <BWAPI.h>
#include <vector>
#include <queue>
#include <set>
#include <BWTA.h>
#include <assert.h>
#include <iostream>

using namespace BWAPI;

class PlacementManager
{
public:
	//Function for getting an instance
	static PlacementManager& getInstance();

	//Get a custom building placement for any building
	TilePosition getBuildingPlacement(UnitType type, TilePosition pos);

	//BWAPI functions
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);

	//Reserve
	void reserveSpace(UnitType type, TilePosition pos);

private:
	//unreserve space
	void releaseSpace(UnitType type, TilePosition pos);

	//Check if area is reserved
	bool isReserved(UnitType, TilePosition);

	//Helper functions
	int findNearest(TilePosition, UnitType, UnitType);
	int findNearest(TilePosition, UnitType);
	TilePosition pointBetween(TilePosition, TilePosition, int);

	//Map the size of the playing field, everything starts off being unreseverd
	std::vector<std::vector<bool>> reservedPlaces = std::vector<std::vector<bool>>
		(BWAPI::Broodwar->mapWidth(), std::vector<bool>(BWAPI::Broodwar->mapHeight(), false));

	//Usefull variables
	int mapWidth = reservedPlaces.size();
	int mapHeight = reservedPlaces[0].size();

	//Debugging functions
	void drawReservedSpace();

	//We dont need a constructor
	PlacementManager(){};

};

