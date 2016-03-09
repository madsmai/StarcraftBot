#include "ProbeManager.h"

/*
TODO:
- Liste af probes
- Liste af bygninger der skal bygges af probes
- metode onFrame :
	- Sende t�tteste probe ud for at bygge n�ste bygning hvis der er nok materialer
	- Sende en probe til ScoutManager og sletter probe fra egen liste, hvis bedt om det
	- Sende idle probes til n�rmeste mineral patch
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- S�tter completed unit ind i liste, hvis den h�rer til
- metode onStart:
	- S�tter 4 f�rste probes ind i liste af probes
*/

void ProbeManager::onFrame(){

}

void ProbeManager::onUnitDestroy(BWAPI::Unit unit){

}

void ProbeManager::onUnitComplete(BWAPI::Unit unit){

}

void ProbeManager::onStart(){

}

void addProbe(BWAPI::Unit){

}

void removeProbe(BWAPI::Unit){

}

void addPendingBuilding(BWAPI::Unit){

}

void removePendingBuilding(BWAPI::Unit){

}

ProbeManager& ProbeManager::getInstance(){ //Return ref to probemanager object
	static ProbeManager i; //Make static instance i
	return i;
}

//Getters and setters

std::vector<BWAPI::Unit> getProbes(){

}

void setProbes(std::vector<BWAPI::Unit>){

}

std::vector<BWAPI::Unit> getPendingBuildings(){

}

void setPendingBuildings(std::vector<BWAPI::Unit>){

}
