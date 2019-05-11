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

void AntiAim::StartAA(CUserCmd * cmd, bool & bSendPacket) //Przyklady AA https://github.com/A5-/Gamerfood_CSGO/blob/a7456f5e5d163b8fe7d51b8eb80a95a013feb138/csgo_internal/csgo_internal/AntiAim.cpp
{
	
	if (g_LocalPlayer->m_nMoveType() != MOVETYPE_LADDER && g_LocalPlayer->m_nMoveType() != MOVETYPE_NOCLIP && !(cmd->buttons & IN_ATTACK) )  //Movetype zwraca zawsze 0
	{
		fixMoveStart(cmd);

		AntiAimPitch(cmd, bSendPacket);
		AntiAimYaw(cmd, bSendPacket);

		fixMoveEnd(cmd);
	}
}

void AntiAim::AntiAimYaw(CUserCmd * cmd, bool & bSendPacket)
{
	static int ChockedPackets = -1;
	ChockedPackets++;
	float server_time = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;
	static bool Flip;
	static bool Flip2;
	Flip = !Flip;
	Flip2 = !Flip2;
	QAngle ViewANG;

	switch (g_Options.AntiAim_AntiAimTypeYaw)
	{
	case 0: //Legit
		if (ChockedPackets < 1)
			bSendPacket = true;
		else
		{
			bSendPacket = false;
			Flip ? ViewANG.yaw += 90.0f : ViewANG.yaw -= 90.0f;
			ChockedPackets = -1;
		}
		Flip != Flip;
		break;
	case 1: //Spinbot
		bSendPacket = false;
		ViewANG.yaw = (float)(fmod(server_time / 1.5f * 360.0f * (g_Options.AntiAim_SpinBotSpeed / 2), 360.0f));
		bSendPacket = true;
		break;
	case 2: // Jitter
		bSendPacket = false;
		if (Flip)
			ViewANG.yaw -= 270.0f;
		else
			ViewANG.yaw -= 90.0f;
		bSendPacket = true;
		break;
	case 3: //SideWays
		bSendPacket = false;
		if (Flip)
			ViewANG.yaw += 90.0f;
		else
			ViewANG.yaw -= 90.0f;
		bSendPacket = true;
		break;
	case 4: //Fake SideWays
		if (Flip)
		{
			ViewANG.yaw += 90.0f;
			bSendPacket = false;
		}
		else
		{
			ViewANG.yaw -= 90.0f;
			bSendPacket = true;
		}
	case 5: //Static
		if (Flip)
		{
			if (Flip2)
				ViewANG.yaw += 90.0f;
			else
				ViewANG.yaw -= 90.0f;
		}
		else
		{
			ViewANG.yaw += 180.0f;
		}
		break;
	case 6: //Fake Static
		if (Flip)
		{
			bSendPacket = false;

			if (Flip2)
				ViewANG.yaw += 90.0f;

			else
				ViewANG.yaw -= 90.0f;
		}
		else
		{
			bSendPacket = true;
			ViewANG.yaw += 180.0f;
		}
	case 7: //Custom
		bSendPacket = false;
		ViewANG.yaw += g_Options.AntiAim_AntiAimyaw;
		bSendPacket = true;
		break;
	default:
		break;
	}
	ViewANG.Clamp();
	cmd->viewangles = ViewANG;
}

void AntiAim::AntiAimPitch(CUserCmd * cmd, bool & bSendPacket)
{
	static bool Fliper;
	Fliper = !Fliper;
	QAngle ViewANG;
	switch (g_Options.AntiAim_AntiAimTypePitch)
	{
	case 0:
		bSendPacket = false;
		ViewANG.pitch = -89.0f;
		bSendPacket = true;
		break;
	case 1:
		bSendPacket = false;
		ViewANG.pitch = 0.0f;
		bSendPacket = true;
		break;
	case 2:
		bSendPacket = false;
		ViewANG.pitch = 89.0f;
		bSendPacket = true;
		break;
	case 3:
		if (Fliper) {
			ViewANG.pitch = -89.0f;
			bSendPacket = false;
		}
		else {
			ViewANG.pitch = 89.0f;
			bSendPacket = true;
		}
	case 4:
		bSendPacket = false;
		ViewANG.pitch = g_Options.AntiAim_AntiAimpitch;
		bSendPacket = true;
		break;
	default:
		break;
	}
	ViewANG.Clamp();
	cmd->viewangles = ViewANG;
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
