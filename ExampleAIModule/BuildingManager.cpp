#include "BuildingManager.h"

/*
TODO:
- Liste af alle byginger
- Liste af units der skal bygges af bygninger
- metode onFrame:
	- S�tte bygninger til at producere units fra listen hvis der er mineraler nok
		- Det er vigtigt at produktionen s� vidt muligt bliver spredt ud over flere bygnigner 
			s� det g�r hurtigere
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- S�tter completed unit ind i liste, hvis den h�rer til
*/

BuildingManager::BuildingManager()
{
}


BuildingManager::~BuildingManager()
{
}

void BuildingManager::onFrame(){

}

void BuildingManager::onUnitDestroy(BWAPI::Unit unit){

}

void BuildingManager::onUnitComplete(BWAPI::Unit unit){

}
