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

void BunnyHop::ClantagChanger(const char* ClanTag)
{
	Utils::SetClantag((const char*)ClanTag);
}

void BunnyHop::NameChanger(const char* name)
{
	Utils::SetName((const char*)name);
}