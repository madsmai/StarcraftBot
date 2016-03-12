#include "ExampleAIModule.h"

void ExampleAIModule::onStart(){
	//Enable user input
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

	//Call onStarts
	ProbeManager::getInstance().onStart();
	ResourceManager::getInstance().onStart();

	//Random message
	BWAPI::Broodwar->sendText("Starting...");
	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
}

void ExampleAIModule::onFrame(){
	//Prevent useless calls to onFrame
	if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0) { return; }
	
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
	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
}

void ExampleAIModule::onSendText(std::string text){
	//Print out message
	BWAPI::Broodwar->sendText(text.c_str());

	//Testing
}
