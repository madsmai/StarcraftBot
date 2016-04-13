#pragma once
#include <BWAPI.h>
using namespace BWAPI;
class ResourceManager
{
public:
	//onUnitCreated
	void onUnitCreate(Unit);
	void onUnitMorph(Unit);

	//Function for getting an instance
	static ResourceManager& getInstance();

	//Reserve Minerals and gas
	void reserveMinerals(BWAPI::UnitType);
	void reserveGas(BWAPI::UnitType);

	//Getter
	int const &getReservedMinerals() { return reservedMinerals; } //pure read-only access
	int const &getReservedGas() { return reservedGas; }

private:
	//Minerals
	int reservedMinerals = 400; //Has to start on 400 because the Nexus's minerals are released
	int reservedGas = 0;

	ResourceManager() {};
	~ResourceManager() {};
};

