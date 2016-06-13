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

TilePosition PlacementManager::findNearest(TilePosition position, UnitType type){
	std::vector<BWAPI::Unit> buildings = BuildingManager::getInstance().getBuildings();
	int minDist = INT_MAX;
	TilePosition returnPos;
	for (unsigned int i = 0; i < buildings.size(); i++){
		if (type == buildings.at(i)->getType()){
			if (minDist > position.getApproxDistance(buildings.at(i)->getTilePosition())){
				minDist = position.getApproxDistance(buildings.at(i)->getTilePosition());
				returnPos = buildings.at(i)->getTilePosition();
			}
		}
	}

	return returnPos;
}

TilePosition PlacementManager::findNearest(TilePosition position){
	std::vector<BWAPI::Unit> buildings = BuildingManager::getInstance().getBuildings();
	int minDist = INT_MAX;
	TilePosition returnPos;
	for (unsigned int i = 0; i < buildings.size(); i++){
		if (minDist > position.getApproxDistance(buildings.at(i)->getTilePosition())){
			minDist = position.getApproxDistance(buildings.at(i)->getTilePosition());
			returnPos = buildings.at(i)->getTilePosition();
		}
	}

	return returnPos;
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
	//Broodwar->registerEvent([midWay, type](Game*)
	//{Broodwar->drawTextMap(Position(TilePosition(midWay.x, midWay.y)), "MidWayPoint"); 
	//Broodwar->drawCircleMap(Position(TilePosition(midWay.x, midWay.y)), 16, Colors::Yellow, false); }
	//, nullptr, type.buildTime() + 1000);

	//Broodwar->registerEvent([cannonPos, type](Game*)
	//{Broodwar->drawTextMap(Position(TilePosition(cannonPos.x, cannonPos.y)), "9/10 Point");
	//Broodwar->drawCircleMap(Position(TilePosition(cannonPos.x, cannonPos.y)), 16, Colors::Yellow, false); }
	//, nullptr, type.buildTime() + 1000);

	/*TilePosition nearest = findNearest(pos, UnitTypes::Protoss_Pylon);
	Broodwar->registerEvent([nearest, type](Game*)
	{Broodwar->drawTextMap(Position(TilePosition(nearest.x, nearest.y)), "Closest pylon");
	Broodwar->drawCircleMap(Position(TilePosition(nearest.x, nearest.y)), 16, Colors::Yellow, false); }
	, nullptr, type.buildTime() + 1000);*/
	
	//Make a queue of possible places to put a building, push midWay onto it
	//If it's a cannon we want to place it elsewhere
	std::queue<TilePosition> tileQueue;
	if (type == UnitTypes::Protoss_Photon_Cannon){
		tileQueue.push(cannonPos); //(9/10)
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


		//If the position is not reserved, is valid and in the right region
		if (!isReserved(type, returnPos)
			&& Broodwar->canBuildHere(returnPos, type) 
			&& returnPos.isValid()
			&& (BWTA::getRegion(returnPos) == baseRegion)){

			//For pylons and photon cannons there are more checks
			if (type == UnitTypes::Protoss_Pylon 
				&& returnPos.getApproxDistance(findNearest(returnPos, UnitTypes::Protoss_Pylon)) > 5){
				return returnPos;
			}
			else if (type == UnitTypes::Protoss_Photon_Cannon 
				&& returnPos.getApproxDistance(chokePoint) < chokePoint.getApproxDistance(ourBase)
				&& returnPos.getApproxDistance(findNearest(returnPos)) < 5 //Maybe change 7 (thier range)
				&& returnPos.getApproxDistance(findNearest(returnPos, UnitTypes::Protoss_Photon_Cannon)) > 5
				&& returnPos.getApproxDistance(findNearest(returnPos, UnitTypes::Protoss_Pylon)) < 5){ //Maybe also change to 7
				return returnPos;
			}
			else if (type != UnitTypes::Protoss_Pylon && type != UnitTypes::Protoss_Photon_Cannon
				&& returnPos.getApproxDistance(findNearest(returnPos, UnitTypes::Protoss_Pylon)) < 7){
				return returnPos;
			}
		}
		//Otherwise add all the nearby positions if we haven't already checked them (or are about to)
		else{
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
	}
	

	TilePosition backupPos = Broodwar->getBuildLocation(type, pos);
	//DEBUGGING//
	Broodwar->registerEvent([backupPos, type](Game*)
	{Broodwar->drawBoxMap(Position(TilePosition(backupPos.x, backupPos.y)),
	Position(TilePosition(backupPos.x + type.tileWidth(), backupPos.y + type.tileHeight())), Colors::Purple, false); 
	Broodwar->drawTextMap(Position(TilePosition(backupPos.x, backupPos.y)), "BWAPI made"); }
	, nullptr, type.buildTime() + 1000);

	//It fails if it gets to this line so we just revert to 
	return backupPos;
}

//Get a static instance of class
PlacementManager& PlacementManager::getInstance(){ //Return ref to probemanager object
	static PlacementManager i; //Make static instance i
	return i;
}