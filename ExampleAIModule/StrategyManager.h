#pragma once
#include <BWAPI.h>

#include "AggressiveZealotRush.h"
#include "CommonZealotRush.h"
#include "EarlyDarkTemplar.h"
#include "ContinueZealotRush.h"
#include "TransitionMidGame.h"
#include "Reavers.h"
#include "Carriers.h"
#include "AddGoons.h"
#include "ObserverTech.h"
#include "CarrierSpam.h"

class StrategyManager {
public:

	static StrategyManager& getInstance();

	enum strategy{ none, basic, commonZealotRush, aggressiveZealotRush, 
		earlyDarkTemplar, continueZealotRush, transitionMidGame,
		addGoons ,reavers, carriers, carrier_spam, observerTech};


	int getCurrentStrategy(){ return currentStrategy; }
	int getNextStrategy(){ return nextStrategy; }
	void setCurrentStrategy(int strategy){ currentStrategy = strategy; }
	void setNextStrategy(int strategy){ nextStrategy = strategy; }

	bool transistionMidCalled = false;
	bool carriersCalled = false;

	void evaluateStrategies();


	int setInitialStrategy();
	void evaluateInitialStrategy();


private:

	int currentStrategy;
	int nextStrategy;

};

