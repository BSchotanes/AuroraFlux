/***********************************************
* Filename:  		ParticleSystem.h
* Date:      		01/08/2013
* Mod. Date: 		01/08/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be using for particles			
************************************************/

#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../StdAfx.h"
#include "../Definitions.h"
#include "../Entity/Entity.h"
#include "../Input/Input.h"

class CParticleSystem
{

public:
	CParticleSystem();
	~CParticleSystem();

	void Update(float _fDeltaTime, D3DXMATRIX * _d3dCurrPosition, int _eType = -1, CEntity* _pOwner = NULL, float _dist = 0.0f, float _infrontDist = 0.0f, D3DXVECTOR3* dir = NULL);
	void Init(D3DXVECTOR3* position);
	void ResetPosition( D3DXVECTOR3* _position );
	void ResetPosition( D3DXMATRIX _Matrix );

	void SetFrameOffset(float _fOffset) { m_fFrameOffset = _fOffset; }
	float GetFrameOffset() { return m_fFrameOffset; }
	void SetLifeTime(float _fLife) { m_fLifeTime = _fLife; }
	
	void SetAllScaleValues(std::vector<D3DXVECTOR2>* _vfpScales);
	void SetAllScaleTimes(std::vector<float>* _vfpScaleTimes);

	void SetAllRotationValues(std::vector<float>* _vfpRotations);
	void SetAllRotationTimes(std::vector<float>* _vfpRotationTimes);

	void SetAllVelocityValues(std::vector<D3DXVECTOR3>* _vd3dpVelocities);
	void SetAllVelocityTimes(std::vector<float>* _vd3dpVelocityTimes);

	void SetAllColorValues(std::vector<unsigned int>* _vuipColors);
	void SetAllColorTimes(std::vector<float>* _vuipColorTimes);

	void SetGravity( float _fGravity ) { m_fGravity = _fGravity; }
	void SetPositionOffset( D3DXVECTOR3 _d3dOffset ) { m_d3dPositionOffset = _d3dOffset; }
	void SetBlendModes( unsigned int _uiSourceBlend, unsigned int _uiDestinationBlend ) { m_uiDestinationBlendModes = _uiSourceBlend; m_uiSourceBlendModes = _uiSourceBlend; }
	void SetNumberOfParticles( unsigned int _uiNumber ) { m_uiNumberOfParticles = _uiNumber; }

	void SetTexture(int _textID);
	void SetEmitterRate( float _fRate ) { fEmitterRate = _fRate; }
		
	void SetRepeating(bool _repeat) { m_bRepeating = _repeat; }
	void SetFacePlayer(bool _facing) {m_bFacePlayer = _facing; }
	void SetNoRotation(bool _NoRotation) {m_bNoRotation = _NoRotation; }

	vector<tParticles> * const GetParticles() { return &m_vtParticles; } 
	int getTextureID() { return m_nTextureID; } 
	unsigned int  getDesitinationBlend() { return m_uiDestinationBlendModes; }
	unsigned int  getSourceBlend() { return m_uiSourceBlendModes; }

	void SetSecondScale(float _scaleX, float _scaleY) {m_fSecondScale.x = _scaleX; m_fSecondScale.y = _scaleY;}

private:

	void ResetParticle(tParticles * _tParticle, D3DXMATRIX* _d3dCurPos);
	void UpdateColor(tParticles* _tCurrParticle);


	float m_fFrameOffset;
	float m_fLifeTime;

	//Will need to change if different particles have different ratios 

	//Scale
	D3DXVECTOR2 m_fFirstScale;
	D3DXVECTOR2 m_fSecondScale;
	D3DXVECTOR2 m_fThirdScale;

	float m_fStartScaleTime;
	float m_fMiddleScaleTime;
	float m_fEndScaleTime;


	//Rotation
	float m_fFirstRotation;
	float m_fSecondRotation;
	float m_fThirdRotation;

	float m_fStartRotationTime;
	float m_fMiddleRotationTime;
	float m_fEndRotationTime;

	
	//Velocity
	D3DXVECTOR3 m_d3dFirstVelocity;
	D3DXVECTOR3 m_d3dSecondVelocity;
	D3DXVECTOR3 m_d3dThirdVelocity;

	float m_fStartVelocityTime;
	float m_fMiddleVelocityTime;
	float m_fEndVelocityTime;


	//Color
	unsigned int m_uiFirstColor;
	unsigned int m_uiSecondColor;
	unsigned int m_uiThirdColor;
	
	float m_fStartColorTime;
	float m_fMiddleColorTime;
	float m_fEndColorTime;


	//General
	float m_fGravity;
	D3DXVECTOR3 m_d3dPositionOffset;

	unsigned int m_uiDestinationBlendModes;
	unsigned int m_uiSourceBlendModes;


	vector<tParticles > m_vtParticles;

	unsigned int m_uiNumberOfParticles;


	int m_nTextureID;
	
	bool m_bFacePlayer;
	bool m_bNoRotation;
	bool m_bRepeating;

	D3DXVECTOR3 position;

	float fEmitterRate;
	float fCurrEmitterTime;
	unsigned int uiNextParticle;

	D3DXVECTOR2 XYVelocity;
	
	float GetXVelocity()			{return XYVelocity.x;}
	void SetXVelocity(float _x)		{XYVelocity.x = _x;};
	float GetYVelocity()			{return XYVelocity.y;}
	void SetYVelocity(float _y)		{XYVelocity.y = _y;};
};

#endif