#include "DirectionalLight.h"

void DirectionalLight::ApplyLight(ID3DXEffect* _effect)
{
	D3DXHANDLE handle;
	handle = _effect->GetParameterByName(m_szName.c_str(), "Direction");
	D3DXVECTOR4 direction(GetWorldMatrixPtr()->_31, 
		GetWorldMatrixPtr()->_32, GetWorldMatrixPtr()->_33, 0);
	_effect->SetVector(handle, &GetPosition());


	Light::ApplyLight(_effect);
}