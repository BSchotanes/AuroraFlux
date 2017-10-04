#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "../StdAfx.h"
#include "Light.h"
class PointLight : public Light
{
private:
	//	A vector representing the attenuation of the light.
	//	x = constant attenuation
	//	y = linear attenuation
	//	z = quadratic attenuation
	D3DXVECTOR3 attenuation;

	float radius;

public:
	PointLight();
	~PointLight();

	void ApplyLight(ID3DXEffect *_effect);
	inline float GetRadius() { return radius; }
	void SetAttenuation(const D3DXVECTOR3 *_attenuation, float _radius);
	void SetAttenuation(const D3DXVECTOR3 *_attenuation);
	void SetRadius(float _radius);
};
#endif