#pragma once
class CUserCmd;
class C_BasePlayer;
class QAngle;

namespace Aimbot
{
	C_BasePlayer* GetClosestEnemy();
	void Aim(CUserCmd* cmd);
};