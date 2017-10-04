#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "../StdAfx.h"
#include "Light.h"

class DirectionalLight : public Light
{
public:
	virtual void ApplyLight(ID3DXEffect* _effect);
};
#endif