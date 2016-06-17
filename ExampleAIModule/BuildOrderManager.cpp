#include "BuildOrderManager.h"
using namespace BWAPI;

void BuildOrderManager::onStart(){

	// initializes the basic build
	BaseBuild();
	StrategyManager::getInstance().setCurrentStrategy(StrategyManager::basic);

	// sets the initial strategy depending on the enemy race
	StrategyManager::getInstance().setNextStrategy(StrategyManager::getInstance().setInitialStrategy());

}

void BuildOrderManager::onFrame(){

	if (StrategyManager::getInstance().getCurrentStrategy() == StrategyManager::none){

		//Broodwar << "Setting a new current strategy" << std::endl;

		StrategyManager::getInstance().setCurrentStrategy(StrategyManager::getInstance().getNextStrategy());
		StrategyManager::getInstance().setNextStrategy(StrategyManager::none);

		switch (StrategyManager::getInstance().getCurrentStrategy()){

		case StrategyManager::commonZealotRush:
			CommonZealotRush();
			break;

		case StrategyManager::aggressiveZealotRush:
			AggressiveZealotRush();
			break;

		case StrategyManager::earlyDarkTemplar:
			EarlyDarkTemplar();
			break;

		case StrategyManager::continueZealotRush:
			ContinueZealotRush();
			break;

		case StrategyManager::transitionMidGame:
			TransitionMidGame();
			break;

		case StrategyManager::reavers:
			Reavers();
			break;

		case StrategyManager::carriers:
			Carriers();
			break;

		case StrategyManager::addGoons:
			AddGoons();
			break;

		case StrategyManager::observerTech:
			ObserverTech();
			break;

		case StrategyManager::carrier_spam:
			CarrierSpam();
			break;

		case StrategyManager::none:
			TransitionMidGame();
			break;

		}
	}
}

BuildOrderManager& BuildOrderManager::getInstance(){ //Return ref to BuildOrderManager object
	static BuildOrderManager i; //Make static instance i
	return i;
}
