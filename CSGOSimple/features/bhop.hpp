#pragma once

class C_BasePlayer;
class CUserCmd;

namespace BunnyHop
{
	void OnCreateMove(CUserCmd* cmd);
	void UnlimitedDuck(CUserCmd* cmd);
	void AutoStrafe(CUserCmd * cmd);
	void AirStuck(CUserCmd * cmd);
	void ClantagChanger(const char* ClanTag);
	void NameChanger(const char* name);
	void ChatSpam();
}