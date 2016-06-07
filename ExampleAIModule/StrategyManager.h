#pragma once
#include <BWAPI.h>

#include "AggressiveZealotRush.h"
#include "CommonZealotRush.h"
#include "EarlyDarkTemplar.h"
#include "ContinueZealotRush.h"
#include "TransitionMidGame.h"
#include "Reavers.h"

class StrategyManager {
public:

	static StrategyManager& getInstance();

	enum strategy{ none, basic, commonZealotRush, aggressiveZealotRush, 
		earlyDarkTemplar, continueZealotRush, transitionMidGame,
		reavers};


	int getCurrentStrategy(){ return currentStrategy; }
	int getNextStrategy(){ return nextStrategy; }
	void setCurrentStrategy(int strategy){ currentStrategy = strategy; }
	void setNextStrategy(int strategy){ nextStrategy = strategy; }

	
	void evaluateStrategies();


	int setInitialStrategy();
	void evaluateInitialStrategy();


private:

	int currentStrategy;
	int nextStrategy;

};

