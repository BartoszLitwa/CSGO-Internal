#pragma once
#include "valve_sdk/csgostructs.hpp"
#include "options.hpp"
namespace Skins {
	void OnFrameStageNotify(ClientFrameStage_t stage);
	bool ApplyCustomSkin(C_BaseAttributableItem* pWeapon, int nWeaponIndex);
	bool ApplyCustomModel(C_BasePlayer* pLocal, C_BaseAttributableItem* pWeapon);
	bool ApplyCustomKillIcon(IGameEvent* pEvent);
};
