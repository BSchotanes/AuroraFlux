/***********************************************
* Filename:  		Entity.h
* Date:      		11/27/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	AR
* Author:    		Andrew A Rosser
* Purpose:   		This class will be the base class all entities will inherit from.
************************************************/
#ifndef	CENTITY_H
#define CENTITY_H

/////// CHANGED

#include "IEntity.h"
#include "../Renderer/CAssetManager.h"
#include "../Collision/CollOBJ.h"
class CShield;

class CEntity : public IEntity
{
protected:
	// Objects local matrix
	D3DXMATRIX			m_d3dMatrix;
	// Whether the object should be rendered or not
	bool				m_bActive;
	 
	// This entities target that it will chase and fire at
	CEntity				*m_pTarget;
	// what type of object each entity is
	int				m_nObjectType;
	// Bounding Volume Pointer
	CCollOBJ*			m_pBV;
	// send this to the object manager to fire weapons
	char				m_cFlags;
	//	mesh id
	int				m_nMesh;
	//on if the entity has taken damage
	bool				m_bIsHit;
	//on if the entity has taken reduced damage
	bool				m_bFasterIsHit;
	// on if the entity needs to flee
	bool				m_bShouldRun;
	//on if the entity is not cloaked
	bool				m_bIsVisable;
	// on if the shield has been hit
	bool				m_bShieldHit;
	// position 
	D3DXVECTOR3			m_d3dOtherObjectPos;
	//the entitys shield 
	CShield				*m_pEnemyShield;

	float				m_chTransitionValue;
	bool isScenery;
	int m_nEntityMass;
	D3DXVECTOR3 m_d3dVelocity;

	float m_fDistanceFromPlayer;
	

	int m_nMainMenuType;

	bool m_bIsTransparent;


public:
	int GetMainMenuType(){return m_nMainMenuType;}
	void SetMainMenuType(int _type){ m_nMainMenuType = _type;}

	bool GetIsTransparent(){return m_bIsTransparent;}
	void SeIsTransparent(bool _type){ m_bIsTransparent = _type;}
	/*****************************************************************
	* CEntity():		Default Constructor will pass down to its respective chlid.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CEntity(void);
	/*****************************************************************
	* CEntity():		Overload Constructor Default Constructor will pass down to its 
	*					respective chlid and set the entites target to the passed in one.
	* Ins:			    _target    		      
	* Outs:		    None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	CEntity(CEntity* _pTarget);
	/*Destructor*/
	virtual ~CEntity(void);
	/*****************************************************************
	* Update():		Pass update to its children still.
	* Ins:			    fElapedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Update(float _fElapsedTime);
	/*****************************************************************
	* Render():		Pass render to its children still.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Render()  ;
	/*****************************************************************
	* Rotate():		Will create a rotation matrix based on the degrees passed in and what axis you want.
	0 will not rotate that axis. 1 will rotate that axis.
	* Ins:			    _degrees, _x, _y, _z    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Rotate(float _nDegrees, int _nX, int _nY, int _nZ  )	;
	/*****************************************************************
	* Scale():			Will create a scale matrix based on the scale passed in 
	and will scale every axis by that much.
	* Ins:			    _scale    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Scale( D3DXVECTOR3 _d3dScale);
	/*****************************************************************
	* Scale():			Will create a scale matrix based on the scale passed in 
	and will scale each axis by that much.
	* Ins:			    _x, _y, _z    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	void Scale( float fX, float _fY, float _fZ);

	/*****************************************************************
	Accessors
	*****************************************************************/
	int GetType()  {return  m_nObjectType  ;}
	bool GetIsScenery() {return isScenery;}
	void SetIsScenery(bool _scene) {isScenery =  _scene;}
	bool GetActive() {return m_bActive;}
	CEntity* GetTarget() {return m_pTarget;}
	CShield* GetShield() {return m_pEnemyShield;}
	virtual CCollOBJ* GetBV() {return m_pBV;}
	D3DXMATRIX* GetMatrix()  {return  &m_d3dMatrix; }
	D3DXMATRIX GetMatrixCopy() {return m_d3dMatrix; }
	D3DXVECTOR3 GetPosition();
	char GetFlags() {return m_cFlags;}
	int GetMesh() const { return m_nMesh; }
	bool GetFasterIsHit() {return m_bFasterIsHit;}
	bool GetIsHit() {return m_bIsHit;}
	bool GetShouldRun() {return m_bShouldRun;}
	bool GetIsVisable() const { return m_bIsVisable; }
	bool GetShieldHit() const { return m_bShieldHit; }
	D3DXVECTOR3 GetOtherObjectPos() const { return m_d3dOtherObjectPos; }
	float GetTransitionValue() const { return m_chTransitionValue; }
	int GetEntityMass() {return m_nEntityMass;}
	D3DXVECTOR3 GetVelocity() {return m_d3dVelocity;}
	float GetDistanceFromPlayer() {return m_fDistanceFromPlayer;}
	// *****************************************************************//

	/*****************************************************************
	Mutators

	*****************************************************************/

	void SetBV(CCollOBJ* _pBV) {m_pBV = _pBV;}
	void SetType(int _nType)  {  m_nObjectType = _nType ;}
	void SetActive(bool _bActive) {m_bActive = _bActive;}
	void SetIsHit(bool _bIsHit) {m_bIsHit = _bIsHit;}
	void SetFasterIsHit(bool _bIsHit) {m_bFasterIsHit = _bIsHit;}
	void SetShouldRun(bool _bIsHit) {m_bShouldRun = _bIsHit;}
	void SetTarget(CEntity* _pTarget) {m_pTarget = _pTarget;}
	void SetShield(CShield* _pShield) {m_pEnemyShield = _pShield;}
	void SetMatrix(D3DXMATRIX* _d3dpType)  {  m_d3dMatrix = *_d3dpType ;}
	void SetPosition(D3DXVECTOR3 _d3dPosition);
	void SetPosition(float _fX, float _fY, float _fZ);
	void SetFlags( char _cFlag ) { m_cFlags = _cFlag;}
	void SetMesh(int _nID) {m_nMesh = _nID;}
	void SetIsVisable(bool _bVal) { m_bIsVisable = _bVal; }
	void SetShieldHit(bool _bVal) { m_bShieldHit = _bVal; }
	void SetOtherObjectPos(D3DXVECTOR3 _d3dVal) { m_d3dOtherObjectPos = _d3dVal; }
	void SetTransitionValue(float val) { m_chTransitionValue = val; }
	void SetEntityMass(int _nMass) {m_nEntityMass = _nMass;}
	void SetVelocity(D3DXVECTOR3 _d3dVelocity) {m_d3dVelocity = _d3dVelocity;}
	void SetDistanceFromPlayer(float _fDistanceFromPlayer) {m_fDistanceFromPlayer = _fDistanceFromPlayer;}
	// *****************************************************************//
};

#endif
