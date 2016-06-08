#include "EarlyDarkTemplar.h"


EarlyDarkTemplar::EarlyDarkTemplar() {

	Broodwar << name << std::endl;

	setSquadSize(3);

	buildOrder = { // base build
		probe, gateway, probe, scoutRequest, assimilator, probe,
		gasworkerRequest, gasworkerRequest, cybercore, probe,
		citadel, probe, gasworkerRequest, archives,
		gateway, dtemplar, dtemplar, pylon,
		dtemplar, probe, pylon, probe,
		evaluateStrategyRequest };

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}
}


void EarlyDarkTemplar::evaluateStrategy(){

	Broodwar << "Evaluating early dark templar" << std::endl;
	StrategyManager::getInstance().setNextStrategy(StrategyManager::transitionMidGame);

}



