#include "PlacementManager.h"

using namespace BWAPI;

// Author of class: Mads Engberg

void PlacementManager::onFrame(){
	drawReservedSpace();
}

void PlacementManager::onUnitDestroy(BWAPI::Unit unit){
	UnitType type = unit->getType();
	if (!(type.isBuilding() && unit->getPlayer() == Broodwar->self())){
		return;
	}
	TilePosition pos = unit->getTilePosition();
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
	for (int x = pos.x; x < (pos.x + type.tileWidth()) && x < mapWidth; x++){
		for (int y = pos.y; y < (pos.y + type.tileHeight()) && y < mapHeight; y++){
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

int PlacementManager::findNearest(TilePosition position, UnitType anchor, UnitType finding){
	Unitset buildings = Broodwar->self()->getUnits();
	std::vector<TilePosition> anchorPoints;
	int minDist = INT_MAX;

	//find the 4 corners of the anchor building
	anchorPoints.push_back(position);
	anchorPoints.push_back(TilePosition(position.x + anchor.tileWidth(), position.y));
	anchorPoints.push_back(TilePosition(position.x, position.y + anchor.tileHeight()));
	anchorPoints.push_back(TilePosition(position.x + anchor.tileWidth(), position.y + anchor.tileHeight()));

	//Loop through all our buildings
	for (Unit unit : buildings){
		UnitType type = unit->getType();
		if (type.isBuilding() && type == finding){
			TilePosition building = unit->getTilePosition();
			std::vector<TilePosition> buildingPoints;

			//find the 4 corners of the other building
			buildingPoints.push_back(building);
			buildingPoints.push_back(TilePosition(building.x + type.tileWidth(), building.y));
			buildingPoints.push_back(TilePosition(building.x, building.y + type.tileHeight()));
			buildingPoints.push_back(TilePosition(building.x + type.tileWidth(), building.y + type.tileHeight()));

			//Check corners against each other
			for (unsigned int j = 0; j < anchorPoints.size(); j++){
				for (unsigned int k = 0; k < buildingPoints.size(); k++){
					//If the distance between two corners are smaller than current smallest we use that position
					if (minDist > anchorPoints[j].getApproxDistance(buildingPoints[k])){
						minDist = anchorPoints[j].getApproxDistance(buildingPoints[k]);
					}
				}
			}

			//No reason to check anymore
			if (minDist == 0){
				break;
			}

			//DEBUGGING//
			Broodwar->registerEvent([building, type](Game*)
			{Broodwar->drawBoxMap(Position(building), Position(TilePosition(building.x + type.tileWidth(), building.y + type.tileHeight())), Colors::Yellow, false); }
			, nullptr, type.buildTime() + 10);
		}
	}

	return minDist;
}

int PlacementManager::findNearest(TilePosition position, UnitType anchor){
	Unitset buildings = Broodwar->self()->getUnits();
	std::vector<TilePosition> anchorPoints;
	int minDist = INT_MAX;

	//find the 4 corners of the anchor building
	anchorPoints.push_back(position);
	anchorPoints.push_back(TilePosition(position.x + anchor.tileWidth(), position.y));
	anchorPoints.push_back(TilePosition(position.x, position.y + anchor.tileHeight()));
	anchorPoints.push_back(TilePosition(position.x + anchor.tileWidth(), position.y + anchor.tileHeight()));

	//Loop through all our buildings
	for (Unit unit : buildings){
		UnitType type = unit->getType();
		if (type.isBuilding()){
			TilePosition building = unit->getTilePosition();
			std::vector<TilePosition> buildingPoints;

			//find the 4 corners of the other building
			buildingPoints.push_back(building);
			buildingPoints.push_back(TilePosition(building.x + type.tileWidth(), building.y));
			buildingPoints.push_back(TilePosition(building.x, building.y + type.tileHeight()));
			buildingPoints.push_back(TilePosition(building.x + type.tileWidth(), building.y + type.tileHeight()));

			//Check corners against each other
			for (unsigned int j = 0; j < anchorPoints.size(); j++){
				for (unsigned int k = 0; k < buildingPoints.size(); k++){
					//If the distance between two corners are smaller than current smallest we use that position
					if (minDist > anchorPoints[j].getApproxDistance(buildingPoints[k])){
						minDist = anchorPoints[j].getApproxDistance(buildingPoints[k]);
					}
				}
			}

			//No reason to check anymore
			if (minDist == 0){
				break;
			}

			//DEBUGGING//
			Broodwar->registerEvent([building, type](Game*)
			{Broodwar->drawBoxMap(Position(building), Position(TilePosition(building.x + type.tileWidth(), building.y + type.tileHeight())), Colors::Yellow, false); }
			, nullptr, type.buildTime() + 10);
		}
	}

	return minDist;
}

TilePosition PlacementManager::pointBetween(TilePosition pos1, TilePosition pos2, int towardsTile2){
	int x = pos1.x + (int) ((pos2.x - pos1.x)*(1-(1.0/towardsTile2)));
	int y = pos1.y + (int) ((pos2.y - pos1.y)*(1-(1.0/towardsTile2)));

	return TilePosition(x, y);
}

TilePosition PlacementManager::getBuildingPlacement(UnitType type, TilePosition pos){
	//If it's an assimilator then place it where BWAPI would
	if (type == UnitTypes::Protoss_Assimilator){
		return Broodwar->getBuildLocation(type, pos);
	}

	//Find the position of the nearest chokepoint and our base
	TilePosition ourBase = Broodwar->self()->getStartLocation();
	TilePosition chokePoint = TilePosition(BWTA::getNearestChokepoint(ourBase)->getCenter());

	//Find the tilepositions we want to build at (1/2 and 4/5 of the way between)
	TilePosition midWay = pointBetween(ourBase, chokePoint, 2);
	TilePosition cannonPos = pointBetween(ourBase, chokePoint, 5);
	
	//DEBUGGING
	Broodwar->registerEvent([midWay, type](Game*)
	{Broodwar->drawTextMap(Position(TilePosition(midWay.x, midWay.y)), "MidWayPoint"); 
	Broodwar->drawCircleMap(Position(TilePosition(midWay.x, midWay.y)), 16, Colors::Yellow, false); }
	, nullptr, type.buildTime() + 1000);
	
	Broodwar->registerEvent([cannonPos, type](Game*)
	{Broodwar->drawTextMap(Position(TilePosition(cannonPos.x, cannonPos.y)), "9/10 Point");
	Broodwar->drawCircleMap(Position(TilePosition(cannonPos.x, cannonPos.y)), 16, Colors::Yellow, false); }
	, nullptr, type.buildTime() + 1000);
	
	
	//Make a queue of possible places to put a building, push midWay onto it
	//If it's a cannon we want to place it elsewhere
	std::queue<TilePosition> tileQueue;
	if (type == UnitTypes::Protoss_Photon_Cannon){
		tileQueue.push(cannonPos); //(4/5)
	}
	else{
		tileQueue.push(midWay);
	}

	//A set of all tiles we've visited thus far
	std::set<TilePosition> checkedTiles;

	//Pointer to current BWTA region
	BWTA::Region* baseRegion = BWTA::getRegion(ourBase);
	BWTA::Region* initRegion = BWTA::getRegion(tileQueue.front());

	while (!tileQueue.empty()){
		//Take the next position in queue, add it to checked positions, pop it
		TilePosition returnPos = tileQueue.front();
		checkedTiles.insert(returnPos);
		tileQueue.pop();

		//DEBUGGING//
		//Broodwar->registerEvent([returnPos, type](Game*)
		//{Broodwar->drawBoxMap(Position(TilePosition(returnPos.x, returnPos.y)), 
		//	Position(TilePosition(returnPos.x+1, returnPos.y+1)), Colors::Red, false); }
		//, nullptr, type.buildTime());

		//If the position is not reserved, is valid and in the right region, and the region doesn't have any units on it
		if (!isReserved(type, returnPos)
			&& Broodwar->canBuildHere(returnPos, type) 
			&& returnPos.isValid()
			&& (BWTA::getRegion(returnPos) == baseRegion)
			&& Broodwar->getUnitsInRectangle(Position(returnPos),
				Position(TilePosition(returnPos.x + type.tileWidth(), returnPos.y + type.tileHeight()))).empty()){

			//For pylons and photon cannons there are more checks
			if (type == UnitTypes::Protoss_Pylon){

				if (findNearest(returnPos, type, type) > 3){ //Space between pylons
					return returnPos;
				}
			}
			else if (type == UnitTypes::Protoss_Photon_Cannon){
				if (returnPos.getApproxDistance(chokePoint) < chokePoint.getApproxDistance(ourBase)
					&& findNearest(returnPos, type) < 6 //Maybe change to 7 (thier range)
					&& findNearest(returnPos, type, type) > 1 //Space between cannons
					&& findNearest(returnPos, type, UnitTypes::Protoss_Pylon) < 5){ //Maybe also change to 7){
					return returnPos;
				}
			}
			else if (type == UnitTypes::Protoss_Gateway){
				if (findNearest(returnPos, type) > 1
					&& findNearest(returnPos, type, UnitTypes::Protoss_Pylon) < 7){
					return returnPos;
				}
			}
			else if (findNearest(returnPos, type, UnitTypes::Protoss_Pylon) < 7){
				return returnPos;
			}
		}
		//Otherwise add all the nearby positions if we haven't already checked them (or are about to)
		TilePosition up = TilePosition(returnPos.x, returnPos.y - 1);
		TilePosition down = TilePosition(returnPos.x, returnPos.y + 1);
		TilePosition left = TilePosition(returnPos.x - 1, returnPos.y);
		TilePosition right = TilePosition(returnPos.x + 1, returnPos.y);

		//Add to queue if position is valid and in right region
		if (checkedTiles.find(up) == checkedTiles.end() && up.isValid() 
			&& (BWTA::getRegion(up) == baseRegion || BWTA::getRegion(up) == initRegion)){
			tileQueue.push(up);
			checkedTiles.insert(up);
		}
		if (checkedTiles.find(down) == checkedTiles.end() && up.isValid() 
			&& (BWTA::getRegion(down) == baseRegion || BWTA::getRegion(down) == initRegion)){
			tileQueue.push(down);
			checkedTiles.insert(down);
		}
		if (checkedTiles.find(left) == checkedTiles.end() && up.isValid() 
			&& (BWTA::getRegion(left) == baseRegion || BWTA::getRegion(left) == initRegion)){
			tileQueue.push(left);
			checkedTiles.insert(left);
		}
		if (checkedTiles.find(right) == checkedTiles.end() && up.isValid() 
			&& (BWTA::getRegion(right) == baseRegion || BWTA::getRegion(right) == initRegion)){
			tileQueue.push(right);
			checkedTiles.insert(right);
		}
	}
	

	TilePosition backupPos = Broodwar->getBuildLocation(type, pos);
	//DEBUGGING//
	Broodwar->registerEvent([backupPos, type, midWay](Game*)
	{Broodwar->drawBoxMap(Position(TilePosition(backupPos.x, backupPos.y)),
	Position(TilePosition(backupPos.x + type.tileWidth(), backupPos.y + type.tileHeight())), Colors::Purple, false); 
	Broodwar->drawTextMap(Position(backupPos), "BWAPI made"); }
	, nullptr, type.buildTime());

	//It fails if it gets to this line so we just revert to 
	return backupPos;
}

//Get a static instance of class
PlacementManager& PlacementManager::getInstance(){ //Return ref to probemanager object
	static PlacementManager i; //Make static instance i
	return i;
}