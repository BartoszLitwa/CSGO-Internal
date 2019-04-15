#include "SkinChanger.h"
#include "../CSGOSimple/options.hpp"
#include "../CSGOSimple/helpers/utils.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/CClientState.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/IVEngineClient.hpp"

void Skinchanger::Skinchanger()
{
	for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		C_BaseCombatWeapon* weapon = (C_BaseCombatWeapon*)g_EntityList->GetClientEntity(i);
		if (!weapon)
			continue;
		if (!g_EntityList->GetClientEntityFromHandle(*(int*)(DWORD)weapon))
			continue;
		if (g_EntityList->GetClientEntityFromHandle(*(int*)(DWORD)weapon) != g_LocalPlayer)
			continue;
	}
}

void Skinchanger::ForceSkin(C_BaseCombatWeapon * weapon, int skinid)
{
	
}

void Skinchanger::Knifechanger()
{
}
