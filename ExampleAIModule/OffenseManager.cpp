#include "OffenseManager.h"

/*
TODO:
- Liste af alle zealots
- S�rg for at samle zealots sammen og angrib med dem sammen, det samlede antal skal varierer
	alt efter modstanderens forsvar
- En m�de at f� zealots til at prioriterer hvad de angriber
- metode onUnitDestroy:
	- Fjerne destroyed object fra listen, hvis den er i listen
- metode onUnitComplete:
	- S�tter completed unit ind i liste, hvis den h�rer til
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
