#include "ProbeManager.h"

/*
TODO:
- Liste af probes
- Liste af bygninger der skal bygges af probes
- metode onFrame :
	- Sende tætteste probe ud for at bygge næste bygning hvis der er nok materialer
	- Sende en probe til ScoutManager og sletter probe fra egen liste, hvis bedt om det
	- Sende idle probes til nærmeste mineral patch
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen

*/

ProbeManager::ProbeManager()
{
}


ProbeManager::~ProbeManager()
{
}

void ProbeManager::onFrame(){

}

void ProbeManager::onUnitDestroy(){

}

