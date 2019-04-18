#include "bhop.hpp"

#include "../valve_sdk/csgostructs.hpp"

void BunnyHop::OnCreateMove(CUserCmd* cmd)
{
  static bool jumped_last_tick = false;
  static bool should_fake_jump = false;

  if(!jumped_last_tick && should_fake_jump) {
    should_fake_jump = false;
    cmd->buttons |= IN_JUMP;
  } else if(cmd->buttons & IN_JUMP) {
    if(g_LocalPlayer->m_fFlags() & FL_ONGROUND) {
      jumped_last_tick = true;
      should_fake_jump = true;
    } else {
      cmd->buttons &= ~IN_JUMP;
      jumped_last_tick = false;
    }
  } else {
    jumped_last_tick = false;
    should_fake_jump = false;
  }
}

void BunnyHop::UnlimitedDuck(CUserCmd * cmd)
{
	cmd->buttons |= IN_BULLRUSH;
}

void BunnyHop::AutoStrafe(CUserCmd * cmd)
{
	if (g_LocalPlayer->m_fFlags() & FL_ONGROUND)
		return;

	if (cmd->mousedx > 1 || cmd->mousedx < -1)
		cmd->sidemove = cmd->mousedx < 0.0f ? -400.0f : 400.0f;
}

void BunnyHop::AirStuck(CUserCmd * cmd)
{
	if (cmd->buttons & IN_ATTACK)
		return;

	if (GetAsyncKeyState(VK_F2))
		cmd->tick_count = 16777216;
}

void BunnyHop::ClantagChanger(const char* ClanTag)
{
	Utils::SetClantag((const char*)ClanTag);
}

void BunnyHop::NameChanger(const char* name)
{
	Utils::SetName((const char*)name);
}

size_t linenum = 0;

void BunnyHop::ChatSpam()
{
	//static float nextTime = 0.f;
	//float flServerTime = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;

	//if (nextTime > flServerTime)
	//	return;

	//nextTime = flServerTime + 0.5f;

	//if (m_spamlines.empty())
	//	return;

	///*if (Vars.Misc.ChatSpamMode == 0)*/
	//	linenum = rand() % m_spamlines.size() - 1;

	//char str[512];
	//strcpy(str, charenc("say "));
	//strcat(str, m_spamlines[linenum].c_str());

	//g_EngineClient->ClientCmd_Unrestricted(str);

	//if (Vars.Misc.ChatSpamMode == 1)
	//{
	//	linenum++;

	//	if (linenum >= m_spamlines.size())
	//		linenum = 0;
	//}
}
