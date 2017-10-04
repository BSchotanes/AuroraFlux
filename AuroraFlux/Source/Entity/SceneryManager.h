/***********************************************
* Filename:  		Entity.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be the base class all entities will inherit from.
************************************************/
#ifndef	CSCENERYMANAGER_H
#define CSCENERYMANAGER_H

/////// CHANGED

#include "EnvironmentObjects.h"
#include "../Object Manager/ObjectManager.h"
#include "../Entity/comet.h"

class CSceneryManager 
{
	CObjectManager * m_pObjectManager;

	D3DXVECTOR3 m_Position;
	CEnvironmentObject * currObject;
	vector<CEnvironmentObject*> m_vListOfAsteroids;
	vector<CComet*> m_vpListOfComets;
	bool m_bIsMainMenu;
public:
	
	/*****************************************************************
	* CEntity():		Overload Constructor Default Constructor will pass down to its 
	*					respective chlid and set the entites target to the passed in one.
	* Ins:			    _target    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CSceneryManager(CObjectManager * m_pObjectManager, bool _isMainMenu = false);
	~CSceneryManager(void);

	/*Destructor*/
	void CreateScene();
	CEnvironmentObject* AddSceneryObject(int _type, D3DXVECTOR3 _position, int _rotate = 0, float _scale = 1.0f, bool _hasAsteroidField = false, D3DXVECTOR3 _d3dOffset = D3DXVECTOR3(0,0,0), CEntity* _pOwner = 0, bool _isVisable = true);
	void CreateAsteroidField(CEnvironmentObject * _pOwner);
	void Update(float _time);
	void ScatterDebris();
	void HandleComets(float);
};

#endif