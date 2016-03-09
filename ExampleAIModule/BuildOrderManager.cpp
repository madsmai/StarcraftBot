#include "BuildOrderManager.h"

/*
TODO:
- Indeholder hardcoded logik for hvornår vi skal lave hvad, med hensyn til antal probes, zealots, tid gået, mineraler etc.
- Personlig liste for hvilke units der findes i spillet så det er nemmere at implementere ovenstående logik
- Metode onFrame:
	- Sender scouts, bygnigner eller units til deres respective objects lister, hvis logikken opfyldes
- Metode onUnitDestroyed:
	- Fjerne units fra personlig liste
*/

BuildOrderManager::BuildOrderManager()
{
}


BuildOrderManager::~BuildOrderManager()
{
}

void BuildOrderManager::onFrame(){

}

void BuildOrderManager::onUnitDestroy(BWAPI::Unit unit){

}

BuildOrderManager& BuildOrderManager::getInstance(){ //Return ref to BuildOrderManager object
	static BuildOrderManager i; //Make static instance i
	return i;
}
