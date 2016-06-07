#include "StrategyManager.h"
using namespace BWAPI;


int StrategyManager::setInitialStrategy(){

	Race enemyRace = Broodwar->enemy()->getRace();
	switch (enemyRace){

	case Races::Enum::Protoss:
		Broodwar << "Enemy race is: " << enemyRace << std::endl;
		Broodwar << "Setting initial strategy to: 'Aggressive Zealot rush'" << std::endl;
		return aggressiveZealotRush;
		break;

	case Races::Enum::Zerg:
		Broodwar << "Enemy race is: " << enemyRace << std::endl;
		Broodwar << "Setting initial strategy to: 'Aggressive Zealot rush'" << std::endl;
		return aggressiveZealotRush;
		break;

	case Races::Enum::Terran:
		Broodwar << "Enemy race is: " << enemyRace << std::endl;
		Broodwar << "Setting initial strategy to: 'Common Zealot rush'" << std::endl;
		return earlyDarkTemplar;
		break;

	case Races::Enum::Unknown:
		Broodwar << "Enemy race is unknown" << std::endl;
		Broodwar << "Setting initial strategy to: 'Common Zealot rush'" << std::endl;
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
	}
}


StrategyManager& StrategyManager::getInstance(){ //Return ref to BuildOrderManager object
	static StrategyManager i; //Make static instance i
	return i;
}
