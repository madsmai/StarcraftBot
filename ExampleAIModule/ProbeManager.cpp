#include "ProbeManager.h"
#include "BWAPI.h"

/*
TODO:
- metode onFrame:
	- Sende t�tteste probe ud for at bygge n�ste bygning hvis der er nok materialer
	- Sende idle probes til n�rmeste mineral patch
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- S�tter completed unit ind i liste, hvis den h�rer til
- metode onStart:
	- S�tter 4 f�rste probes ind i liste af probes
- metode makeScout:
	- Sende en probe til ScoutManager og sletter probe fra egen liste, hvis bedt om det
- metode constructBuilding:
	- Fors�ger at lave en bygning hvis der er nok mineraler, hvis det lykkedes s� returnerer den sandt
*/

void ProbeManager::onFrame(){

	if (!pendingBuildings.empty()){
		//BWAPI::Unit u = getProbes().front();

		getProbes().front()->build(BWAPI::UnitTypes::Protoss_Pylon, 
			BWAPI::Broodwar->getBuildLocation(BWAPI::UnitTypes::Protoss_Pylon, getProbes().front()->getTilePosition()));
	}

	for (BWAPI::Unit unit : getProbes()){
		if (unit->exists()	&&	unit->getType().isWorker()	&&	unit->isIdle()){
			unit->gather(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
		}
	}
}

void ProbeManager::onUnitDestroy(BWAPI::Unit unit){


}

void ProbeManager::onUnitComplete(BWAPI::Unit unit){
	if (unit->getType().isWorker){
		addProbe(unit);
	}
}

void ProbeManager::onStart(){
	for (auto &unit : BWAPI::Broodwar->self()->getUnits()) {
		if (unit->exists		&&	  unit->getType().isWorker){
			addProbe(unit);
		}
	}
}

bool ProbeManager::becomeScout(BWAPI::Unit){

}

bool ProbeManager::constructBuilding(BWAPI::Unit, BWAPI::UnitType){

}

void ProbeManager::addProbe(BWAPI::Unit unit){


}

void ProbeManager::removeProbe(BWAPI::Unit unit){

}

void ProbeManager::addPendingBuilding(BWAPI::Unit unit){

}

void ProbeManager::removePendingBuilding(BWAPI::Unit unit){

}

ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}


//Getters and setters
std::vector<BWAPI::Unit> ProbeManager::getProbes(){

}

void ProbeManager::setProbes(std::vector<BWAPI::Unit> units){

}

std::queue<BWAPI::Unit> ProbeManager::getPendingBuildings(){

}

void ProbeManager::setPendingBuildings(std::queue<BWAPI::Unit> units){

}
