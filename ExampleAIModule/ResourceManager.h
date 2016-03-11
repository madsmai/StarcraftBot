#pragma once
#include <BWAPI.h>

class ResourceManager
{
public:

	//onStart and onUnitComplete
	void onStart(){ reservedMinerals = 0; };
	void onUnitComplete(BWAPI::Unit);

	//Function for getting an instance
	static ResourceManager& getInstance();

	//Reserve Minerals
	void reserveMinerals(BWAPI::UnitType);

	//Getter
	int getReservedMinerals() { return reservedMinerals; }

private:
	//Minerals
	int reservedMinerals;

	//Release
	void releaseMinerals(BWAPI::UnitType);

	ResourceManager() {};
	~ResourceManager() {};
};

