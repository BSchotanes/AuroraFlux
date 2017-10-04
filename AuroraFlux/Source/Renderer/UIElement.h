#ifndef	UIELEMENT_H
#define UIELEMENT_H
#include "../StdAfx.h"

/***********************************************
* Filename:  		UIElement.h
* Date:      		11/29/2012
* Mod. Date: 		11/29/2012
* Mod. Initials:	BS
* Author:    		Bryan W. Schotanes
* Purpose:   		This Class holds the UI Element quads and textures
*
************************************************/

class CUIElement 
{
private:
	D3DXMATRIX  m_d3dMatrix;
	int	    m_nModel;
	int	    m_nTexture1;
	int	    m_nTexture2;

public:
	/*****************************************************************
	* CUIElement():  Default Constructor
	* 			     
	*			     
	*			     
	*
	* Ins:						void
	*      		     
	*
	* Outs:						void
	*
	* Returns:					void
	*
	* Mod. Date:		      12/12/2012
	* Mod. Initials:	      BS
	*****************************************************************/

	CUIElement();

	/*****************************************************************
	* ~CUIElement():  Destructor
	* 			     
	*			     
	*			     
	*
	* Ins:						void
	*      		     
	*
	* Outs:						void
	*
	* Returns:					void
	*
	* Mod. Date:		      12/12/2012
	* Mod. Initials:	      BS
	*****************************************************************/

	~CUIElement();

	/***************
	*  Accessors
	***************/

	D3DXMATRIX* GetMatrix() { return &m_d3dMatrix; }
	int GetModel() const    { return m_nModel; }
	int GetTexture1() const { return m_nTexture1; }
	int GetTexture2() const { return m_nTexture2; }

	/***************
	* Mutators
	***************/

	void SetMatrix(D3DXMATRIX* _pVal) { m_d3dMatrix = *_pVal; }
	void SetModel(int _nVal)	  { m_nModel = _nVal; }
	void SetTexture1(int _nVal)       { m_nTexture1 = _nVal; }
	void SetTexture2(int _nVal)       { m_nTexture2 = _nVal; }
};

#endif