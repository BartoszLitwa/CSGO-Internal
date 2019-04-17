#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"

#define OPTION(type, var, val) type var = val

class Config
{
public:
    // 
    // ESP
    // 
    OPTION(bool, esp_enabled, true);
    OPTION(bool, esp_enemies_only, true);
    OPTION(bool, esp_player_boxes, true);
    OPTION(bool, esp_player_names, true);
    OPTION(bool, esp_player_health, true);
    OPTION(bool, esp_player_armour, true);
    OPTION(bool, esp_player_weapons, true);
    OPTION(bool, esp_player_snaplines, true);
    OPTION(bool, esp_crosshair, true);
    OPTION(bool, esp_dropped_weapons, true);
    OPTION(bool, esp_defuse_kit, true);
    OPTION(bool, esp_planted_c4, true);
	OPTION(bool, esp_items, true);

    // 
    // GLOW
    // 
    OPTION(bool, glow_enabled, true);
    OPTION(bool, glow_enemies_only, true);
    OPTION(bool, glow_players, true);
    OPTION(bool, glow_chickens, true);
    OPTION(bool, glow_c4_carrier, true);
    OPTION(bool, glow_planted_c4, true);
    OPTION(bool, glow_defuse_kits, true);
    OPTION(bool, glow_weapons, true);

    //
    // CHAMS
    //
    OPTION(bool, chams_player_enabled, true);
    OPTION(bool, chams_player_enemies_only, true);
    OPTION(bool, chams_player_wireframe, true);
    OPTION(bool, chams_player_flat, true);
    OPTION(bool, chams_player_ignorez, true);
    OPTION(bool, chams_player_glass, true);
    OPTION(bool, chams_arms_enabled, true);
    OPTION(bool, chams_arms_wireframe, true);
    OPTION(bool, chams_arms_flat, true);
    OPTION(bool, chams_arms_ignorez, true);
    OPTION(bool, chams_arms_glass, true);

    //
    // MISC
    //
	OPTION(bool, misc_bhop, true);
	OPTION(bool, misc_UnlimitedDuck, true);
	OPTION(bool, misc_AutoStrafe, true);
	OPTION(bool, misc_no_hands, true);
	OPTION(bool, misc_no_visual_recoil, true);
	OPTION(bool, misc_no_scope, true);
	OPTION(bool, misc_FakeLag, true);
	OPTION(int, misc_FakeLag_FakeLagAmount, 1);
	OPTION(bool, misc_thirdperson, true);
	OPTION(bool, misc_showranks, true);
	OPTION(bool, misc_watermark, true);
	OPTION(bool, misc_BulletTracer, true);
	OPTION(int, misc_BulletTracer_Value, 1000);
	OPTION(float, misc_thirdperson_dist, 150.f);
	OPTION(int, viewmodel_fov, 68);
	OPTION(int, fov, 110);
	OPTION(float, mat_ambient_light_r, 0.0f);
	OPTION(float, mat_ambient_light_g, 0.0f);
	OPTION(float, mat_ambient_light_b, 0.0f);
	OPTION(int, SkyChangerIndex, 0);
	OPTION(bool, misc_ClanTag, true);
	OPTION(char*, misc_clantagString, "CRNYY's Cheat");
	OPTION(bool, misc_nameChanger, false);
	OPTION(char*, misc_NameString, "Elo");

	//
	// Aimbot
	//
	OPTION(bool, AimbotCanShootWhileAAON, false);
	OPTION(bool, Aimbot_Aimbot, true);
	OPTION(bool, Aimbot_Silent, true);
	OPTION(bool, Aimbot_VisibilityCheck, false);
	OPTION(int, Aimbot_Bone, 0);
	OPTION(float, Aimbot_AimbotFov, 90.f);
	OPTION(bool, Aimbot_Smooth, false);
	OPTION(float, Aimbot_SmoothValue, 10.f);
	OPTION(bool, Aimbot_RecoilPrediction, true);
	OPTION(bool, Aimbot_BackTrack, true);
	OPTION(bool, Aimbot_AimAtBackTrack, true);
	OPTION(int, Aimbot_BackTrackTicks, 12);
	OPTION(bool, Aimbot_AutoWall, false);
	OPTION(float, Aimbot_AutoWallMinDmg, 50.0f);
	OPTION(bool, Aimbot_AutoStop, true);
	OPTION(bool, Aimbot_AutoScope, true);
	OPTION(bool, Aimbot_AutoCrouch, true);

	//
	// AntiAim
	//
	OPTION(bool, AntiAim_AntiAim, true);
	OPTION(int, AntiAim_AntiAimTypeYaw, 0);
	OPTION(int, AntiAim_AntiAimTypePitch, 0);
	OPTION(int, AntiAim_SpinBotSpeed, 1);
	OPTION(float, AntiAim_AntiAimyaw, 90.0f);
	OPTION(float, AntiAim_AntiAimpitch, 89.0f);

	//
	// SkinChanger
	//
	OPTION(bool, SkinChanger_SkinChanger, true);

	OPTION(bool, KnifeChanger_KnifeChanger, true);

    // 
    // COLORS
    // 
    OPTION(Color, color_esp_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_visible, Color(255, 0, 0));
    OPTION(Color, color_esp_ally_occluded, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_occluded, Color(255, 0, 0));
    OPTION(Color, color_esp_crosshair, Color(255, 255, 255));
    OPTION(Color, color_esp_weapons, Color(128, 0, 128));
    OPTION(Color, color_esp_defuse, Color(0, 128, 255));
    OPTION(Color, color_esp_c4, Color(255, 255, 0));
	OPTION(Color, color_esp_item, Color(255, 255, 255));

    OPTION(Color, color_glow_ally, Color(0, 128, 255));
    OPTION(Color, color_glow_enemy, Color(255, 0, 0));
    OPTION(Color, color_glow_chickens, Color(0, 128, 0));
    OPTION(Color, color_glow_c4_carrier, Color(255, 255, 0));
    OPTION(Color, color_glow_planted_c4, Color(128, 0, 128));
    OPTION(Color, color_glow_defuse, Color(255, 255, 255));
    OPTION(Color, color_glow_weapons, Color(255, 128, 0));

    OPTION(Color, color_chams_player_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_player_ally_occluded, Color(0, 255, 128));
    OPTION(Color, color_chams_player_enemy_visible, Color(255, 0, 0));
    OPTION(Color, color_chams_player_enemy_occluded, Color(255, 128, 0));
    OPTION(Color, color_chams_arms_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_arms_occluded, Color(0, 128, 255)); 
	OPTION(Color, color_watermark, Color(0, 128, 255)); // no menu config cuz its useless
	OPTION(Color, color_BulletTracer, Color(0, 255, 0));
};

extern Config g_Options;
extern bool   g_Unload;
