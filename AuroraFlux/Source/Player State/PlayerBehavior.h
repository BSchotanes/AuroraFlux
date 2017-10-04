/***********************************************
* Filename:  		CPlayerBehaviour.h
* Date:      		12/08/2012
* Mod. Date: 		12/08/2012
* Mod. Initials:	AG
* Author:    		Andrew Goossens
* Purpose:   		This class will be the base class all entities will inherit from.
************************************************/
#ifndef CPLAYERBEHAVIOR_H
#define CPLAYERBEHAVIOR_H

#include "IPlayerBehavior.h"
#include "../Entity/Player.h"
#include "../Input/Input.h"

class CPlayerBehavior : public IPlayerBehavior
{
	int	m_nType;
	int	m_nHealth;
	int	m_nShields;
	int	m_nDamageModifier;
	float	m_fVelocityModifier;
	bool	m_bIsActive;
	bool	m_bIsVisible;
	int	m_nModel;

public:
	/*****************************************************************
	* CPlayerBehavior():	Default Constructor will pass down to its respective chlid.
	* Ins:					None    		      
	* Outs:				None
	* Returns:				None
	* Mod. Date:			12/08/2012
	* Mod. Initials:		AG
	*****************************************************************/
	CPlayerBehavior(void);

	/*****************************************************************
	* CPlayerBehavior():	Overload Constructor Default Constructor will pass down to its 
	*						respective chlid and set the entites target to the passed in one.
	* Ins:					_player    		      
	* Outs:				None
	* Returns:				None
	* Mod. Date:			12/08/2012
	* Mod. Initials:		AG
	*****************************************************************/
	CPlayerBehavior(CPlayer* _pPlayer);

	/*****************************************************************
	* ~CPlayerBehavior():Default Destructor
	* Ins:			    None    		      
	* Outs:				None
	* Returns:		    None
	* Mod. Date:		11/29/2012
	* Mod. Initials:	AR
	*****************************************************************/
	virtual ~CPlayerBehavior(void);

	/*****************************************************************
	* Update():		Pass update to its children still.
	* Ins:			    _elapsedTime    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Update(float _fElapsedTime);

	/*****************************************************************
	* Render():		Pass render to its children still.
	* Ins:			    None    		      
	* Outs:		    None
	* Returns:		    void
	* Mod. Date:		12/08/2012
	* Mod. Initials:	AG
	*****************************************************************/
	void Render();

	/*Accessors*/
	int GetType() {return m_nType;}
	int GetHealth() {return m_nHealth;}
	int GetShields() {return m_nShields;}
	int GetDamage() {return m_nDamageModifier;}
	float GetVelocity() {return m_fVelocityModifier;}
	bool GetActive() {return m_bIsActive;}
	bool GetVisible() {return m_bIsVisible;}
	int GetModel() const { return m_nModel;}
	virtual int GetMaxShield() {return -1;}
	virtual int GetMaxHealth() {return -1;}

	/*Mutators*/
	void SetType(int _nType) {m_nType = _nType;}
	void SetHealth(int _nHealth) {m_nHealth = _nHealth;}
	void SetShields(int _nShields) {m_nShields = _nShields;}
	void SetDamage(int _nDamage) {m_nDamageModifier = _nDamage;}
	void SetVelocity(float _fVelocity) {m_fVelocityModifier = _fVelocity;}
	void SetActive(bool _bIsActive) {m_bIsActive = _bIsActive;}
	void SetVisible(bool _bIsVisible) {m_bIsVisible = _bIsVisible;}
	void SetModel(int _nModel) {m_nModel = _nModel;}
};
#endif