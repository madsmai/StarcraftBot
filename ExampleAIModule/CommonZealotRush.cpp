#include "CommonZealotRush.h"


CommonZealotRush::CommonZealotRush(){


	buildOrder = { probe, probe, probe, probe, pylon, // 8
		scoutRequest,
		probe, probe, gateway, // 10
		probe, probe, gateway, // 12
		probe, zealot, probe, pylon, // 16
		zealot, zealot, zealot, pylon, // 22
		zealot, zealot, zealot, pylon };


}
