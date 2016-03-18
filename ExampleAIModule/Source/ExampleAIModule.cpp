#include "ExampleAIModule.h"
#include <iostream>
#include <vector>
#include <BWTA.h>
#include <windows.h>

using namespace BWAPI;
using namespace Filter;

bool analyzed;
bool analysis_just_finished;


void ExampleAIModule::onStart() {

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
	if (Broodwar->isReplay()) {

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

	// Initializing variables

	//Basic game variables
	mineralsReserved = 0;
	refineryWorkers = 0;
	scout = NULL;
	builder = NULL;
	scouting = true;
	building = false;
	pendingBuildings;
	enemyUnits;
	ourZealots;


	// tactic variables
	zealot_rush = true;

	// zealot rush variables
	if (zealot_rush){
		zealotMAX = 6;

	}

	// Upgrade level variables
	ground_weapons_count = 0;
	ground_armor_count = 0;
	air_weapons_count = 0;
	air_armor_count = 0;
	plasma_shields_count = 0;
	singularity_Charge_Researched = false;

	// BWTA
	BWTA::readMap();
	BWTA::analyze();
	analyzed = false;
	analysis_just_finished = false;
	ourBase = BWTA::getStartLocation(Broodwar->self());
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

void ExampleAIModule::onFrame() {




	if (Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Zealot) >= zealotMAX){
		for (BWAPI::Unit zealot : ourZealots){
			zealot->attack(enemyBase->getPosition());
		}
		zealotMAX = zealotMAX * 2;
	}

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


		if (u->getType() == UnitTypes::Protoss_Forge
			&& u->isCompleted()){

			researchForge(u);

		}
		else if (u->getType() == UnitTypes::Protoss_Cybernetics_Core
			&& u->isCompleted()){

			researchCybernetics_Core(u);

		}



		if (u->getType() == UnitTypes::Protoss_Gateway
			&& zealot_rush){
			trainZealots(u);
		}
		if (u->getType() == UnitTypes::Protoss_Zealot && u->isIdle()) {

			//TODO: Find en god radius til det her
			u->attack(u->getClosestUnit((Filter::IsEnemy && Filter::CanAttack), 50));
		}

		if (u->getType() == UnitTypes::Protoss_Zealot && u->isUnderAttack()) {
			u->attack(u->getClosestUnit((Filter::IsEnemy && Filter::CanAttack), 4));
		}

		if (u->getType().isWorker() && u == scout && u->isUnderAttack()) {
			scout->move(ourBase->getPosition());
			scouting = false;
			scout = NULL;
		}


		// If the unit is a worker unit
		if (u->getType().isWorker()
			&& u != scout
			&& !isGasWorker(u)) {

			if (builder == NULL){
				builder = u;
			}

			// Method that checks for builds
			
			if (!building){
				callBuildFunctions(builder);
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

					if (Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Assimilator) == 1
						&& refineryWorkers < 2) {
						Broodwar << "go gather gas!" << std::endl;
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

			int workerCount = Broodwar->self()->allUnitCount(UnitTypes::Protoss_Probe);
			int gatewayCount = Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway);

			// Order the depot to construct more workers! But only when it is idle and there is below 25 workers.
			if (Broodwar->self()->minerals() - mineralsReserved >= UnitTypes::Protoss_Probe.mineralPrice()
				&& u->isIdle()
				&& workerCount < 25
				&& (!(workerCount >= 10 && gatewayCount == 0) || !(workerCount >= 12 && gatewayCount == 1))
				&& !u->train(u->getType().getRace().getWorker())
				){

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

	if (text == "incomplete zealots"){
		Broodwar << Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Zealot) << std::endl;
	}
	else if (text == "completed zealots"){
		Broodwar << Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Zealot) << std::endl;
	}
	else if (text == "assimilator"){
		Broodwar << Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Assimilator) << std::endl;
	}
	else if (text == "scout"){
		if (scout != NULL){
			Broodwar << scout->exists() << std::endl;
		}
	}
	else if (text == "reserved"){
		Broodwar << mineralsReserved << std::endl;
	}


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

	if (scout != NULL){
		if (scouting
			&& unit->getType().isResourceDepot()
			&& BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()
			&& unit->getPlayer()->isEnemy(Broodwar->self())){

			enemyBase = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			scout->attack(unit);
			scouting = false;
			Broodwar->sendText("Done scouting, found mainbase");

		}

		else if (unit->getPlayer() != Broodwar->self() && unit->getType().isResourceDepot()
			&& !BWTA::getNearestBaseLocation(unit->getPosition())->isStartLocation()) {

			expansion = BWTA::getNearestBaseLocation(unit->getPosition());
			scout->move(unit->getPosition());
			Broodwar->sendText("Found expansion");
			goScout(scout);
		}
	}



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


	if (unit->getType().isBuilding()){
		building = false;
	}

	if (unit->getType() == UnitTypes::Protoss_Zealot){
		Broodwar << "Zealots in training are: " << Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Zealot) << std::endl;
		Broodwar << "Zealots done are:  " << Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Zealot) << std::endl;
	}

	releaseMinerals(unit);

	if (scouting && scout != NULL && unit->getType() == UnitTypes::Protoss_Pylon
		&& Broodwar->self()->supplyTotal() < 19){
		goScout(scout);
	}

	if (Broodwar->isReplay()) {
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

void ExampleAIModule::onUnitDestroy(BWAPI::Unit unit) {

	Broodwar << unit->getPlayer()->getName() << " Lost a unit" << std::endl;
	Broodwar << unit->getType().getName() << " was destryoed!" << std::endl;

	if (unit->getPlayer()->isEnemy(Broodwar->self())){
		//releaseFromList(unit, enemyUnits);
	}

	if (unit->getType() == BWAPI::UnitTypes::Protoss_Zealot
		&& unit->getPlayer() == Broodwar->self()){
		releaseFromList(unit, ourZealots);
	}

	if (unit == scout){
		scouting = false;

		scout = NULL;
	}

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

	if (unit->getType() == UnitTypes::Protoss_Zealot
		&& unit->getPlayer() == Broodwar->self()){
		ourZealots.push_back(unit);
	}

	if (unit->getType() == UnitTypes::Protoss_Gateway){
		unit->setRallyPoint(BWTA::getNearestChokepoint(ourBase->getPosition())->getCenter());
	}


}


// Kasper
void ExampleAIModule::trainZealots(BWAPI::Unit gateway){

	BWAPI::UnitType zealot = BWAPI::UnitTypes::Protoss_Zealot;
	int zealotPrice = zealot.mineralPrice();

	if (gateway->isIdle()
		&& Broodwar->self()->minerals() - mineralsReserved >= zealotPrice
		&& (Broodwar->self()->incompleteUnitCount(zealot) + Broodwar->self()->completedUnitCount(zealot)) < zealotMAX
		&& Broodwar->self()->supplyUsed() < Broodwar->self()->supplyTotal()){

		gateway->train(zealot);
	}


}


// Kasper
void ExampleAIModule::releaseFromList(BWAPI::Unit unit, std::vector<BWAPI::Unit> list){

	std::vector<BWAPI::Unit>::iterator it; 		// iteratation vector
	for (it = list.begin(); it != list.end(); it++){
		// remove the price if it is found in the vector
		if (*it == unit){
			list.erase(it);
			break;
		}
	}

}


// Kasper
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

// Kasper
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




		//TODO Implement at den releaser den fra listen igen.
		worker->build(buildingType, targetBuildLocation);
		building = true;
		mineralsReserved += buildingPrice;
		pendingBuildings.push_back(buildingPrice);









	}
}

//Kasper
void ExampleAIModule::callBuildFunctions(BWAPI::Unit worker){
	buildGateway(worker);
	buildSupply(worker);
	buildForge(worker);
	buildRefinery(worker);
	buildCitadel_Of_Adun(worker);
	buildCybernetics_Core(worker);
	buildPhoton_Cannon(worker);
}


// Kasper
void ExampleAIModule::buildGateway(BWAPI::Unit worker){

	static int lastChecked = 0;
	UnitType gateway = UnitTypes::Protoss_Gateway;
	if (zealot_rush
		&& ((Broodwar->self()->supplyUsed() / 2 >= 10 && Broodwar->self()->allUnitCount(gateway) == 0)
		|| (Broodwar->self()->supplyUsed() / 2 >= 12 && Broodwar->self()->allUnitCount(gateway) == 1))
		&& Broodwar->self()->minerals() - mineralsReserved >= gateway.mineralPrice()
		&& lastChecked + 400 < Broodwar->getFrameCount()
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(gateway, worker);
	}

}


// Kasper
void ExampleAIModule::buildSupply(BWAPI::Unit worker){
	static int lastChecked = 0;
	UnitType supplyType = UnitTypes::Protoss_Pylon;
	if (((Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed()) / 2 < 5
		|| Broodwar->self()->supplyUsed() < 17)
		&& lastChecked + 400 < Broodwar->getFrameCount()
		&& Broodwar->self()->minerals() - mineralsReserved >= supplyType.mineralPrice()
		&& Broodwar->self()->incompleteUnitCount(supplyType) == 0) {

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(supplyType, worker);

		if (Broodwar->self()->supplyTotal() < 19){
			scout = worker;
			for (auto &u : Broodwar->self()->getUnits()) {
				if (u->getType().isWorker()
					&& u != scout){
					builder = u;
				}
			}





		}
	}
}


// Kasper
void ExampleAIModule::buildRefinery(BWAPI::Unit worker){

	static int lastChecked = 0;
	UnitType refineryType = UnitTypes::Protoss_Assimilator;
	if (Broodwar->self()->allUnitCount(refineryType) == 0
		&& Broodwar->self()->minerals() >= refineryType.mineralPrice() + mineralsReserved
		&& (Broodwar->self()->supplyUsed() / 2) > 12
		&& lastChecked + 400 < Broodwar->getFrameCount()
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(refineryType, worker);

	}

}

// Kasper
void ExampleAIModule::buildCybernetics_Core(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType cybernetics_Core = UnitTypes::Protoss_Cybernetics_Core;

	// Amount of gateways (including ones in construction)
	int gatewayCount = Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Gateway)
		+ Broodwar->self()->incompleteUnitCount(UnitTypes::Protoss_Gateway);


	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(cybernetics_Core) == 0
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Gateway) >= 1
		&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
		&& Broodwar->self()->minerals() >= cybernetics_Core.mineralPrice()
		&& Broodwar->self()->supplyUsed() / 2 >= 15
		//TODO: Tactic
		) {

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(cybernetics_Core, worker);

	}
}

// Kasper
void ExampleAIModule::buildPhoton_Cannon(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType photon_cannon = UnitTypes::Protoss_Photon_Cannon;

	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(photon_cannon) < 5
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Forge) >= 1
		&& Broodwar->self()->minerals() >= photon_cannon.mineralPrice() + 400
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(photon_cannon, worker);

	}

}

// Kasper
void ExampleAIModule::buildCitadel_Of_Adun(BWAPI::Unit worker){

	static int lastChecked = 0;
	BWAPI::UnitType citadel_Of_Adun = UnitTypes::Protoss_Citadel_of_Adun;

	if (lastChecked + 600 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(citadel_Of_Adun) == 0
		&& Broodwar->self()->completedUnitCount(UnitTypes::Protoss_Cybernetics_Core)
		&& Broodwar->self()->minerals() >= citadel_Of_Adun.mineralPrice()
		&& Broodwar->self()->gas() >= citadel_Of_Adun.gasPrice()
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(citadel_Of_Adun, worker);

	}
}

// Kasper
void ExampleAIModule::buildForge(BWAPI::Unit worker){


	BWAPI::UnitType forge = UnitTypes::Protoss_Forge;
	static int lastChecked = 0;


	if (Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
		&& Broodwar->self()->minerals() >= forge.mineralPrice() + 100
		&& lastChecked + 400 < Broodwar->getFrameCount()
		&& Broodwar->self()->allUnitCount(forge) == 0
		//TODO: Tactic
		){

		lastChecked = Broodwar->getFrameCount();
		constructBuilding(forge, worker);

	}

}

//Kasper
void ExampleAIModule::researchForge(BWAPI::Unit forge){

	if (forge->isIdle()){

		// Ground weapons
		BWAPI::UpgradeType ground_weapons = BWAPI::UpgradeTypes::Protoss_Ground_Weapons;
		int ground_weapons_mineral_price = ground_weapons.mineralPrice()
			+ (ground_weapons.mineralPriceFactor() * ground_weapons_count);
		int ground_weapons_gas_price = ground_weapons.gasPrice()
			+ (ground_weapons.gasPriceFactor() * ground_weapons_count);

		if (ground_weapons_count == 0
			&& Broodwar->self()->allUnitCount(UnitTypes::Protoss_Gateway) >= 2
			&& Broodwar->self()->minerals() >= ground_weapons_mineral_price + 100
			&& Broodwar->self()->gas() >= ground_weapons_gas_price
			//TODO: Tactic
			){

			forge->upgrade(ground_weapons);
			ground_weapons_count++;

		}

		// Ground armor
		BWAPI::UpgradeType ground_armor = BWAPI::UpgradeTypes::Protoss_Ground_Armor;
		int ground_armor_mineral_price = ground_armor.mineralPrice()
			+ (ground_armor.mineralPriceFactor() * ground_armor_count);
		int ground_armor_gas_price = ground_armor.gasPrice()
			+ (ground_armor.gasPriceFactor() * ground_armor_count);

		if (ground_weapons_count >= 1
			&& Broodwar->self()->minerals() >= ground_armor_mineral_price + 100
			&& Broodwar->self()->gas() >= ground_armor_gas_price
			//TODO: Tactic
			){

			forge->upgrade(ground_armor);
			ground_armor_count++;

		}

		// Plasma shields
		BWAPI::UpgradeType plasma_shields = BWAPI::UpgradeTypes::Protoss_Plasma_Shields;
		int plasma_shields_mineral_price = plasma_shields.mineralPrice()
			+ (plasma_shields.mineralPriceFactor() * plasma_shields_count);
		int plasma_shields_gas_price = plasma_shields.gasPrice()
			+ (plasma_shields.gasPriceFactor() * plasma_shields_count);

		if (ground_armor_count >= 1
			&& Broodwar->self()->minerals() >= plasma_shields_mineral_price + 100
			&& Broodwar->self()->gas() >= plasma_shields_gas_price
			//TODO: Tactic
			){

			forge->upgrade(plasma_shields);
			plasma_shields_count++;

		}

	}

}

// Kasper
void ExampleAIModule::researchCybernetics_Core(BWAPI::Unit cybernetics_Core){

	if (cybernetics_Core->isIdle()){

		// Air weapons
		BWAPI::UpgradeType air_weapons = BWAPI::UpgradeTypes::Protoss_Air_Weapons;
		int air_weapons_mineral_price = air_weapons.mineralPrice()
			+ (air_weapons.mineralPriceFactor() * air_weapons_count);
		int air_weapons_gas_price = air_weapons.gasPrice()
			+ (air_weapons.gasPriceFactor() * air_weapons_count);

		if (FALSE
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(air_weapons);
			air_weapons_count++;

		}

		// Air armor
		BWAPI::UpgradeType air_armor = BWAPI::UpgradeTypes::Protoss_Air_Armor;
		int air_armor_mineral_price = air_armor.mineralPrice()
			+ (air_armor.mineralPriceFactor() * air_armor_count);
		int air_armor_gas_price = air_armor.gasPrice()
			+ (air_armor.gasPriceFactor() * air_armor_count);

		if (FALSE
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(air_armor);
			air_armor_count++;

		}

		// Singularity_Charge
		BWAPI::UpgradeType singularity_charge = BWAPI::UpgradeTypes::Singularity_Charge;

		if (!singularity_Charge_Researched
			&& Broodwar->self()->minerals() >= singularity_charge.mineralPrice() + 400
			&& Broodwar->self()->gas() >= singularity_charge.gasPrice() + 200
			//TODO: Tactic
			){

			cybernetics_Core->upgrade(singularity_charge);
			singularity_charge = true;

		}
	}


}

//Kasper
bool ExampleAIModule::isGasWorker(BWAPI::Unit worker){

	if (worker->isCarryingGas()
		|| worker->isGatheringGas()
		|| worker->getOrderTargetPosition() == ourBase->getGeysers().getPosition()){
		return true;
	}
	else {
		return false;
	}

}


void ExampleAIModule::goScout(BWAPI::Unit scout){
	baseLocations = BWTA::getStartLocations();
	double minDistance = 9999999;

	std::set<BWTA::BaseLocation*>::iterator it; 		// iteratation set
	for (it = baseLocations.begin(); it != baseLocations.end(); ++it){
		scoutedBase = *it;
		// Remove our baselocation
		if (scoutedBase->getPosition() == ourBase->getPosition() || scout->getPosition() == scoutedBase->getPosition()){
			baseLocations.erase(*it);
		}
		else if (scout->getDistance(scoutedBase->getPosition()) < minDistance) {
			minDistance = scout->getDistance(scoutedBase->getPosition());
			scout->move(scoutedBase->getPosition());
		}
		
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
