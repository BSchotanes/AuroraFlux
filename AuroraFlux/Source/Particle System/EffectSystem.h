/***********************************************
* Filename:  		EffectSystem.h
	* Date:      		01/09/2013
* Mod. Date: 		01/09/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that will control all the Effects in the game			
************************************************/

#ifndef EFFECT_SYSTEM_H
#define EFFECT_SYSTEM_H


#include "../StdAfx.h"
#include "../Definitions.h"
#include "../Entity/Entity.h"
#include "GeometrySystem.h"
#include "ParticleSystem.h"




class CEffectSystem
{
public:
	CEffectSystem();
	~CEffectSystem();

	void Update(float _fDeltaTime);
	//bool LoadEffects( char * _cpFilePath );
	void Init();
	void CreateEffect(tEffectSystemCreationInfo* _pEffectInfo, bool _pRepeatible, bool _pPlay);
	void ResetPosition();

	void SetAssetMananger(CAssetManager* _pAssetManager) { m_pAssetManager = _pAssetManager; } 	
	void SetRepeatible(bool _bRepeat);
	void SetAlive( bool _isAlive ) { m_bIsAlive = _isAlive; }
	void SetOwner(CEntity* _pOwner) { m_pOwner = _pOwner; }	
	void SetPlay( bool _bPlay ) { m_bPlay = _bPlay; }
	void SetEmitterRate( float _fRate );
	void SetEffectType(int _nType) { m_nEffectType = _nType; }
	

	vector<CParticleSystem> * const GetParticleSystem() { return &m_ParticleSystem; }
	vector<CGeometrySystem> * const GetGeometrySystem() { return &m_GeometrySystem; }
	bool GetAlive() { return m_bIsAlive; }
	bool GetPlay() { return m_bPlay; }
	int GetType() { return m_nEffectType; }

private:

	float m_fLifetime;
	float m_fCurrLife;
	CEntity * m_pOwner;

	vector<CParticleSystem> m_ParticleSystem;
	vector<CGeometrySystem> m_GeometrySystem;

	CAssetManager* m_pAssetManager;

	bool m_bIsAlive;
	bool m_bRepeatable;
	bool m_bPlay;
	int m_nEffectType;
};

#endif