#include "Strategy.h"

Strategy::Strategy(int strategy){
	if (strategy == strategies::justSomeStrategy){
		theFirstStrat();
		name = "justSomeStrategy";
		starter = true;
	}
}


void Strategy::theFirstStrat(){
	
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			// 8 - pylon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(gateway));			// 10 - gateway
	strategyQueue.push_back(BuildOrderType(scoutRequest));		// 10 - scout
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(assimilator));		//12 - Assimilator
	strategyQueue.push_back(BuildOrderType(gasworkerRequest)); //12 - Gasworker request
	strategyQueue.push_back(BuildOrderType(gasworkerRequest)); //12 - Gasworker request
	strategyQueue.push_back(BuildOrderType(gasworkerRequest)); //12 - Gasworker request
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			//13 - Pylon
	strategyQueue.push_back(BuildOrderType(zealot));			//13 - Zealot
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			//16 - Pylon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(cybercore));		//18 - Cybernetics Core
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			//19 - Pylon
	strategyQueue.push_back(BuildOrderType(zealot));			//19 - Zealot
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			//21 - Pylon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(dragoon));			//23 - Dragoon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));			//25 - Pylon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(citadel));			//26 - Citadel
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(dragoon));			//27 - Dragoon
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(gateway));			//29 - Gateway
	strategyQueue.push_back(BuildOrderType(pylon));			//29 - Pylon
	strategyQueue.push_back(BuildOrderType(archives));			//29 - Templar Archives
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(dtemplar));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(dtemplar));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(forge));
	strategyQueue.push_back(BuildOrderType(cannon));
	strategyQueue.push_back(BuildOrderType(cannon));
	strategyQueue.push_back(BuildOrderType(cannon));

}