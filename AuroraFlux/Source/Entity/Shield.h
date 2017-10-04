/***********************************************
 * Filename:  		Shield.h
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan Schotanes
 * Purpose:   		This class will hold all the shield objects data
 ************************************************/
#ifndef	CSHIELD_H
#define CSHIELD_H

#include "Entity.h"

class CShield: public CEntity
{
	CEntity* m_pOwner;

	float		m_fTime;
	float		m_fShieldPercent;
	D3DXMATRIX m_d3dQuadMatrix;
	
public:
/*****************************************************************
 * CShield():		Default Constructor will set up his data members
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/
	CShield(void){;}
/*****************************************************************
 * CShield():		Overloaded Constructor will set up his data members
 * Ins:			    CEntity* _pOwner   		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/
	CShield(CEntity* _pOwner);

/*****************************************************************
 * ~CShield():		Overloaded Constructor will delete/ null all values
 * Ins:			    CEntity* _pOwner   		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/
	~CShield(void);
/*****************************************************************
 * Update():		Will update its respective data, shields, and health.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/
	void Update(float _fElapedTime);
/*****************************************************************
 * Render():		Draw effects or whatnot.
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/
	void Render() {;}

	/***************
	 *  Accessors
	 ***************/
	CEntity* GetOwner(void) {return m_pOwner;}
	float GetTime() const { return m_fTime; }
	D3DXMATRIX* GetQuadMatrix() { return &m_d3dQuadMatrix; }
	float GetShieldPercent() {return (m_fShieldPercent * 0.7f);}
	
	/***************
	 * Mutators
	 ***************/
	void SetOwner(CEntity* _pOwner) {m_pOwner = _pOwner;}
	void SetQuadMatrix(D3DXMATRIX* _pVal) { m_d3dQuadMatrix = *_pVal; }
	//void SetTime(float val) { m_fTime = val; }
};

#endif
