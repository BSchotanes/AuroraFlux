/***********************************************
* Filename:  		ParticleSystem.h
* Date:      		01/08/2013
* Mod. Date: 		01/08/2013
* Mod. Initials:	SW
* Author:    		Stephen Warren
* Purpose:   		This is the class that the Effect System will be using for particles			
************************************************/

#ifndef EFFECT_INFORMATION_H
#define EFFECT_INFORMATION_H

#include "../StdAfx.h"
#include "../Definitions.h"
#include "EffectSystem.h"


class CObjectManager;
class CAsssetManager;




class EffectInformation
{
	
	static vector<tEffectSystemCreationInfo> vtEffectInfo;
	static CObjectManager * m_pObjectManager;
public:
	//METHODS
	static bool LoadAllEffects( char* _cpFilePath, CAssetManager* _pAssetManager, CObjectManager* _pObjMag );

	static bool CreateEffect(Effects _eType, CEffectSystem* _pEffect, CEntity* pOwner, bool _bRepeatable, bool _bPlay ); 
	static void resetEffects();
};


#endif