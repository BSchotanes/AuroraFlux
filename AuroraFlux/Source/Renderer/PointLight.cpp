#include "PointLight.h"

PointLight::PointLight()
{
	radius = -1.0f;
	attenuation = D3DXVECTOR3(1, 1, 1);
	m_nMeshNumber = -1;
}
PointLight::~PointLight()
{

}
void PointLight::ApplyLight(ID3DXEffect *_effect)
{
	D3DXHANDLE handle;

	 handle = _effect->GetParameterByName(m_szName.c_str(), "Position");
	_effect->SetVector(handle, &GetPosition());
	 handle = _effect->GetParameterByName(m_szName.c_str(), "Attenuation");
	_effect->SetVector(handle, &D3DXVECTOR4(attenuation, 1.0f));
	 handle = _effect->GetParameterByName(m_szName.c_str(), "Range");
	_effect->SetFloat(handle, radius);

	_effect->SetTechnique("RenderPointLight");
	
	Light::ApplyLight(_effect);
}

void PointLight::SetAttenuation(const D3DXVECTOR3 *_attenuation, float _radius) 
{ 
	attenuation = *_attenuation; 

	SetRadius(_radius);
}

void PointLight::SetAttenuation(const D3DXVECTOR3 *_attenuation) 
{ 
	attenuation = *_attenuation;
}
void PointLight::SetRadius(float _radius)
{
	radius = _radius;
	D3DXMatrixScaling(&m_d3dWorldMat, GetRadius(), GetRadius(), GetRadius());
}