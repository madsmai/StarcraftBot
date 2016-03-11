#include "ProbeManager.h"
#include "BWAPI.h"
#include <string>

/*
TODO:
- metode onFrame:
	- Sende tætteste probe ud for at bygge næste bygning hvis der er nok materialer
	- Sende idle probes til nærmeste mineral patch
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- Sætter completed unit ind i liste, hvis den hører til
- metode onStart:
	- Sætter 4 første probes ind i liste af probes
- metode makeScout:
	- Sende en probe til ScoutManager og sletter probe fra egen liste, hvis bedt om det
- metode constructBuilding:
	- Forsøger at lave en bygning hvis der er nok mineraler, hvis det lykkedes så returnerer den sandt
*/

void ProbeManager::onFrame(){

	//Construct the next building in the queue
	/*if (!pendingBuildings.empty()){
		BWAPI::Unit u = getProbes().front();

		getProbes().front()->build(BWAPI::UnitTypes::Protoss_Pylon, 
			BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Pylon, getProbes().front()->getTilePosition()));
	}*/

	//Make idle workers do stuff
	std::vector<BWAPI::Unit>::iterator it;
	for (it = probes.begin(); it != probes.end(); it++){
		BWAPI::Broodwar->sendText("Inside for-loop");
		BWAPI::Unit unit = *it;
		if (unit->exists() && unit->isIdle()){
			unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
			BWAPI::Broodwar->sendText("Idle worker has been set to work");
		}
	}
}

//Remove destroyed worker
void ProbeManager::onUnitDestroy(BWAPI::Unit unit){
	if (unit->getType().isWorker()){
		std::vector<BWAPI::Unit>::iterator it;
		for (it = probes.begin(); it != probes.end(); it++){
			if (*it == unit){
				probes.erase(it);
				break;
			}
		}
	}
}

//Add newly made worker to list
void ProbeManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isWorker()){
		probes.push_back(unit);
	}
}

//Add the first 4 probes to list
void ProbeManager::onStart(){
	BWAPI::Broodwar->sendText("onStart is being run..");
	for (auto &unit : BWAPI::Broodwar->self()->getUnits()) {
		if (unit->exists()		&&	  unit->getType().isWorker()){
			probes.push_back(unit);
			BWAPI::Broodwar->sendText("Probe was added to list..");
			int i = probes.size();
			std::string s = std::to_string(i);
			char const *pchar = s.c_str();
			BWAPI::Broodwar->sendText(pchar);
		}
	}
}

//Remove worker from list and put into ScoutManager list
bool ProbeManager::becomeScout(BWAPI::Unit){

	return true;
}

//Construct a building with a specific worker
bool ProbeManager::constructBuilding(BWAPI::Unit, BWAPI::UnitType){

	return true;
}

//Get a static instance of class
ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}