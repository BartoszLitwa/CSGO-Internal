#include "AntiAim.h"
#include "Aimbot.hpp"
#include "../options.hpp"
#include "../helpers/utils.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/interfaces/CClientState.hpp"
#include "../valve_sdk/interfaces/IVEngineClient.hpp"


void AntiAim::LegitAA(CUserCmd * cmd, bool & bSendPacket)
{
	C_BasePlayer* local = (C_BasePlayer*)g_EngineClient->GetLocalPlayer();

	if ((cmd->buttons & IN_USE) || local->m_nMoveType() == MOVETYPE_LADDER)
		return;

	if (g_Options.AntiAim_AntiAim && !cmd->buttons & IN_USE)
	{
		static int ChockedPackets = -1;
		ChockedPackets++;
		static bool Flip;

		if (ChockedPackets < 1)
			bSendPacket = true;
		else
		{
			bSendPacket = false;
			Flip ? cmd->viewangles.yaw += 90.0f : cmd->viewangles.yaw -= 90.0f;
			ChockedPackets = -1;
		}
		Flip != Flip;
	}
}
