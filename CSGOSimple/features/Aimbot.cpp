#include "Aimbot.hpp"
#include "../options.hpp"
#include "../helpers/utils.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/interfaces/CClientState.hpp"
#include "../valve_sdk/interfaces/IVEngineClient.hpp"
#include "../Autowall.h"
#include "../BackTrack.h"

C_BasePlayer * Aimbot::GetClosestEnemy()
{
	QAngle ViewAngles;
	g_EngineClient->GetViewAngles(&ViewAngles);
	C_BasePlayer* BestpEntity = nullptr;
	float bestDelta = FLT_MAX;
	for (int i = 0; i <= g_EngineClient->GetMaxClients(); i++)
	{
		C_BasePlayer* pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
		if (pEntity && !pEntity->IsDormant() && pEntity->IsAlive() && pEntity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() && !pEntity->m_bGunGameImmunity())
		{
			if (g_Options.Aimbot_VisibilityCheck)
			{
				if (!g_LocalPlayer->CanSeePlayer(pEntity, g_Options.Aimbot_Bone))
					break;
			}

			QAngle EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(g_Options.Aimbot_Bone));
			float delta = Math::GetFOV(ViewAngles - g_LocalPlayer->m_aimPunchAngle() * 2, EnemyAng);    //(EnemyAng - (g_LocalPlayer->m_aimPunchAngle() * 2) - ViewAngles).Clamped().Length();
			if (delta < bestDelta && delta <= g_Options.Aimbot_AimbotFov)
			{
				bestDelta = delta;
				BestpEntity = pEntity;
			}
		}
	}
	return BestpEntity;
}

void Smooth(QAngle& viewangle, QAngle & angle, float smoothValue)
{
	angle = (viewangle + (angle - viewangle).Clamped() / smoothValue).Clamped();
}

void Aimbot::Aim(CUserCmd* cmd, bool& bSendPacket)
{
	if (g_LocalPlayer->IsAlive() && g_LocalPlayer->m_hActiveWeapon()->IsGun()) {
		g_Options.AimbotCanShootWhileAAON = false;
		if (cmd->buttons & IN_ATTACK && !g_Options.Aimbot_AutoWall)
		{
			QAngle ViewAngles;
			g_EngineClient->GetViewAngles(&ViewAngles);
			C_BasePlayer* entity = GetClosestEnemy();
			if (entity)
			{
				/*char array[10];
				sprintf(array, "%f", (Autowall::GetDamage(entity->GetHitboxPos(HITBOX_HEAD)) * 2)); //Print to console autowall dmg
				Utils::ConsolePrint(array); Utils::ConsolePrint("\n");*/
				QAngle EnemyAng;
				switch (g_Options.Aimbot_Bone)
				{
				case 0:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_HEAD));
					break;
				case 1:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_NECK));
					break;
				case 2:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_PELVIS));
					break;
				case 3:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_STOMACH));
					break;
				case 4:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LOWER_CHEST));
					break;
				case 5:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_CHEST));
					break;
				case 6:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_UPPER_CHEST));
					break;
				case 7:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_THIGH));
					break;
				case 8:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_THIGH));
					break;
				case 9:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_CALF));
					break;
				case 10:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_CALF));
					break;
				case 11:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_FOOT));
					break;
				case 12:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_FOOT));
					break;
				case 13:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_HAND));
					break;
				case 14:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_HAND));
					break;
				case 15:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_UPPER_ARM));
					break;
				case 16:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_RIGHT_FOREARM));
					break;
				case 17:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_UPPER_ARM));
					break;
				case 18:
					EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetHitboxPos(HITBOX_LEFT_FOREARM));
					break;
				default:
					break;
				}

				/*if (g_Options.AntiAim_AntiAim) {
					if (g_Options.AntiAim_AntiAimType == 1) {
						float server_time = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;
						cmd->viewangles.yaw = (float)(fmod(server_time / 1.5f * 360.0f / g_Options.AntiAim_SpinBotSpeed, 360.0f));
					}
					if (g_Options.AntiAim_AntiAimType == 2) {
						cmd->viewangles.yaw -= g_Options.AntiAim_AntiAimyaw;
						cmd->viewangles.pitch -= g_Options.AntiAim_AntiAimpitch;
					}
				}*/

				if (g_Options.Aimbot_RecoilPrediction)
					EnemyAng -= g_LocalPlayer->m_aimPunchAngle() * 2;

				if (g_Options.Aimbot_Smooth)
					Smooth(ViewAngles, EnemyAng, g_Options.Aimbot_SmoothValue);
				EnemyAng.Clamp();

				if (g_Options.Aimbot_AutoScope && !g_LocalPlayer->m_bIsScoped() && g_LocalPlayer->m_hActiveWeapon()->IsSniper())
					cmd->buttons |= IN_ATTACK2;

				if (g_Options.Aimbot_AutoStop && cmd->buttons & IN_ATTACK) {
					cmd->forwardmove = 0;
					cmd->sidemove = 0;
					cmd->upmove = 0;
					cmd->buttons = 0;
				}

				if (g_Options.Aimbot_AutoCrouch && cmd->buttons & IN_ATTACK)
					cmd->buttons |= IN_DUCK;

				if (!g_Options.Aimbot_Silent) {
					g_EngineClient->SetViewAngles(&EnemyAng);
					cmd->buttons |= IN_ATTACK;
				}
				else {
					cmd->viewangles = EnemyAng;
					cmd->buttons != IN_ATTACK;
				}
			}
		}

		if (g_Options.Aimbot_AutoWall)
		{
			QAngle ViewAngles;
			g_EngineClient->GetViewAngles(&ViewAngles);
			for (int i = 0; i <= g_EngineClient->GetMaxClients(); i++)
			{
				C_BasePlayer* pEntity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
				if (pEntity && !pEntity->IsDormant() && pEntity->IsAlive() && pEntity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum() && !pEntity->m_bGunGameImmunity())
				{
					QAngle EnemyAng;
					bool canshoot = false;
					if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_HEAD)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_HEAD)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_NECK)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_NECK)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_PELVIS)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_PELVIS)); canshoot = true;
					} 
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_STOMACH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_STOMACH)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LOWER_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LOWER_CHEST)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_CHEST)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_UPPER_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_UPPER_CHEST)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_THIGH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_THIGH)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_THIGH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_THIGH)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_CALF)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_CALF)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_CALF)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_CALF)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_FOOT)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_FOOT)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_FOOT)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_FOOT)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_HAND)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_HAND)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_HAND)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_HAND)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_UPPER_ARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_UPPER_ARM)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_FOREARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_UPPER_ARM)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_UPPER_ARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_RIGHT_FOREARM)); canshoot = true;
					}
					else if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_FOREARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg) {
						EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetHitboxPos(HITBOX_LEFT_FOREARM)); canshoot = true;
					}
					if (canshoot)
					{
						
						/*if (!g_Options.Aimbot_AimAtBackTrack)
							EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetBonePos(g_Options.Aimbot_Bone));
						else
							EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetBonePos(8));*/
						/*if (g_Options.AntiAim_AntiAim) {
							if (g_Options.AntiAim_AntiAimType == 1) {
								float server_time = g_LocalPlayer->m_nTickBase() * g_GlobalVars->interval_per_tick;
								cmd->viewangles.yaw = (float)(fmod(server_time / 1.5f * 360.0f / g_Options.AntiAim_SpinBotSpeed, 360.0f));
							}
							if (g_Options.AntiAim_AntiAimType == 2) {
								cmd->viewangles.yaw -= g_Options.AntiAim_AntiAimyaw;
								cmd->viewangles.pitch -= g_Options.AntiAim_AntiAimpitch;
							}
						}*/

						EnemyAng -= g_LocalPlayer->m_aimPunchAngle() * 2;
						EnemyAng.Clamp();
						if (g_LocalPlayer->m_hActiveWeapon()->CanFire()) {

							if (g_Options.Aimbot_AutoScope && !g_LocalPlayer->m_bIsScoped() && g_LocalPlayer->m_hActiveWeapon()->IsSniper())
								cmd->buttons |= IN_ATTACK2;

							if (g_Options.Aimbot_AutoStop && cmd->buttons & IN_ATTACK ) {
								cmd->forwardmove = 0;
								cmd->sidemove = 0;
								cmd->upmove = 0;
								cmd->buttons = 0;
							}

							if (g_Options.Aimbot_AutoCrouch && cmd->buttons & IN_ATTACK)
								cmd->buttons |= IN_DUCK;
							if (!g_Options.Aimbot_Silent) {
								g_EngineClient->SetViewAngles(&EnemyAng);
								cmd->buttons |= IN_ATTACK;
							}
							else {
								cmd->viewangles = EnemyAng;
								cmd->buttons |= IN_ATTACK;
							}
						}
					}
				}
			}
		}
	}
}

