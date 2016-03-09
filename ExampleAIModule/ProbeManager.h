#pragma once

#include <BWAPI.h>
#include <vector>
#include <queue>

class ProbeManager
{
public:

	//BWAPI functions
	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onStart();

	//Managing the lists
	void addProbe(BWAPI::Unit);
	void removeProbe(BWAPI::Unit);
	void addPendingBuilding(BWAPI::Unit);
	void removePendingBuilding(BWAPI::Unit);

	//Function for getting an instance
	static ProbeManager& getInstance();

	//Helper methods
	bool becomeScout(BWAPI::Unit);
	bool constructBuilding(BWAPI::Unit, BWAPI::UnitType);

	//Getters and setters
	std::vector<BWAPI::Unit> getProbes();
	void setProbes(std::vector<BWAPI::Unit>);
	std::queue<BWAPI::Unit> getPendingBuildings();
	void setPendingBuildings(std::queue<BWAPI::Unit>);

private:
	//The lists
	std::vector<BWAPI::Unit> probes;
	std::queue<BWAPI::Unit> pendingBuildings;

	//Con- and destructor aren't needed since class is used as static
	ProbeManager(){};
	~ProbeManager(){};
};

