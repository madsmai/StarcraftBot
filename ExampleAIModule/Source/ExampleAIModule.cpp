#include "ExampleAIModule.h"

using namespace BWAPI;
using namespace Filter;

void ExampleAIModule::onStart(){
	Broodwar->sendText("Hello me!");
}

void ExampleAIModule::onFrame()
{
	// Prevent spamming by only running our onFrame once every number of latency frames.
	// Latency frames are the number of frames before commands are processed.
	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	// Iterate through all the units that we own
	for (auto &u : Broodwar->self()->getUnits()) {

		// Ignore the unit if it no longer exists
		// Make sure to include this block when handling any Unit pointer!
		if (!u->exists())
			continue;

		// If the unit is a worker unit
		if (u->getType().isWorker()) {

			// if our worker is idle
			if (u->isIdle()){
				//Gather minerals
				if (!u->gather(u->getClosestUnit(IsMineralField))) {
					// If that fails, then print the last error message
					Broodwar << Broodwar->getLastError() << std::endl;
				}
			} // closure: if idle
		} // closure: builder type
	} // closure: unit iterator
}