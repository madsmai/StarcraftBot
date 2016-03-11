#include "ExampleAIModule.h"

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart(){
	Broodwar->sendText("Hello me!");
	ProbeManager::getInstance().onStart();
}

void ExampleAIModule::onFrame()
{
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0) { return; }
	
	//Update ProbeManager
	ProbeManager::getInstance().onFrame();


}