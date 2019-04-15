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
				if (!g_LocalPlayer->CanSeePlayer(pEntity, HITBOX_HEAD))
					break;
			}

			QAngle EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetBonePos(g_Options.Aimbot_Bone));
			float delta = (EnemyAng - (g_LocalPlayer->m_aimPunchAngle() * 2) - ViewAngles).Clamped().Length();
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

void Aimbot::Aim(CUserCmd* cmd)
{
	if (g_LocalPlayer->IsAlive() && g_LocalPlayer->m_hActiveWeapon()->IsGun()) {
		if (cmd->buttons & IN_ATTACK && !g_Options.Aimbot_AutoWall)
		{
			QAngle ViewAngles;
			g_EngineClient->GetViewAngles(&ViewAngles);
			C_BasePlayer* entity = GetClosestEnemy();
			if (entity)
			{
				/*char array[10];
				sprintf(array, "%f", (Autowall::GetDamage(entity->GetHitboxPos(HITBOX_HEAD)) * 2));
				Utils::ConsolePrint(array); Utils::ConsolePrint("\n");*/

				QAngle EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), entity->GetBonePos(g_Options.Aimbot_Bone));

				if (g_Options.Aimbot_RecoilPrediction)
					EnemyAng -= g_LocalPlayer->m_aimPunchAngle() * 2;

				if (g_Options.Aimbot_Smooth)
					Smooth(ViewAngles, EnemyAng, g_Options.Aimbot_SmoothValue);

				EnemyAng.Clamp().Normalize();

				if (!g_Options.Aimbot_Silent)
					g_EngineClient->SetViewAngles(&EnemyAng);
				else
					cmd->viewangles = EnemyAng;
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
					if (Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_HEAD)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_NECK)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_PELVIS)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_STOMACH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LOWER_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_UPPER_CHEST)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_THIGH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_THIGH)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_CALF)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_CALF)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_FOOT)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_FOOT)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_HAND)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_HAND)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_UPPER_ARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_RIGHT_FOREARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg || Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_UPPER_ARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg
						|| Autowall::GetDamage(pEntity->GetHitboxPos(HITBOX_LEFT_FOREARM)) * 2 >= g_Options.Aimbot_AutoWallMinDmg)
					{
						QAngle EnemyAng;
						if (!g_Options.Aimbot_AimAtBackTrack)
							EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetBonePos(g_Options.Aimbot_Bone));
						else
							EnemyAng = Math::CalcAngle(g_LocalPlayer->GetEyePos(), pEntity->GetBonePos(8));

						EnemyAng -= g_LocalPlayer->m_aimPunchAngle() * 2;
						EnemyAng.Clamp().Normalize();
						if (g_LocalPlayer->m_hActiveWeapon()->CanFire()) {

							if (g_Options.Aimbot_AutoScope && !g_LocalPlayer->m_bIsScoped() && g_LocalPlayer->m_hActiveWeapon()->IsSniper())
								cmd->buttons = IN_ATTACK2;

							if (g_Options.Aimbot_AutoStop) {
								cmd->forwardmove = 0;
								cmd->sidemove = 0;
							}


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

