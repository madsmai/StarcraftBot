#include "BaseBuild.h"

//Author of class: Kasper

BaseBuild::BaseBuild() {

	setSquadSize(999);
	setStrategyVariables();

	buildOrder = { probe, probe, probe, probe, pylon};
	// ending with 8 / 16 supply

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void BaseBuild::setStrategyVariables(){
	if (starter){
		InformationManager::getInstance().starter = true;
		InformationManager::getInstance().endgame = false;
	}
	else {
		InformationManager::getInstance().starter = false;
		InformationManager::getInstance().endgame = true;
	}

	if (antiAir){
		InformationManager::getInstance().antiAir = true;
	}
	else {
		InformationManager::getInstance().antiAir = false;
	}

	if (antiInvis){
		InformationManager::getInstance().antiInvis = true;
	}
	else {
		InformationManager::getInstance().antiInvis = false;
	}

	if (antiRush){
		InformationManager::getInstance().antiRush = true;
	}
	else {
		InformationManager::getInstance().antiRush = false;
	}

	if (antiTurtle){
		InformationManager::getInstance().antiTurtle = true;
	}
	else {
		InformationManager::getInstance().antiTurtle = false;
	}

}



