#include "ExampleAIModule.h"
#include <iostream>
#include <vector>
#include <BWTA.h>
#include <windows.h>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;

void ExampleAIModule::onStart()
{
	// Hello World!
	Broodwar->sendText("Hello ME!");

	// Print the map name.
	// BWAPI returns std::string when retrieving a string, don't forget to add .c_str() when printing!
	Broodwar << "The map is " << Broodwar->mapName() << "!" << std::endl;

	// Enable the UserInput flag, which allows us to control the bot and type messages.
	Broodwar->enableFlag(Flag::UserInput);

	// Uncomment the following line and the bot will know about everything through the fog of war (cheat).
	//Broodwar->enableFlag(Flag::CompleteMapInformation);

	// Set the command optimization level so that common commands can be grouped
	// and reduce the bot's APM (Actions Per Minute).
	Broodwar->setCommandOptimizationLevel(2);

	// Check if this is a replay
	if (Broodwar->isReplay())
	{

		// Announce the players in the replay
		Broodwar << "The following players are in this replay:" << std::endl;

		// Iterate all the players in the game using a std:: iterator
		Playerset players = Broodwar->getPlayers();
		for (auto p : players)
		{
			// Only print the player if they are not an observer
			if (!p->isObserver())
				Broodwar << p->getName() << ", playing as " << p->getRace() << std::endl;
		}

	}
	else // if this is not a replay
	{
		// Retrieve you and your enemy's races. enemy() will just return the first enemy.
		// If you wish to deal with multiple enemies then you must use enemies().
		if (Broodwar->enemy()) // First make sure there is an enemy
			Broodwar << "The matchup is " << Broodwar->self()->getRace() << " vs " << Broodwar->enemy()->getRace() << std::endl;
	}

	// setting values
	refinery = false;
	refineryFinished = false;
	workers = 0;
	mineralsReserved = 0;
	pendingBuildings;
	scout = NULL;
	firstPylon = true;
	startPositions;

	// BWTA2 MAP ANALYSIS YO
	BWTA::readMap();
	BWTA::analyze();
	analyzed = false;
	analysis_just_finished = false;


	Broodwar << "Analyzing map... this may take a minute" << std::endl;;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AnalyzeThread, NULL, 0, NULL);
}

void ExampleAIModule::onEnd(bool isWinner)
{
	// Called when the game ends
	if (isWinner)
	{
		// Log your win here!
	}
}

void ExampleAIModule::onFrame()
{
	// Called once every game frame

	// Display the game frame rate as text in the upper left area of the screen
	Broodwar->drawTextScreen(200, 0, "FPS: %d", Broodwar->getFPS());
	Broodwar->drawTextScreen(200, 20, "Average FPS: %f", Broodwar->getAverageFPS());

	// Return if the game is a replay or is paused
	if (Broodwar->isReplay() || Broodwar->isPaused() || !Broodwar->self())
		return;


	//BWTA draw
	if (analyzed) {
		drawTerrainData();
	}

	if (analysis_just_finished) {
		Broodwar << "Finished analyzing map." << std::endl;;
		analysis_just_finished = false;
	}


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

		// Ignore the unit if it has one of the following status ailments
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;

		// Ignore the unit if it is in one of the following states
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;

		// Ignore the unit if it is incomplete or busy constructing
		if (!u->isCompleted() || u->isConstructing())
			continue;

		// Finally make the unit do some stuff!
		// -----------------------------!!----------------------------------

		// If the unit is a worker unit
		if (u->getType().isWorker()) {

			supplyCheckAndBuild(u);

			// checks for a refinery
			if (!refinery && Broodwar->self()->minerals() >= UnitTypes::Protoss_Assimilator.mineralPrice()
				&& (Broodwar->self()->supplyUsed() / 2) > 10){
				UnitType refineryType = UnitTypes::Protoss_Assimilator;
				constructBuilding(refineryType, u);
				refinery = true;
			}

			// if our worker is idle
			if (u->isIdle()){

				// Order workers carrying a resource to return them to the center,
				// otherwise find a mineral patch to harvest.
				if (u->isCarryingGas() || u->isCarryingMinerals()) {
					u->returnCargo();
				}
				else if (!u->getPowerUp())  // The worker cannot harvest anything if it
				{                             // is carrying a powerup such as a flag

					if (refineryFinished && refineryWorkers < 3) {
						if (!u->isGatheringMinerals()){
							refineryWorkers++;
							if (!u->gather(u->getClosestUnit(IsRefinery))){
								Broodwar << Broodwar->getLastError() << std::endl;
							}
						}
					}
					// Harvest from the nearest mineral patch
					else {
						if (!u->gather(u->getClosestUnit(IsMineralField))) {
							// If the call fails, then print the last error message
							Broodwar << Broodwar->getLastError() << std::endl;
						}
					}
				} // closure: has no powerup
			} // closure: if idle
		} // closure: builder type


		// A resource depot is a Command Center, Nexus, or Hatchery
		else if (u->getType().isResourceDepot()) {

			// Order the depot to construct more workers! But only when it is idle and there is below 25 workers.
			if (Broodwar->self()->minerals() - mineralsReserved >= UnitTypes::Protoss_Probe.mineralPrice()
				&& u->isIdle()
				&& workers < 25
				&& !u->train(u->getType().getRace().getWorker())){

				// If that fails, draw the error at the location so that you can visibly see what went wrong!
				// However, drawing the error once will only appear for a single frame
				// so create an event that keeps it on the screen for some frames
				Position pos = u->getPosition();
				Error lastErr = Broodwar->getLastError();
				Broodwar->registerEvent([pos, lastErr](Game*){ Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
					nullptr,    // condition
					Broodwar->getLatencyFrames());  // frames to run

				// Retrieve the supply provider type in the case that we have run out of supplies
				UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
				static int lastChecked = 0;

				// If we are supply blocked and haven't tried constructing more recently
				if (lastErr == Errors::Insufficient_Supply &&
					lastChecked + 400 < Broodwar->getFrameCount() &&
					Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
				{
					lastChecked = Broodwar->getFrameCount();

					// Retrieve a unit that is capable of constructing the supply needed
					Unit supplyBuilder = u->getClosestUnit(GetType == supplyProviderType.whatBuilds().first &&
						(IsIdle || IsGatheringMinerals) && IsOwned);
					// If a unit was found
					if (supplyBuilder) {
						constructBuilding(supplyProviderType, supplyBuilder);
					} // closure: insufficient supply
				} // closure: failed to train idle unit
			}
		} // closure: resoruceDepot

	} // closure: unit iterator
}

void ExampleAIModule::onSendText(std::string text) {

	
		// Send the text to the game if it is not being processed.
		Broodwar->sendText("%s", text.c_str());
	


	// Make sure to use %s and pass the text as a parameter,
	// otherwise you may run into problems when you use the %(percent) character!

}

void ExampleAIModule::onReceiveText(BWAPI::Player player, std::string text)
{
	// Parse the received text
	Broodwar << player->getName() << " said \"" << text << "\"" << std::endl;
}

void ExampleAIModule::onPlayerLeft(BWAPI::Player player)
{
	// Interact verbally with the other players in the game by
	// announcing that the other player has left.
	Broodwar->sendText("Goodbye %s!", player->getName().c_str());
}

void ExampleAIModule::onNukeDetect(BWAPI::Position target)
{

	// Check if the target is a valid position
	if (target)
	{
		// if so, print the location of the nuclear strike target
		Broodwar << "Nuclear Launch Detected at " << target << std::endl;
	}
	else
	{
		// Otherwise, ask other players where the nuke is!
		Broodwar->sendText("Where's the nuke?");
	}

	// You can also retrieve all the nuclear missile targets using Broodwar->getNukeDots()!
}

void ExampleAIModule::onUnitDiscover(BWAPI::Unit unit) {

	// scouting code


}

void ExampleAIModule::onUnitEvade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitShow(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitHide(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitCreate(BWAPI::Unit unit) {

	releaseMinerals(unit);

	if (unit->getType().isWorker()) {
		workers++;
	}

	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s creates a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit)
{
}

void ExampleAIModule::onUnitMorph(BWAPI::Unit unit) {

	releaseMinerals(unit);

	if (Broodwar->isReplay())
	{
		// if we are in a replay, then we will print out the build order of the structures
		if (unit->getType().isBuilding() && !unit->getPlayer()->isNeutral())
		{
			int seconds = Broodwar->getFrameCount() / 24;
			int minutes = seconds / 60;
			seconds %= 60;
			Broodwar->sendText("%.2d:%.2d: %s morphs a %s", minutes, seconds, unit->getPlayer()->getName().c_str(), unit->getType().c_str());
		}
	}
}

void ExampleAIModule::onUnitRenegade(BWAPI::Unit unit)
{
}

void ExampleAIModule::onSaveGame(std::string gameName)
{
	Broodwar << "The game was saved to \"" << gameName << "\"" << std::endl;
}

void ExampleAIModule::onUnitComplete(BWAPI::Unit unit)
{

	if (unit->getType().isRefinery()){
		refineryFinished = true;
	}

}


void ExampleAIModule::releaseMinerals(BWAPI::Unit unit){
	if (unit->getType().isBuilding()) {
		int mineralPrice = unit->getType().mineralPrice();
		std::vector<int>::iterator it; 		// iteratation vector
		for (it = pendingBuildings.begin(); it != pendingBuildings.end(); it++){
			// remove the price if it is found in the vector
			if (*it == mineralPrice){
				mineralsReserved -= mineralPrice;
				pendingBuildings.erase(it);
				// break the loop to make sure no dublicates are removed
				break;
			}
		}
	}
}

void ExampleAIModule::constructBuilding(BWAPI::UnitType buildingType, BWAPI::Unit worker){
	int buildingPrice = buildingType.mineralPrice();
	TilePosition targetBuildLocation = Broodwar->getBuildLocation(buildingType,
		worker->getTilePosition());
	if (targetBuildLocation) {
		// Register an event that draws the target build location
		Broodwar->registerEvent([targetBuildLocation, buildingType](Game*) {
			Broodwar->drawBoxMap(Position(targetBuildLocation),
				Position(targetBuildLocation + buildingType.tileSize()),
				Colors::Yellow);
		},
			nullptr,  // condition
			buildingType.buildTime() + 100);  // frames to run

		// Order the worker to construct the structure
		worker->build(buildingType, targetBuildLocation);
		mineralsReserved += buildingPrice;
		pendingBuildings.push_back(buildingPrice);
	}
}

void ExampleAIModule::supplyCheckAndBuild(BWAPI::Unit worker){
	static int lastChecked = 0;
	UnitType supplyType = UnitTypes::Protoss_Pylon;
	if (((Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed()) / 2 < 4
		|| Broodwar->self()->supplyUsed() < 17)
		&& lastChecked + 400 < Broodwar->getFrameCount()
		&& Broodwar->self()->minerals() >= supplyType.mineralPrice()
		&& Broodwar->self()->incompleteUnitCount(supplyType) == 0) {

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(supplyType, worker);

		if (firstPylon){
			scout = worker;
			goScout(worker);
		}
	}
}

void ExampleAIModule::goScout(BWAPI::Unit scout){

	//BWTA::BaseLocation enemyBase = BWTA::getBaseLocations
	baseLocations = BWTA::getStartLocations();
	
	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = baseLocations.begin(); it != baseLocations.end(); ++it){
		// remove the price if it is found in the vector
		if (*it == BWTA::getStartLocation(Broodwar->self())){
			baseLocations.erase(it);
		}
		else {
			scout->move(BWTA::getNearestBaseLocation(scout->getPosition())->getPosition());
			
		}

		

		//if (*it == BWTA::getNearestBaseLocation(scout->getPosition())){}
	}

	

	


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
