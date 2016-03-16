#pragma once
#include <BWAPI.h>

class ResourceManager
{
public:
	//onStart and onUnitCreated
	void onStart(){ reservedMinerals = 400; }; //Has to start on 400 because the Nexus's minerals are released
	void onUnitCreate(BWAPI::Unit);

	//Function for getting an instance
	static ResourceManager& getInstance();

	//Reserve Minerals
	void reserveMinerals(BWAPI::UnitType);

	//Getter
	int getReservedMinerals() { return reservedMinerals; }

private:
	//Minerals
	int reservedMinerals;

	ResourceManager() {};
	~ResourceManager() {};
};

