#include "ExampleAIModule.h"

void ExampleAIModule::onStart(){
	//Enable user input
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

	//Call onStarts
	ProbeManager::getInstance().onStart();

	//Random message
	BWAPI::Broodwar->sendText("Starting...");
	/*
	For printing out amount of reservedMinerals:
	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	*/
}

void ExampleAIModule::onFrame(){
	//Return if paused
	if (BWAPI::Broodwar->isReplay() || BWAPI::Broodwar->isPaused() || !BWAPI::Broodwar->self()) { return; }

	//Prevent useless calls to onFrame
	if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0) { return; }

	//Display FPS
	BWAPI::Broodwar->drawTextScreen(200, 0, "FPS: %d", BWAPI::Broodwar->getFPS());
	BWAPI::Broodwar->drawTextScreen(200, 20, "Average FPS: %f", BWAPI::Broodwar->getAverageFPS());
	
	//Call onFrames
	ProbeManager::getInstance().onFrame();

}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit){
	//Call onUnitCompletes
	ProbeManager::getInstance().onUnitComplete(unit);
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit){
	//Call onUnitCreates
	ResourceManager::getInstance().onUnitCreate(unit);
}

void ExampleAIModule::onSendText(std::string text){
	//Print out message
	BWAPI::Broodwar->sendText(text.c_str());



}
