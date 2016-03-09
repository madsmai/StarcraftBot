#include "BuildingManager.h"

/*
TODO:
- Liste af alle byginger
- Liste af units der skal bygges af bygninger
- metode onFrame:
	- Sætte bygninger til at producere units fra listen hvis der er mineraler nok
		- Det er vigtigt at produktionen så vidt muligt bliver spredt ud over flere bygnigner 
			så det går hurtigere
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
*/

BuildingManager::BuildingManager()
{
}


BuildingManager::~BuildingManager()
{
}

void BuildingManager::onFrame(){

}

void BuildingManager::onUnitDestroy(){

}
