#pragma once
class CUserCmd;

namespace AntiAim
{
	void StartAA(CUserCmd * cmd, bool& bSendPacket);
	void AntiAimYaw(CUserCmd * cmd, bool& bSendPacket);
	void AntiAimPitch(CUserCmd * cmd, bool& bSendPacket);
	void fixMoveStart(CUserCmd * cmd);
	void fixMoveEnd(CUserCmd * cmd);
}
