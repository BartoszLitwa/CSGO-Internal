#include "BackTrack.h"
#include "features/Aimbot.hpp"
#include "../CSGOSimple/options.hpp"

BackTracking Backtrack;

void BackTracking::BackTrackStart(CUserCmd* cmd)
{
	C_BasePlayer* local = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
	if (!local)
		return;

	for (int i = 0; i < 64; i++)
	{
		C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (entity && entity->IsAlive() && entity->m_iTeamNum() != local->m_iTeamNum() && !entity->IsDormant())
		{
			tick[i].insert(tick[i].begin(), Tick(cmd->tick_count, entity->GetBonePos(8)));

			if (tick[i].size() > g_Options.Aimbot_BackTrackTicks)
			{
				tick[i].pop_back();
			}
		}
	}

}

void BackTracking::BackTrackingCalc(CUserCmd* cmd)
{
	C_BasePlayer* local = (C_BasePlayer*)g_EntityList->GetClientEntity(g_EngineClient->GetLocalPlayer());
	if (!local)
		return;
	QAngle Viewangles;
	g_EngineClient->GetViewAngles(&Viewangles);
	Viewangles += local->m_aimPunchAngle() * 2;
	float tempdelta = FLT_MAX;
	int ClosestPlayer = -1;
	for (int i = 0; i < 64; i++)
	{
		C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (entity && entity->IsAlive() && entity->m_iTeamNum() != local->m_iTeamNum() && !entity->IsDormant())
		{
			QAngle Enemyangle = Math::CalcAngle(local->GetEyePos(), entity->GetEyePos()) - Viewangles;
			float delta = (Enemyangle.Clamp().Length());
			if (delta < tempdelta)
			{
				ClosestPlayer = i;
				tempdelta = delta;
			}
		}
	}

	int index = -1;
	if (ClosestPlayer != -1)
	{
		for (int i = 0; i < tick[ClosestPlayer].size(); i++)
		{
				QAngle Enemyangle = Math::CalcAngle(local->GetEyePos(), tick[ClosestPlayer].at(i).Head) -Viewangles;
				float delta = Enemyangle.Clamp().Length();
				if (delta < tempdelta)
				{
					index = i;
					tempdelta = delta;
				}

		}

		if (index != -1 && cmd->buttons & IN_ATTACK)
		{
			cmd->tick_count = tick[ClosestPlayer].at(index).tick;
		}
	}
}