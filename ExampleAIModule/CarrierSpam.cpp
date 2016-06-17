#include "CarrierSpam.h"

CarrierSpam::CarrierSpam() {

	setSquadSize(OffenseManager::getInstance().getSquadSize());
	setStrategyVariables();

	if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Protoss_Air_Weapons) == 0
		&& !Broodwar->self()->isUpgrading(UpgradeTypes::Protoss_Air_Weapons)
		&& !Broodwar->self()->isUpgrading(UpgradeTypes::Protoss_Air_Armor)){
		buildOrder = { UpgradeTypes::Protoss_Air_Weapons, evaluateStrategyRequest };
	}
	else if (Broodwar->self()->getUpgradeLevel(UpgradeTypes::Protoss_Air_Armor) == 0
		&& !Broodwar->self()->isUpgrading(UpgradeTypes::Protoss_Air_Armor)
		&& !Broodwar->self()->isUpgrading(UpgradeTypes::Protoss_Air_Weapons)){
		buildOrder = { UpgradeTypes::Protoss_Air_Armor, evaluateStrategyRequest };
	}
	else if (Broodwar->self()->minerals() > 1200
		&& Broodwar->self()->gas() > 700){
		buildOrder = { carrier, carrier, dragoon, dragoon,
			zealot, evaluateStrategyRequest };
	}
	else if (Broodwar->self()->gas() < 500
		&& Broodwar->self()->minerals() > 700
		&& ProbeManager::getInstance().getGasProbes().size() < 3){
		buildOrder = { gasworkerRequest, probe, carrier, zealot, zealot, zealot,
			evaluateStrategyRequest };
	}
	else if (Broodwar->self()->minerals() > 3000){
		buildOrder = { zealot, zealot, zealot, zealot, zealot,
			zealot, zealot, carrier, dragoon, dragoon, evaluateStrategyRequest };
	}
	else {
		buildOrder = { carrier, dragoon, dragoon, zealot,
			carrier, zealot, dragoon, zealot, zealot,
			evaluateStrategyRequest };
	}

	for (BuildOrderType order : buildOrder){
		BuildOrderManager::getInstance().getNewFixedOrderQueue().push_back(order);
	}

}

void CarrierSpam::evaluateStrategy(){

	//Broodwar << "Evaluating carrier spam strategy " << std::endl;

	if (InformationManager::getInstance().invisSpottet
		&& !InformationManager::getInstance().hasInvisDetection){
		StrategyManager::getInstance().setNextStrategy(StrategyManager::observerTech);
		return;
	}

	StrategyManager::getInstance().setNextStrategy(StrategyManager::carrier_spam);
	return;

}

void CarrierSpam::setStrategyVariables(){
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




