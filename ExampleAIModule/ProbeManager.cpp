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
*/

ProbeManager::ProbeManager()
{
}


ProbeManager::~ProbeManager()
{
}

void ProbeManager::onFrame(){

}

void ProbeManager::onUnitDestroy(BWAPI::Unit unit){

}

void ProbeManager::onUnitComplete(BWAPI::Unit unit){

}

