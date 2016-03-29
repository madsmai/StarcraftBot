#pragma once

#include <BWAPI.h>
#include <vector> // for std::vector

class InformationManager
{
public:
	static InformationManager& getInstance();
	InformationManager(){};
	~InformationManager(){};

	void onUnitDiscover(BWAPI::Unit);
	void onUnitDestroy(BWAPI::Unit);

	void currentStatus();

	void addEnemyBarracks(BWAPI::Unit);
	void addEnemyAttackers(BWAPI::Unit);
	void addEnemyWorkers(BWAPI::Unit);
	void addEnemyTowers(BWAPI::Unit);
	void addEnemyPassiveBuildings(BWAPI::Unit);

	void removeEnemyBarracks(BWAPI::Unit);
	void removeEnemyAttackers(BWAPI::Unit);
	void removeEnemyWorkers(BWAPI::Unit);
	void removeEnemyTowers(BWAPI::Unit);
	void removeEnemyPassiveBuildings(BWAPI::Unit);

	

	std::vector<BWAPI::Unit> enemyBarracks;
	std::vector<BWAPI::Unit> enemyAttackers;
	std::vector<BWAPI::Unit> enemyWorkers;
	std::vector<BWAPI::Unit> enemyTowers;
	std::vector<BWAPI::Unit> enemyPassiveBuildings;
};

