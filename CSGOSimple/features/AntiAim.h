#pragma once
class CUserCmd;

namespace AntiAim
{
	void LegitAA(CUserCmd * cmd, bool& bSendPacket);
	void fixMoveStart(CUserCmd * cmd);
	void fixMoveEnd(CUserCmd * cmd);
}
