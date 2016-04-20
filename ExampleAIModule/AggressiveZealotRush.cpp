#include "AggressiveZealotRush.h"


AggressiveZealotRush::AggressiveZealotRush() {

	buildOrder = { probe, probe, probe, probe, pylon, // 8
		probe, gateway, gateway, //2 gateways on 9
		probe, probe, zealot, pylon, // 13
		zealot, zealot, zealot, pylon,
		zealot, zealot, zealot, pylon };

}



