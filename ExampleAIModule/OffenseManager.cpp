#include "OffenseManager.h"

/*
TODO:
- Liste af alle zealots
- Sørg for at samle zealots sammen og angrib med dem sammen, det samlede antal skal varierer
	alt efter modstanderens forsvar
- En måde at få zealots til at prioriterer hvad de angriber
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- Sætter completed unit ind i liste, hvis den hører til
*/

OffenseManager::OffenseManager()
{
}

OffenseManager::~OffenseManager()
{
}

void OffenseManager::onUnitDestroy(BWAPI::Unit unit){

}

void OffenseManager::onUnitComplete(BWAPI::Unit unit){

}
