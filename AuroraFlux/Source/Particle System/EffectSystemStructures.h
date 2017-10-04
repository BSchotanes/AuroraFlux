/***********************************************
* Filename:  		EffectSystemStructures.h
* Date:      		01/08/2013
* Mod. Date: 		01/08/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be using to hold all its structures			
************************************************/

#include "../StdAfx.h"

struct tParticles
{
	float m_fLifeTime;
	D3DXVECTOR2 m_d3dScale;
	float m_fRotation;
	unsigned int m_uiColor;
	D3DXVECTOR3 m_d3dVelocity;
	D3DXVECTOR3 m_d3dPosition;
	bool m_bFacePlayer;
	bool m_bAlive;
	int m_nType;
	// 
	bool m_bNoRotation;
	tParticles() : m_fLifeTime(0.0f), m_d3dScale(0.0f, 0.0f), m_fRotation(0.0f), m_uiColor(0), 
		m_d3dVelocity(0.0f,0.0f, 0.0f), m_d3dPosition(0.0f, 0.0f, 0.0f), m_bFacePlayer(true), m_bAlive(false), m_bNoRotation(false), m_nType(-1)
	{}
};

struct tGeometry 
{
	float		m_fLifetime;
	unsigned int	m_uiColor;
	D3DXVECTOR3	m_d3dRotation;
	D3DXVECTOR3	m_d3dScale;
	D3DXVECTOR3	m_d3dPosition;
	D3DXVECTOR3	m_d3dVelocity;
	bool m_bAlive;
	int m_nType;

	tGeometry() : m_fLifetime(0.0f), m_uiColor(0), m_d3dRotation(0.0f,0.0f,0.0f), m_d3dScale(0.0f,0.0f,0.0f), 
		m_d3dPosition(0.0f, 0.0f, 0.0f), m_d3dVelocity(0.0f, 0.0f, 0.0f), m_bAlive(false), m_nType(-1)
	{
	}
};


struct tParticleScale
{
	vector<D3DXVECTOR2> m_d3dValues;
	vector<float>	    m_fTime;

	tParticleScale()
	{}
};

struct tParticleRotation
{
	vector<float> m_fValues;
	vector<float> m_fTime;

	tParticleRotation() 
	{}
};

struct tParticleVelocity
{
	vector<D3DXVECTOR3> m_d3dValues;
	vector<float>       m_fTime;

	tParticleVelocity()
	{}
};


struct tParticleColor
{
	vector<unsigned int> m_uiValues;
	vector<float>        m_fTime;

	tParticleColor()
	{}
};

struct tParticleGeneralInfo
{
	float m_fFrameOffset;
	float m_fLifetime;
	float m_fGravity;
	unsigned int m_uiSourceBlend;
	unsigned int m_uiDestinationBlend;
	int m_nNumberOfParticles;
	int m_nTextureID;
	D3DXVECTOR3 m_d3dPositionOffset;
	bool m_bFacePlayer;
	float m_fEmitterRate;
	bool m_bNoRotation;

	tParticleGeneralInfo() : m_fFrameOffset(0.0f), m_fLifetime(0.0f), m_fGravity(0.0f), 
		m_uiSourceBlend(0), m_uiDestinationBlend(0), m_nNumberOfParticles(0), m_nTextureID(0), m_d3dPositionOffset(0.0f, 0.0f, 0.0f), m_bFacePlayer(true), m_bNoRotation(false)
	{}
};


struct tGeometryScale
{
	vector<D3DXVECTOR3> m_d3dValues;
	vector<float>	    m_fTime;

	tGeometryScale()
	{}
};

struct tGeometryRotation
{
	vector<D3DXVECTOR3> m_d3dValues;
	vector<float> m_fTime;

	tGeometryRotation() 
	{}
};

struct tGeometryVelocity
{
	vector<D3DXVECTOR3> m_d3dValues;
	vector<float>       m_fTime;

	tGeometryVelocity()
	{}
};

struct tGeometryColor
{
	vector<unsigned int> m_uiValues;
	vector<float>        m_fTime;

	tGeometryColor()
	{}
};

struct tGeometryGeneralInfo
{
	float m_fLifetime;
	float m_fTimeOffset;
	unsigned int m_uiNumberOfGeometry;
	unsigned int m_uiSourceBlend;
	unsigned int m_uiDestinationBlend;
	int m_nMeshID;
	int m_nTextureID;
	float m_fGravity;
	D3DXVECTOR3 m_d3dPositionOffset;
	float m_fEmitterRate;

	tGeometryGeneralInfo() : m_fTimeOffset(0.0f), m_fLifetime(0.0f), m_nMeshID(-1), 
		m_uiNumberOfGeometry(0), m_nTextureID(-1), m_d3dPositionOffset(0.0f, 0.0f, 0.0f), m_fGravity(0.0f)
	{}
};





struct tEffectSystemCreationInfo
{
	int m_nNumberOfParticleSystems;
	int m_nNumberOfGeometrySystems;
	float m_fLifetime;
	string m_nEffectName;

	vector<tParticleGeneralInfo>    m_vParticleGeneralInfo;
	vector<tParticleScale>			m_vParticleScaleInfo;
	vector<tParticleRotation>		m_vParticleRotationInfo;
	vector<tParticleVelocity>		m_vParticleVelocityInfo;
	vector<tParticleColor>			m_vParticleColorInfo;

	vector<tGeometryGeneralInfo>    m_vGeometryGeneralInfo;
	vector<tGeometryScale>			m_vGeometryScaleInfo;
	vector<tGeometryRotation>		m_vGeometryRotationInfo;
	vector<tGeometryVelocity>		m_vGeometryVelocityInfo;
	vector<tGeometryColor>			m_vGeometryColorInfo;


	tEffectSystemCreationInfo() : m_nNumberOfParticleSystems(0), m_nNumberOfGeometrySystems(0), m_fLifetime(0.0f)
	{}
};



