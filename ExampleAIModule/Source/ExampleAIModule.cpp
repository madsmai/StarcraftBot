#include "ExampleAIModule.h"
using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;


void ExampleAIModule::onStart(){
	//Enable user input
	BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);

	//Random message
	BWAPI::Broodwar->sendText("Starting...");
	/*
	For printing out amount of reservedMinerals:
	int  min = ResourceManager::getInstance().getReservedMinerals();
	BWAPI::Broodwar->sendText(std::to_string(min).c_str());
	*/

	BWTA::readMap();
	BWTA::analyze();
	analyzed = false;
	analysis_just_finished = false;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);

	//Call on starts
	BuildOrderManager::getInstance().onStart();
	BWAPI::Broodwar << BuildOrderManager::getInstance().getFixedOrderQueue().size() << std::endl;
}

void ExampleAIModule::onFrame(){
	//Display FPS
	BWAPI::Broodwar->drawTextScreen(200, 0, "FPS: %d", BWAPI::Broodwar->getFPS());
	BWAPI::Broodwar->drawTextScreen(200, 20, "Average FPS: %f", BWAPI::Broodwar->getAverageFPS());

	if (analyzed) {
		drawTerrainData();
	}
	if (analysis_just_finished) {
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
	}

	//Return if paused
	if (BWAPI::Broodwar->isReplay() || BWAPI::Broodwar->isPaused() || !BWAPI::Broodwar->self()) { return; }

	//Prevent useless calls to onFrame
	if (BWAPI::Broodwar->getFrameCount() % BWAPI::Broodwar->getLatencyFrames() != 0) { return; }

	//Call onFrames
	ProbeManager::getInstance().onFrame();
	BuildingManager::getInstance().onFrame();
	BuildOrderManager::getInstance().onFrame();
	OffenseManager::getInstance().onFrame();
	ScoutManager::getInstance().onFrame();
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit){
	//Call onUnitCompletes
	ProbeManager::getInstance().onUnitComplete(unit);
	BuildingManager::getInstance().onUnitComplete(unit);
	OffenseManager::getInstance().onUnitComplete(unit);
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit){
	//Call onUnitCreates
	ResourceManager::getInstance().onUnitCreate(unit);
}

void ExampleAIModule::onSendText(std::string text){
	//Print out message
	BWAPI::Broodwar->sendText(text.c_str());
	if (text == "Current Status") {
		InformationManager::getInstance().currentStatus();
	}
	else if (text == "size"){
		BWAPI::Broodwar << BuildOrderManager::getInstance().getFixedOrderQueue().size() << std::endl;
	}
	else if (text == "front"){
		BWAPI::Broodwar << BuildOrderManager::getInstance().getFixedOrderQueue().front().getName() << std::endl;
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit) {
	InformationManager::getInstance().onUnitDestroy(unit);
	BuildingManager::getInstance().onUnitDestroy(unit);
	OffenseManager::getInstance().onUnitDestroy(unit);
	ProbeManager::getInstance().onUnitDestroy(unit);
	ScoutManager::getInstance().onUnitDestroy(unit);
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit) {
	InformationManager::getInstance().onUnitDiscover(unit);
	ScoutManager::getInstance().onUnitDiscover(unit);
}

DWORD WINAPI AnalyzeThread() {
	//BWTA::analyze();

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

void ExampleAIModule::drawTerrainData() {
	//we will iterate through all the base locations, and draw their outlines.
	for (const auto& baseLocation : BWTA::getBaseLocations()) {
		TilePosition p = baseLocation->getTilePosition();

		//draw outline of center location
		Position leftTop(p.x * TILE_SIZE, p.y * TILE_SIZE);
		Position rightBottom(leftTop.x + 4 * TILE_SIZE, leftTop.y + 3 * TILE_SIZE);
		Broodwar->drawBoxMap(leftTop, rightBottom, Colors::Blue);

		//draw a circle at each mineral patch
		for (const auto& mineral : baseLocation->getStaticMinerals()) {
			Broodwar->drawCircleMap(mineral->getInitialPosition(), 30, Colors::Cyan);
		}

		//draw the outlines of Vespene geysers
		for (const auto& geyser : baseLocation->getGeysers()) {
			TilePosition p1 = geyser->getInitialTilePosition();
			Position leftTop1(p1.x * TILE_SIZE, p1.y * TILE_SIZE);
			Position rightBottom1(leftTop1.x + 4 * TILE_SIZE, leftTop1.y + 2 * TILE_SIZE);
			Broodwar->drawBoxMap(leftTop1, rightBottom1, Colors::Orange);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if (baseLocation->isIsland()) {
			Broodwar->drawCircleMap(baseLocation->getPosition(), 80, Colors::Yellow);
		}
	}

	//we will iterate through all the regions and ...
	for (const auto& region : BWTA::getRegions()) {
		// draw the polygon outline of it in green
		BWTA::Polygon p = region->getPolygon();
		for (size_t j = 0; j < p.size(); ++j) {
			Position point1 = p[j];
			Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLineMap(point1, point2, Colors::Green);
		}
		// visualize the chokepoints with red lines
		for (auto const& chokepoint : region->getChokepoints()) {
			Position point1 = chokepoint->getSides().first;
			Position point2 = chokepoint->getSides().second;
			Broodwar->drawLineMap(point1, point2, Colors::Red);
		}
	}
}