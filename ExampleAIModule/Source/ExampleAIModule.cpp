#include "ExampleAIModule.h"

void ExampleAIModule::onStart(){
	BWAPI::Broodwar->sendText("Starting up");
	ProbeManager::getInstance().onStart();
	ResourceManager::getInstance().onStart();
}

void ExampleAIModule::onFrame()
{
	if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0) { return; }
	
	//Update ProbeManager
	ProbeManager::getInstance().onFrame();
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit){
	ProbeManager::getInstance().onUnitComplete(unit);
	ResourceManager::getInstance().onUnitComplete(unit);
}

void ExampleAIModule::onSendText(std::string text){
	BWAPI::Broodwar->sendText("%s", text.c_str());



}
