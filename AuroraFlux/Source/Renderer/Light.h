#ifndef LIGHT_H
#define LIGHT_H
#include "../StdAfx.h"

class Light
{
protected:
	string m_szName;
	D3DXVECTOR4 m_d3dDiffuseColor;
	D3DXVECTOR4 m_d3dAmbientColor;
	D3DXVECTOR4 m_d3dSpecularColor;
	// How Large the reflection is
	float			m_fSpecularPower;
	// Brightness of Reflection
	float			m_fSpecularIntensity;
	int				m_nMeshNumber;
	bool			m_bIsEnabled;
	//D3DXVECTOR4		m_d3dPosition;
	D3DXMATRIX		m_d3dWorldMat;
public:
	Light();
	~Light();

	virtual void ApplyLight(ID3DXEffect *_effect) = 0;

	void SetName(const char *_name) { m_szName = _name; }
	void SetDiffuseColor(const D3DXVECTOR4 *_diffuseColor) { m_d3dDiffuseColor = *_diffuseColor; }
	void SetAmbientColor(const D3DXVECTOR4 *_ambientColor) { m_d3dAmbientColor = *_ambientColor; }
	void SetSpecularColor(const D3DXVECTOR4 *_specularColor) { m_d3dSpecularColor = *_specularColor; }
	void SetEnabled(bool _isEnabled) { m_bIsEnabled = _isEnabled; }
	void SetSpecularPower(float _specularPower) { m_fSpecularPower = _specularPower; }
	void SetSpecularIntensity(float _specularIntensisty) { m_fSpecularIntensity = _specularIntensisty; }
	//void SetAttenuation(const D3DXVECTOR3 *_attenuation, float _radius);
	void SetPosition(D3DXVECTOR3 position);
	void SetMesh(int meshNumber) {m_nMeshNumber = meshNumber;}

	inline string GetName() {return m_szName;}
	inline int GetMeshNumber() {return m_nMeshNumber;}
	inline bool GetEnabled() {return m_bIsEnabled;}
	D3DXMATRIX *GetWorldMatrixPtr() {return &m_d3dWorldMat;}
	D3DXVECTOR4 GetPosition();
};
#endif