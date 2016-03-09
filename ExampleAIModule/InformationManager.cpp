#include "InformationManager.h"

/*
TODO:
- Gemme information om hvor modstanderen er og hvor godt deres forsvar er, 
	dette skal være tilgængeligt for de anddre klasser
*/

InformationManager::InformationManager()
{
}


InformationManager::~InformationManager()
{
}

InformationManager& InformationManager::getInstance(){ //Return ref to InformationManager object
	static InformationManager i; //Make static instance i
	return i;
}
