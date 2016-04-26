#include "StrategyManager.h"


StrategyManager& StrategyManager::getInstance(){ //Return ref to BuildOrderManager object
	static StrategyManager i; //Make static instance i
	return i;
}
