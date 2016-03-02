#include "ExampleAIModule.h"

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart(){
	Broodwar->sendText("Hello me!");
}