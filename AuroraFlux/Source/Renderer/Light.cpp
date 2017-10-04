#include "Light.h"

Light::Light()
{
	D3DXMatrixIdentity(&m_d3dWorldMat);
}

Light::~Light()
{

}
void Light::ApplyLight(ID3DXEffect *_effect)
{
	D3DXHANDLE handle;
	handle = _effect->GetParameterByName(m_szName.c_str(), "DiffuseColor");
	_effect->SetVector(handle, &m_d3dDiffuseColor);
	handle = _effect->GetParameterByName(m_szName.c_str(), "AmbientColor");
	_effect->SetVector(handle, &m_d3dAmbientColor);
	handle = _effect->GetParameterByName(m_szName.c_str(), "SpecularColor");
	_effect->SetVector(handle, &m_d3dSpecularColor);
	handle = _effect->GetParameterByName(m_szName.c_str(), "SpecularPower");
	_effect->SetFloat(handle, m_fSpecularPower);
	handle = _effect->GetParameterByName(m_szName.c_str(), "SpecularIntensity");
	_effect->SetFloat(handle, m_fSpecularIntensity);
	handle = _effect->GetParameterByName(m_szName.c_str(), "Enabled");
	_effect->SetBool(handle, m_bIsEnabled);
}

D3DXVECTOR4 Light::GetPosition()
{
	return D3DXVECTOR4(m_d3dWorldMat._41,m_d3dWorldMat._42,m_d3dWorldMat._43,m_d3dWorldMat._44);
}
void Light::SetPosition(D3DXVECTOR3 position)
{
	D3DXMatrixIdentity(&m_d3dWorldMat);
	D3DXMatrixTranslation(&m_d3dWorldMat,position.x,position.y,position.z);
}
