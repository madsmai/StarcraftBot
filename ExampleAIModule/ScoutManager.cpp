#include "ScoutManager.h"

/*
TODO:
- Liste af alle scouts der ikke laver noget
- Liste af alle scouts der laver noget
- metode onFrame:
	- Alle units der ikke scouter bliver sendt ud for at scoute
- metode onUnitDestroy:
	- Fjern unit fra listen hvis den er der
- metode onUnitComplete:
	- Sætter completed unit ind i liste, hvis den hører til
*/

ScoutManager::ScoutManager()
{
}


ScoutManager::~ScoutManager()
{
}

void ScoutManager::onFrame(){

}

void ScoutManager::onUnitDestroy(BWAPI::Unit unit){

}

void ScoutManager::onUnitDiscover(BWAPI::Unit unit){

}