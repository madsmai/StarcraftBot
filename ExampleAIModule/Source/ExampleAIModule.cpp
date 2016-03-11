#include "ExampleAIModule.h"

void ExampleAIModule::onStart(){
	ProbeManager::getInstance().onStart();
	ResourceManager::getInstance().onStart();
	BWAPI::Broodwar->sendText("Starting reserved minerals: ");
	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
}

void ExampleAIModule::onFrame()
{
	if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0) { return; }
	
	//Update ProbeManager
	ProbeManager::getInstance().onFrame();

}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit){
	ProbeManager::getInstance().onUnitComplete(unit);
	ResourceManager::getInstance().onUnitCreated(unit);

	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

}

void ExampleAIModule::onSendText(std::string text){
	BWAPI::Broodwar->sendText(text.c_str());

	//Testing
	int min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
}
