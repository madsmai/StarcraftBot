#include "GameManager.h"

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;


void GameManager::onStart(){
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
	BWAPI::Broodwar << BuildOrderManager::getInstance().getNewFixedOrderQueue().size() << std::endl;
}

void GameManager::onEnd(bool isWinner){
	std::ofstream log;
	if (BWAPI::Broodwar->enemy()->getRace() == Races::Protoss){
		log.open("bwapi-data//AI/WLvsProtoss.txt", std::ios::app);
		if (isWinner){
			log << "1" << "\n";
		}
		else {
			log << "0" << "\n";
		}
		log.flush();
		log.close();
	}
	else if (BWAPI::Broodwar->enemy()->getRace() == Races::Terran){
		log.open("bwapi-data//AI/WLvsTerran.txt", std::ios::app);
		if (isWinner){
			log << "1" << "\n";
		}
		else {
			log << "0" << "\n";
		}
		log.flush();
		log.close();
	}
	else if (BWAPI::Broodwar->enemy()->getRace() == Races::Zerg){
		log.open("bwapi-data//AI/WLvsZerg.txt", std::ios::app);
		if (isWinner){
			log << "1" << "\n";
		}
		else {
			log << "0" << "\n";
		}
		log.flush();
		log.close();
	}


}


void GameManager::onFrame(){

	Broodwar->drawTextScreen(0, 0,
		"FPS: %d \t APM: %d \n FrameCount: %d \t rushing?: %d",
		Broodwar->getFPS(), Broodwar->getAPM(), Broodwar->getFrameCount(), OffenseManager::getInstance().rushOngoing);

	Broodwar->drawTextScreen(200, 0, "Invis spottet: %d \n" "has detection: %d",
		InformationManager::getInstance().invisSpottet, InformationManager::getInstance().hasInvisDetection);

	Broodwar->drawTextScreen(300, 100, "Current Strategy: %d \n Next Strategy: %d",
		StrategyManager::getInstance().getCurrentStrategy(), StrategyManager::getInstance().getNextStrategy());

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
	PlacementManager::getInstance().onFrame();
	ProbeManager::getInstance().onFrame();
	BuildingManager::getInstance().onFrame();
	BuildOrderManager::getInstance().onFrame();
	OffenseManager::getInstance().onFrame();
	ScoutManager::getInstance().onFrame();
}

void GameManager::onUnitComplete(BWAPI::Unit unit){
	//Call onUnitCompletes
	ProbeManager::getInstance().onUnitComplete(unit);
	BuildingManager::getInstance().onUnitComplete(unit);
	OffenseManager::getInstance().onUnitComplete(unit);
}

void GameManager::onUnitCreate(BWAPI::Unit unit){
	//Call onUnitCreates
	ResourceManager::getInstance().onUnitCreate(unit);
}

void GameManager::onUnitMorph(Unit unit){
	//Call onUnitMorph
	ResourceManager::getInstance().onUnitMorph(unit);
}


void GameManager::onSendText(std::string text){
	//Print out message
	BWAPI::Broodwar->sendText(text.c_str());
	if (text == "Current Status") {
		InformationManager::getInstance().currentStatus();
	}
	else if (text == "EAS") {
		InformationManager::getInstance().enemyArmyStatus();
	}
	else if (text == "OAS") {
		InformationManager::getInstance().ourArmyStatus();
	}
	else if (text == "Passives") {
		std::vector<BWAPI::Unit>::iterator it;
		for (it = InformationManager::getInstance().enemyPassiveBuildings.begin(); it != InformationManager::getInstance().enemyPassiveBuildings.end(); it++) {
			Unit u = *it;
			Broodwar << "Element in passives:" << u->getType() << std::endl;
		}
	}
	else if (text == "Fighters Size") {
		Broodwar << OffenseManager::getInstance().fighters.size() << std::endl;
	}
	else if (text == "Army Strength enemy") {

		Broodwar << InformationManager::getInstance().calculateArmyStrength(Broodwar->enemy()) << std::endl;
	}

	else if (text == "invis"){
	
		InformationManager::getInstance().invisSpottet = true;

	}

	else if (text == "Army Strength self") {
		Broodwar << InformationManager::getInstance().calculateArmyStrength(Broodwar->self()) << std::endl;
	}
	else if (text == "size"){
		Broodwar << BuildOrderManager::getInstance().getNewFixedOrderQueue().size() << std::endl;
	}
	else if (text == "front"){
		Broodwar << BuildOrderManager::getInstance().getNewFixedOrderQueue().front().getName() << std::endl;
	}
	else if (text == "reserved"){
		Broodwar << "reserved minerals: " << ResourceManager::getInstance().getReservedMinerals() << std::endl;
	}
	else if (text == "zealot"){
		Broodwar << "zealot supply required " << UnitTypes::Protoss_Zealot.supplyRequired() << std::endl;
	}
	else if (text == "enemyattackers"){
		for (Unit attacker : InformationManager::getInstance().enemyAttackers){
			Broodwar << "Enemy attackers: " << attacker->getType() << std::endl;
		}
	}

}

void GameManager::onUnitDestroy(BWAPI::Unit unit) {
	PlacementManager::getInstance().onUnitDestroy(unit);
	InformationManager::getInstance().onUnitDestroy(unit);
	BuildingManager::getInstance().onUnitDestroy(unit);
	OffenseManager::getInstance().onUnitDestroy(unit);
	ProbeManager::getInstance().onUnitDestroy(unit);
	ScoutManager::getInstance().onUnitDestroy(unit);
}

void GameManager::onUnitDiscover(BWAPI::Unit unit) {
	InformationManager::getInstance().onUnitDiscover(unit);
	ScoutManager::getInstance().onUnitDiscover(unit);
}

DWORD WINAPI AnalyzeThread() {
	//BWTA::analyze();

	analyzed = true;
	analysis_just_finished = true;
	return 0;
}

void GameManager::drawTerrainData() {
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