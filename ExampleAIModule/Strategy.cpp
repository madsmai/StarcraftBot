#include "Strategy.h"

Strategy::Strategy(int strategy){

	switch (strategy) {

	case strategies::justSomeStrategy:
		theFirstStrat();
		name = "justSomeStrategy";
		starter = true;
		break;

	case strategies::standardZealotRush:
		initialStrat();
		name = "Standard Zealot Rush 10/12";
		starter = true;
		break;
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

void Strategy::initialStrat(){

	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe)); // 8. Pylon then scout
	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(scoutRequest));

	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe)); // 10. Gateway
	strategyQueue.push_back(BuildOrderType(gateway));

	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(probe)); // 12. Gateway
	strategyQueue.push_back(BuildOrderType(gateway));

	strategyQueue.push_back(BuildOrderType(probe));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(probe)); // 16. Pylon
	strategyQueue.push_back(BuildOrderType(pylon));

	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot)); // First attack
	// TODO: Set a new army size or smth


	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));

	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));

	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));

	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));

	strategyQueue.push_back(BuildOrderType(pylon));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));
	strategyQueue.push_back(BuildOrderType(zealot));

}