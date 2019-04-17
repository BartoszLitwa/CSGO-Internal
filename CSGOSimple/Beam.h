#pragma once
#include "../CSGOSimple/helpers/utils.hpp"
#include "../CSGOSimple/valve_sdk/csgostructs.hpp"
#include "../CSGOSimple/helpers/math.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/CClientState.hpp"
#include "../CSGOSimple/valve_sdk/interfaces/IVEngineClient.hpp"
class C_Beam;
class Beam_t;

struct bullet
{
	bullet(int userID, Vector FirePos, Vector i, float itime)
	{
		this->userid = userid;
		this->firepos = FirePos;
		this->impactpos = i;
		this->impacttime = itime;
	}
	Vector impactpos;
	float impacttime;
	Vector firepos;
	int userid;
};

struct BeamInfo_t
{
	//Beam
	int                m_nType;
	C_BaseEntity*    m_pStartEnt;
	int                m_nStartAttachment;
	C_BaseEntity*    m_pEndEnt;
	int                m_nEndAttachment;
	Vector            m_vecStart;
	Vector            m_vecEnd;
	int                m_nModelIndex;
	const char*        m_pszModelName;
	int                m_nHaloIndex;
	const char*        m_pszHaloName;
	float            m_flHaloScale;
	float            m_flLife;
	float            m_flWidth;
	float            m_flEndWidth;
	float            m_flFadeLength;
	float            m_flAmplitude;
	float            m_flBrightness;
	float            m_flSpeed;
	int                m_nStartFrame;
	float            m_flFrameRate;
	float            m_flRed;
	float            m_flGreen;
	float            m_flBlue;
	bool            m_bRenderable;
	int                m_nSegments;
	int                m_nFlags;
	// Rings
	Vector            m_vecCenter;
	float            m_flStartRadius;
	float            m_flEndRadius;

	BeamInfo_t()
	{
		m_nType = 0;
		m_nSegments = -1;
		m_pszModelName = NULL;
		m_pszHaloName = NULL;
		m_nModelIndex = -1;
		m_nHaloIndex = -1;
		m_bRenderable = true;
		m_nFlags = 0;
	}
};
