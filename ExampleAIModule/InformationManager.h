#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector
#include <BWTA.h>
#include "OffenseManager.h"

class InformationManager
{
public:
	static InformationManager& getInstance();

	void onUnitDiscover(BWAPI::Unit);
	void onUnitDestroy(BWAPI::Unit);

	void currentStatus();

	int calculateArmyStrength(BWAPI::Player);
	void addEnemyBarracks(BWAPI::Unit);
	void addEnemyAttackers(BWAPI::Unit);
	void addEnemyWorkers(BWAPI::Unit);
	void addEnemyTowers(BWAPI::Unit);
	void addEnemyPassiveBuildings(BWAPI::Unit);

	//Protoss
	void addDarkTemplar(BWAPI::Unit);

	void removeEnemyBarracks(BWAPI::Unit);
	void removeEnemyAttackers(BWAPI::Unit);
	void removeEnemyWorkers(BWAPI::Unit);
	void removeEnemyTowers(BWAPI::Unit);
	void removeEnemyPassiveBuildings(BWAPI::Unit);

	//Protoss
	void removeDarkTemplar(BWAPI::Unit);

	std::vector<BWAPI::Unit> enemyBarracks;
	std::vector<BWAPI::Unit> enemyAttackers;
	std::vector<BWAPI::Unit> enemyWorkers;
	std::vector<BWAPI::Unit> enemyTowers;
	std::vector<BWAPI::Unit> enemyPassiveBuildings;

	//Protoss
	bool citadelOfAdun = false;
	bool templarArchives = false;
	std::vector<BWAPI::Unit> darkTemplars;


	//BWTA variables
	BWTA::BaseLocation* nextBase;
	BWTA::BaseLocation* currentBase = BWTA::getStartLocation(BWAPI::Broodwar->self());
	BWTA::BaseLocation* enemyBase;
	BWTA::BaseLocation* expansion;
	BWTA::BaseLocation* ourBase = BWTA::getStartLocation(BWAPI::Broodwar->self());
	BWTA::BaseLocation* emptyMainBase;
	std::set<BWTA::BaseLocation*> baseLocations;

private:
	InformationManager(){};
	~InformationManager(){};
};

