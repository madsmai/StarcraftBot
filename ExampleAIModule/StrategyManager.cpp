#include "StrategyManager.h"
using namespace BWAPI;


int StrategyManager::setInitialStrategy(){

	Race enemyRace = Broodwar->enemy()->getRace();
	Broodwar << "Enemy race is: " << enemyRace << std::endl;
	switch (enemyRace){

	case Races::Enum::Protoss:
		return aggressiveZealotRush;
		break;

	case Races::Enum::Zerg:
		return aggressiveZealotRush;
		break;

	case Races::Enum::Terran:
		return earlyDarkTemplar;
		break;

	case Races::Enum::Unknown:
		return commonZealotRush;
		break;

	}

	return 0;
}

void StrategyManager::evaluateStrategies(){

	switch (getCurrentStrategy()){

	case commonZealotRush:
		CommonZealotRush::evaluateStrategy();
		break;

	case aggressiveZealotRush:
		AggressiveZealotRush::evaluateStrategy();
		break;

	case earlyDarkTemplar:
		EarlyDarkTemplar::evaluateStrategy();
		break;

	case continueZealotRush:
		ContinueZealotRush::evaluateStrategy();
		break;

	case transitionMidGame:
		TransitionMidGame::evaluateStrategy();
		break;

	case reavers:
		Reavers::evaluateStrategy();
		break;

	case carriers:
		Carriers::evaluateStrategy();
		break;

	case addGoons:
		AddGoons::evaluateStrategy();
		break;
	}
}


StrategyManager& StrategyManager::getInstance(){ //Return ref to BuildOrderManager object
	static StrategyManager i; //Make static instance i
	return i;
}
