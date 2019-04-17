#pragma once
#include "Configs.h"
#include "options.hpp"
#include "../CSGOSimple/helpers/utils.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/CClientState.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/IVEngineClient.hpp"

void CConfig::Setup()
{
	
	SetupValue(g_Options.misc_bhop, 0, "Misc", "BHOP");
}

void CConfig::SetupValue(int& value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue< int >(category, name, &value));
}

void CConfig::SetupValue(float& value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue< float >(category, name, &value));
}

void CConfig::SetupValue(bool& value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue< bool >(category, name, &value));
}

//void CConfig::Save()
//{
//	static TCHAR path[MAX_PATH];
//	std::string folder, file;
//
//	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
//	{
//		folder = std::string(path) + strenc("\\notacheat\\");
//		file = std::string(path) + strenc("\\notacheat\\gamerfood.ini");
//	}
//
//	CreateDirectory(folder.c_str(), NULL);
//
//	for (auto value : ints)
//		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
//
//	for (auto value : floats)
//		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());
//
//	for (auto value : bools)
//		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
//}
//
//void CConfig::Load()
//{
//	static TCHAR path[MAX_PATH];
//	std::string folder, file;
//
//	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, path)))
//	{
//		folder = std::string(path) + strenc("\\notacheat\\");
//		file = std::string(path) + strenc("\\notacheat\\gamerfood.ini");
//	}
//
//	CreateDirectory(folder.c_str(), NULL);
//
//	char value_l[32] = { '\0' };
//
//	for (auto value : ints)
//	{
//		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
//		*value->value = atoi(value_l);
//	}
//
//	for (auto value : floats)
//	{
//		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
//		*value->value = atof(value_l);
//	}
//
//	for (auto value : bools)
//	{
//		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
//		*value->value = !strcmp(value_l, "true");
//	}
//}

CConfig* ConfigSave = new CConfig();