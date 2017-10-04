/***********************************************
* Filename:  		GeometrySystem.h
* Date:      		01/09/2013
* Mod. Date: 		01/09/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be using for geometry		
************************************************/

#ifndef GEOMETRY_SYSTEM_H
#define GEOMETRY_SYSTEM_H

#include "../StdAfx.h"
#include "../Definitions.h"


class CGeometrySystem
{

public:
	CGeometrySystem();
	~CGeometrySystem();

	void Update(float _fDeltaTime, D3DXMATRIX * _d3dCurrPosition, int _eType = -1, CEntity* _pOwner = NULL, float _dist = 0.0f, float _infrontDist = 0.0f, D3DXVECTOR3* dir = NULL );
	void Init(D3DXVECTOR3* position);
	void ResetPosition( D3DXVECTOR3* position );
	void ResetPosition( D3DXMATRIX _Matrix );

	void SetLifeTime( float _fLife ) { m_fLifetime = _fLife; }
	void SetTimeOffset( float _fTime ) { m_fOriginalOffset = m_fTimeOffset = _fTime; }

	void SetAllVelocityValues( std::vector<D3DXVECTOR3>* _vpVelocity );
	void SetAllVelocityTimes( std::vector<float>* _vfpVelocityTimes );

	void SetAllRotationValues( std::vector<D3DXVECTOR3>* _vpRotations );
	void SetAllRotationTimes(std::vector<float>* _vfpRotationTimes);

	void SetAllScaleValues(std::vector<D3DXVECTOR3>* _vfpScales);
	void SetAllScaleTimes(std::vector<float>* _vfpScaleTimes);

	void SetAllColorValues(std::vector<unsigned int>* _vuipColors);
	void SetAllColorTimes(std::vector<float>* _vuipColorTimes);
		
	void SetGravity( float _fGravity) { m_fGravity = _fGravity; }
	void SetPositionOffset( D3DXVECTOR3 _d3dOffset ) { m_d3dPositionOffset = _d3dOffset; }
	void SetNumberOfGeometry( unsigned int _uiNumber ) { m_uiNumberOfGeometry = _uiNumber; }
	
	void SetTexture(int _textID) { m_nTextureID = _textID; }
	void SetMesh(int _meshID) { m_nMeshID = _meshID; }

	void SetRepeating(bool _repeat) { m_bRepeating = _repeat; }
	void SetEmitterRate( float _fRate) { m_fEmitterRate = _fRate; }

	void SetBlendModes( unsigned int _uiSourceBlend, unsigned int _uiDestinationBlend ) { m_uiDestinationBlendModes = _uiSourceBlend; m_uiSourceBlendModes = _uiSourceBlend; }


	vector<tGeometry> * const GetGeometry() { return &m_vtGeometry; } 
	int GetTextureID() { return m_nTextureID; }
	int GetMeshID()    { return m_nMeshID; }
	unsigned int  getDesitinationBlend() { return m_uiDestinationBlendModes; }
	unsigned int  getSourceBlend() { return m_uiSourceBlendModes; }
	
	
private:
	void UpdateColor(tGeometry* _tCurrGeometry);
	void ResetGeometry(tGeometry * _tGeometry, D3DXMATRIX* _d3dCurPos);
	void CheckValidValue(float& _value);

	float m_fLifetime;
	float m_fTimeOffset;
	float m_fOriginalOffset;

	bool m_bLooping;
	bool m_bAlive;

	D3DXVECTOR3 m_d3dFirstRotation;
	D3DXVECTOR3 m_d3dSecondRotation;
	D3DXVECTOR3 m_d3dThirdRotation;

	float m_fStartRotationTime;
	float m_fMiddleRotationTime;
	float m_fEndRotationTime;

	D3DXVECTOR3 m_d3dFirstScale;
	D3DXVECTOR3 m_d3dSecondScale;
	D3DXVECTOR3 m_d3dThirdScale;

	float m_fStartScaleTime;
	float m_fMiddleScaleTime;
	float m_fEndScaleTime;


	unsigned int m_uiFirstColor;
	unsigned int m_uiSecondColor;
	unsigned int m_uiThirdColor;

	float m_fStartColorTime;
	float m_fMiddleColorTime;
	float m_fEndColorTime;

	D3DXVECTOR3 m_d3dFirstVelocity;
	D3DXVECTOR3 m_d3dSecondVelocity;
	D3DXVECTOR3 m_d3dThirdVelocity;

	float m_fStartVelocityTime;
	float m_fMiddleVelocityTime;
	float m_fEndVelocityTime;

	unsigned int m_uiNumberOfGeometry;
	D3DXVECTOR3 m_d3dPositionOffset;

	vector<tGeometry> m_vtGeometry;

	int m_nMeshID;
	int m_nTextureID;

	float m_fGravity;
	bool m_bRepeating;
	
	D3DXVECTOR3 m_d3dPosition;

	float m_fEmitterRate;
	float m_fCurrEmitterTime;
	unsigned int m_uiNextGeometry;	

	unsigned int m_uiDestinationBlendModes;
	unsigned int m_uiSourceBlendModes;

};

#endif