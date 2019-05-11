#include "Skins.h"
#include "../CSGOSimple/options.hpp"
#include "../CSGOSimple/helpers/utils.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/CClientState.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/IVEngineClient.hpp"
#include "IteamDefinitions.hpp"
#include <unordered_map>

struct EconomyItemCfg {
	int iItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int nFallbackSeed = 0;
	int nFallbackStatTrak = -1;
	int iEntityQuality = 4;
	char* szCustomName = nullptr;
	float flFallbackWear = 0.1f;
};

std::unordered_map<int, EconomyItemCfg> g_SkinChangerCfg;
std::unordered_map<int, const char*> g_ViewModelCfg;
std::unordered_map<const char*, const char*> g_KillIconCfg;

inline void SetSkinConfig() {
	// StatTrak™ AWP | Dragon Lore
	g_SkinChangerCfg[WEAPON_AWP].nFallbackPaintKit = 344;
	g_SkinChangerCfg[WEAPON_AWP].flFallbackWear = 0.00000001f;
	g_SkinChangerCfg[WEAPON_AWP].nFallbackStatTrak = 1337;

	// Valve AK-47 | Redline
	g_SkinChangerCfg[WEAPON_AK47].nFallbackPaintKit = 282;
	g_SkinChangerCfg[WEAPON_AK47].iEntityQuality = 6;

	// Souvenir M4A4 | Howl
	g_SkinChangerCfg[WEAPON_M4A1].nFallbackPaintKit = 309;
	g_SkinChangerCfg[WEAPON_M4A1].iEntityQuality = 12;

	// Prototype Desert Eagle | Conspiracy
	g_SkinChangerCfg[WEAPON_DEAGLE].nFallbackPaintKit = 351;
	g_SkinChangerCfg[WEAPON_DEAGLE].iEntityQuality = 7;

	// Glock-18 | Fade
	g_SkinChangerCfg[WEAPON_GLOCK].nFallbackPaintKit = 38;

	// USP-S | Stainless
	g_SkinChangerCfg[WEAPON_USP_SILENCER].nFallbackPaintKit = 277;

	// Karambit | Fade (CT)
	g_SkinChangerCfg[WEAPON_KNIFE].iItemDefinitionIndex = WEAPON_KNIFE_KARAMBIT;
	g_SkinChangerCfg[WEAPON_KNIFE].nFallbackPaintKit = 38;
	g_SkinChangerCfg[WEAPON_KNIFE].iEntityQuality = 3;

	// Bowie Knife | Crimson Web (T)
	g_SkinChangerCfg[WEAPON_KNIFE_T].iItemDefinitionIndex = WEAPON_KNIFE_SURVIVAL_BOWIE;
	g_SkinChangerCfg[WEAPON_KNIFE_T].nFallbackPaintKit = 12;
	g_SkinChangerCfg[WEAPON_KNIFE_T].iEntityQuality = 3;
}

inline void SetModelConfig() {
	// Get the indexes of the models we want to replace.
	int nOriginalKnifeCT = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
	int nOriginalKnifeT = g_MdlInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");

	// Configure model replacements.
	g_ViewModelCfg[nOriginalKnifeCT] = "models/weapons/v_knife_karam.mdl";
	g_ViewModelCfg[nOriginalKnifeT] = "models/weapons/v_knife_survival_bowie.mdl";
}

inline void SetKillIconCfg() {
	// Define replacement kill icons. (these only apply to you)
	g_KillIconCfg["knife_default_ct"] = "knife_karambit";
	g_KillIconCfg["knife_t"] = "knife_survival_bowie";
}

void Skins::OnFrameStageNotify(ClientFrameStage_t stage) // https://github.com/emskye96/chameleon-ng
{
	while (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		int localplayer_index = g_EngineClient->GetLocalPlayer();
		C_BasePlayer* localplayer = (C_BasePlayer*)(g_EntityList->GetClientEntity(localplayer_index));

		if (!localplayer || localplayer->m_lifeState() != LIFE_ALIVE)
			break;
		 
		player_info_t localplayer_info;

		if (!g_EngineClient->GetPlayerInfo(localplayer_index, &localplayer_info))
			break;

		UINT* weapons = localplayer->GetWeapons(); 
		
		if (!weapons)
			break;

		for (int i = 0; i < 8; i++)
		{
			C_BaseAttributableItem* weapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(weapons[i]);

			if (!weapon)
				continue;

			int weaponindex = *weapon->GetItemDefinitionIndex();
			char array[10];
			sprintf(array, "%f", weaponindex);
			Utils::ConsolePrint(array); Utils::ConsolePrint("\n");
			
			if (g_ViewModelCfg.find(weapon->m_nModelIndex()) != g_ViewModelCfg.end())
				weapon->SetModelIndex(g_MdlInfo->GetModelIndex(g_ViewModelCfg[weapon->m_nModelIndex()]));

			ApplyCustomModel(localplayer, weapon);

			if (localplayer_info.xuid_low != *weapon->GetOriginalOwnerXuidLow())
				continue;

			if (localplayer_info.xuid_high != *weapon->GetOriginalOwnerXuidHigh())
				continue;

			ApplyCustomSkin(weapon, weaponindex);

			*weapon->GetAccountID() = localplayer_info.xuid_low;
		}
		g_ClientState->ForceFullUpdate();

	}
}

// Function to apply skin data to weapons.
bool Skins::ApplyCustomSkin(C_BaseAttributableItem* pWeapon, int nWeaponIndex) {
	// Check if this weapon has a valid override defined.
	if (g_SkinChangerCfg.find(nWeaponIndex) == g_SkinChangerCfg.end())
		return false;

	// Apply our changes to the fallback variables.
	*pWeapon->GetFallbackPaintKit() = g_SkinChangerCfg[nWeaponIndex].nFallbackPaintKit;
	*pWeapon->GetEntityQuality() = g_SkinChangerCfg[nWeaponIndex].iEntityQuality;
	*pWeapon->GetFallbackSeed() = g_SkinChangerCfg[nWeaponIndex].nFallbackSeed;
	*pWeapon->GetFallbackStatTrak() = g_SkinChangerCfg[nWeaponIndex].nFallbackStatTrak;
	*pWeapon->GetFallbackWear() = g_SkinChangerCfg[nWeaponIndex].flFallbackWear;

	if (g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex)
		*pWeapon->GetItemDefinitionIndex() = g_SkinChangerCfg[nWeaponIndex].iItemDefinitionIndex;

	// If a name is defined, write it now.
	if (g_SkinChangerCfg[nWeaponIndex].szCustomName) {
		sprintf_s(pWeapon->GetCustomName(), 32, "%s", g_SkinChangerCfg[nWeaponIndex].szCustomName);
	}

	// Edit "m_iItemIDHigh" so fallback values will be used.
	*pWeapon->GetOriginalOwnerXuidHigh() = -1;

	return true;
}

// Function to apply custom view models to weapons.
bool Skins::ApplyCustomModel(C_BasePlayer* pLocal, C_BaseAttributableItem* pWeapon) {
	// Get the view model of this weapon.
	C_BaseViewModel* pViewModel = pLocal->m_hViewModel();

	if (!pViewModel)
		return false;

	// Get the weapon belonging to this view model.
	CBaseHandle hViewModelWeapon = pViewModel->m_hWeapon();
	C_BaseAttributableItem* pViewModelWeapon = (C_BaseAttributableItem*)g_EntityList->GetClientEntityFromHandle(hViewModelWeapon);

	if (pViewModelWeapon != pWeapon)
		return false;

	// Check if an override exists for this view model.
	int nViewModelIndex = pViewModel->m_nModelIndex();

	if (g_ViewModelCfg.find(nViewModelIndex) == g_ViewModelCfg.end())
		return false;

	// Set the replacement model.
	pViewModel->m_nModelIndex() = g_MdlInfo->GetModelIndex(g_ViewModelCfg[nViewModelIndex]); 

	return true;
}

// Function to apply custom kill icons to events.
bool Skins::ApplyCustomKillIcon(IGameEvent* pEvent) {
	// Get the user ID of the attacker.
	int nUserID = pEvent->GetInt("attacker");

	if (!nUserID)
		return false;

	// Only continue if we were the attacker.
	if (g_EngineClient->GetPlayerForUserID(nUserID) != g_EngineClient->GetLocalPlayer())
		return false;

	// Get the original weapon used to kill.
	const char* szWeapon = pEvent->GetString("weapon");

	for (auto ReplacementIcon : g_KillIconCfg) {
		// Search for a valid replacement.
		if (!strcmp(szWeapon, ReplacementIcon.first)) {
			// Replace with user defined value.
			pEvent->SetString("weapon", ReplacementIcon.second);
			break;
		}
	}

	return true;
}
