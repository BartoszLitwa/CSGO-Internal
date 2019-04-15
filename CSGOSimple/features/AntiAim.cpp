#include "AntiAim.h"
#include "Aimbot.hpp"
#include "../options.hpp"
#include "../helpers/utils.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/interfaces/CClientState.hpp"
#include "../valve_sdk/interfaces/IVEngineClient.hpp"

QAngle m_oldangle;
float m_oldforward;
float m_oldsidemove;

void AntiAim::LegitAA(CUserCmd * cmd, bool & bSendPacket) //Przyklady AA https://github.com/A5-/Gamerfood_CSGO/blob/a7456f5e5d163b8fe7d51b8eb80a95a013feb138/csgo_internal/csgo_internal/AntiAim.cpp
{
	if (g_Options.AntiAim_AntiAim && g_LocalPlayer->m_nMoveType() != MOVETYPE_LADDER)
	{
		static int ChockedPackets = -1;
		ChockedPackets++;
		float server_time = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;
		static bool Flip;
		static bool Flip2;
		Flip = !Flip;
		Flip2 = !Flip2;

		fixMoveStart(cmd);

		switch (g_Options.AntiAim_AntiAimType)
		{
		case 0:
			if (ChockedPackets < 1)
				bSendPacket = true;
			else
			{
				bSendPacket = false;
				Flip ? cmd->viewangles.yaw += 90.0f : cmd->viewangles.yaw -= 90.0f;
				cmd->viewangles.Clamp();
				ChockedPackets = -1;
			}
			Flip != Flip;
			break;
		case 1:
			bSendPacket = false;
			cmd->viewangles.yaw += 180.0f;
			cmd->viewangles.pitch = 89.0f;
			bSendPacket = true;
			break;
		case 2:
			bSendPacket = true;
			cmd->viewangles.yaw = (float)(fmod(server_time / 1.5f * 360.0f, 360.0f));

			break;
		default:
			break;
		}

		fixMoveEnd(cmd);
	}
}

void AntiAim::fixMoveStart(CUserCmd* cmd)
{
	m_oldangle = cmd->viewangles;
	m_oldforward = cmd->forwardmove;
	m_oldsidemove = cmd->sidemove;
}

void AntiAim::fixMoveEnd(CUserCmd* cmd)
{
	float yaw_delta = cmd->viewangles.yaw - m_oldangle.yaw;
	float f1;
	float f2;

	if (m_oldangle.yaw < 0.f)
		f1 = 360.0f + m_oldangle.yaw;
	else
		f1 = m_oldangle.yaw;

	if (cmd->viewangles.yaw < 0.0f)
		f2 = 360.0f + cmd->viewangles.yaw;
	else
		f2 = cmd->viewangles.yaw;

	if (f2 < f1)
		yaw_delta = abs(f2 - f1);
	else
		yaw_delta = 360.0f - abs(f1 - f2);
	yaw_delta = 360.0f - yaw_delta;

	cmd->forwardmove = cos(DEG2RAD(yaw_delta)) * m_oldforward + cos(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
	cmd->sidemove = sin(DEG2RAD(yaw_delta)) * m_oldforward + sin(DEG2RAD(yaw_delta + 90.f)) * m_oldsidemove;
}
