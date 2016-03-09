#pragma once

#include <BWAPI.h>
#include <vector>

class ProbeManager
{
public:

	void onFrame();
	void onUnitDestroy(BWAPI::Unit unit);
	void onUnitComplete(BWAPI::Unit unit);
	void onStart();

	void addProbe(BWAPI::Unit);
	void removeProbe(BWAPI::Unit);
	void addPendingBuilding(BWAPI::Unit);
	void removePendingBuilding(BWAPI::Unit);

	static ProbeManager& getInstance();

	//Getters and setters
	std::vector<BWAPI::Unit> getProbes();
	void setProbes(std::vector<BWAPI::Unit>);
	std::vector<BWAPI::Unit> getPendingBuildings();
	void setPendingBuildings(std::vector<BWAPI::Unit>);

private:
	std::vector<BWAPI::Unit> probes;
	std::vector<BWAPI::Unit> pendingBuildings;

	ProbeManager(){};
	~ProbeManager(){};
};

