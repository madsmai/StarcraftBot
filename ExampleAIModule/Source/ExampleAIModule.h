#pragma once
#include <BWAPI.h>
#include "../ProbeManager.h"

class ExampleAIModule : public BWAPI::AIModule
{
public:
	virtual void onStart();
	virtual void onFrame();
};
