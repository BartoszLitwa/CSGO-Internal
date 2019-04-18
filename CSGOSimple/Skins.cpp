#include "Skins.h"
#include "../CSGOSimple/options.hpp"
#include "../CSGOSimple/helpers/utils.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/CClientState.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/IVEngineClient.hpp"
#include "IteamDefinitions.hpp"

void Skins::OnFrameStageNotify(ClientFrameStage_t stage) // https://github.com/emskye96/chameleon-ng
{
	while (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		int localplayer_index = g_EngineClient->GetLocalPlayer();
		C_BasePlayer* localplayer = static_cast<C_BasePlayer*>(g_EntityList->GetClientEntity(localplayer_index));

		if (!localplayer || localplayer->m_lifeState() != LIFE_ALIVE)
			break;
		 
		player_info_t localplayer_info;

		if (!g_EngineClient->GetPlayerInfo(localplayer_index, &localplayer_info))
			break;

		CBaseHandle* weapons = localplayer->m_hMyWeapons();

		for (size_t i = 0; weapons[i] != INVALID_EHANDLE_INDEX; i++)
		{
			C_BaseAttributableItem* weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(weapons[i]));

			if (!weapon)
				break;

			//unsigned int item_definition_index = &weapon->m_Item().m_iItemDefinitionIndex;

			weapon->m_Item().m_iItemIDHigh() = -1;						// Force fallback values to be used.
			weapon->m_Item().m_iAccountID() = localplayer_info.xuid_low;	// Set the owner of the weapon to our lower XUID. (fixes StatTrak)
			//weapon->m_nModelIndex() = g_MdlInfo->GetModelIndex(ItemDefinitionIndex.at(507).model); // Set the weapon model index -- required for paint kits to work on replacement items after the 29/11/2016 update.

			weapon->m_nFallbackPaintKit() = 707;
			weapon->m_nFallbackStatTrak() = 707;
			//*weapon->m_Item().m_iCustomName() = "Test";
			weapon->m_nFallbackSeed() = 1;
			weapon->m_flFallbackWear() = 0.001f;
		}

		// Get a handle to the view model entity.
		CBaseHandle viewmodel_handle = localplayer->m_hViewModel();

		if (viewmodel_handle == INVALID_EHANDLE_INDEX)
			break;

		// Convert the view model handle into an entity pointer.
		C_BaseViewModel* viewmodel = static_cast<C_BaseViewModel*>(g_EntityList->GetClientEntityFromHandle(viewmodel_handle));

		if (!viewmodel)
			break;

		// Get a handle to the active weapon for this view model.
		CBaseHandle viewmodel_weapon_handle = viewmodel->m_hWeapon();

		if (viewmodel_weapon_handle == INVALID_EHANDLE_INDEX)
			break;

		// Convert the view model weapon handle into an entity pointer.
		C_BaseAttributableItem* viewmodel_weapon = static_cast<C_BaseAttributableItem*>(g_EntityList->GetClientEntityFromHandle(viewmodel_weapon_handle));

		if (!viewmodel_weapon)
			break;

		// Finally, write the correct model index for this weapon.
			/*const Item_t& override_weapon = ItemDefinitionIndex.at(*viewmodel_weapon->m_Item().m_iItemDefinitionIndex);
			viewmodel->m_nModelIndex() = g_MdlInfo->GetModelIndex(override_weapon.model);*/


		//break;
	}
}
