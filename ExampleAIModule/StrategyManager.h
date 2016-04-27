#pragma once
#include <BWAPI.h>
class StrategyManager {
public:
	static StrategyManager& getInstance();


	bool hasStrategy() { return ongoingStrategy; }
	void setOngoingStrategy(bool input) { ongoingStrategy = input; }

	int getStrategy(){ return strategy; }
	void setStrategy(int newStrategy){ strategy = newStrategy; }

	enum strategy{ none, commonZealotRush, aggressiveZealotRush, earlyDarkTemplar };


private:

	bool ongoingStrategy = true;

	int strategy;

	

};

