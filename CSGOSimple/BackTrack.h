#pragma once
#include "../CSGOSimple/singleton.hpp"
#include "../CSGOSimple/render.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"

class BackTracking
{
public:
	struct Tick
	{
		Tick(int Tickcount, Vector Headposition)
		{
			tick = Tickcount;
			Head = Headposition;
		}
		int tick = -1;
		Vector Head;
	};
	std::vector<Tick> tick[64];

	void BackTrackStart(CUserCmd* cmd);

	void BackTrackingCalc(CUserCmd * cmd);

private:
	


};

extern BackTracking Backtrack;
