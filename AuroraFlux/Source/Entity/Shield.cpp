#include "Shield.h"
#include "Enemy.h"
#include "Player.h"
#include "../Collision/Physics.h"
/***********************************************
 * Filename:  		Shield.cpp
 * Date:      		11/27/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan Schotanes
 * Purpose:   		This class will hold all the shield objects data
 ************************************************/

/*****************************************************************
 * ~CShield():		Overloaded Constructor will delete/ null all values
 * Ins:			    CEntity* _pOwner   		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/

CShield::CShield( CEntity* _pOwner ) : m_pOwner( _pOwner ), m_fTime(0.0f)
{
	m_nObjectType = eSHIELD;
	m_bActive = true;

	D3DXMATRIX d3dScaledMatrix;
	D3DXMatrixIdentity( &d3dScaledMatrix );
	D3DXMatrixScaling(&d3dScaledMatrix,10.0f,10.0f,10.0f);
	SetMatrix(&(*GetMatrix() * d3dScaledMatrix));
	m_fShieldPercent = 1.0f;
}

/*****************************************************************
 * CShield():		Default Constructor will set up his data members
 * Ins:			    None    		      
 * Outs:		    None
 * Returns:		    None
 * Mod. Date:		11/29/2012
 * Mod. Initials:	BS
 *****************************************************************/

CShield::~CShield( void )
{
	if(m_pBV != nullptr)
	{
		delete m_pBV;
		m_pBV = nullptr;
	}
}

/*****************************************************************
 * Update():		Will update its respective data, shields, and health.
 * Ins:			    fElapedTime    		      
 * Outs:		    None
 * Returns:		    void
 * Mod. Date:		11/29/2012
 * Mod. Initials:	AR
 *****************************************************************/

void CShield::Update( float fElapedTime )
{
	int _type = m_pOwner->GetType();

	if(m_pOwner->GetActive() == false)
	{
		SetActive(false);
	}

	if(GetIsHit())
	{
		m_fTime += fElapedTime;
	}
	
	Rotate(90, 0, 1,0 );

	if( GetIsHit( ) == false && m_pOwner->GetShieldHit( ) )
	{
		m_fTime = 0.0f;
		SetIsHit( true );
	}

	
	if( m_fTime >= 1.0f && GetIsHit( ) )
	{
		SetIsHit( false );
		m_pOwner->SetShieldHit( false );
		m_fTime = 0.0f;
	}
	// Finds owners type to be able to upcast
	switch( _type )
	{
	case eRED_ENEMY:
	case eYELLOW_ENEMY:
	case eBLUE_ENEMY:
		{
			if( ( ( CEnemy* ) ( m_pOwner ) )->GetShields( ) > 0)
			{
				SetIsVisable( true );
			}
			else
			{
				SetIsVisable( false );
			}
			break;
		}
	//case eRED_PLAYER: 
	//case eBLUE_PLAYER:
	//case eYELLOW_PLAYER:
	//	{
	//		if( ( ( CPlayer* ) ( m_pOwner ) )->GetShields( ) > 0 )
	//		{
	//			SetIsVisable( true );
	//		}
	//		else
	//		{
	//			SetIsVisable( false );
	//		}
	//		break;
	//	}
	}
	
	float tempPercent = ( ( (CEnemy*) m_pOwner )->GetShields( ) / 230.0f );

	if(tempPercent >= 1.0f)
		m_fShieldPercent = 0.7f;

	if(tempPercent <= (m_fShieldPercent + 0.3f))
	{
		m_fShieldPercent -= fElapedTime / 10.0f;

		if(tempPercent <= 0.0999f && m_fShieldPercent <= 0.0999f)
			m_fShieldPercent = 0.1f;

		if(tempPercent <= 0.0f)
			m_fShieldPercent = 0.0f;
	}


	//if( GetIsHit( ) )
	//{
	//	D3DXMATRIX tempOwnerMat = * m_pOwner->GetMatrix( );
	//
	//	D3DXMATRIX d3dScaledMatrix;
	//	D3DXMatrixIdentity( &d3dScaledMatrix );
	//
	//	//int _type = m_pOwner->GetType();
	//	//if (_type == eYELLOW_PLAYER || _type == eRED_PLAYER || _type == eBLUE_PLAYER )
	//	//{
	//	//	D3DXMatrixScaling( &d3dScaledMatrix, 2.0f, 2.0f, 2.0f );
	//	//}
	//	//else
	//	//{
	//		D3DXMatrixScaling( &d3dScaledMatrix, 8.0f, 8.0f, 8.0f );
	//	//}
	//
	//	D3DXVECTOR3 WorldRight( 1.0f, 0.0f, 0.0f );
	//
	//	D3DXVECTOR3 sourceXaxis = D3DXVECTOR3( tempOwnerMat._11, tempOwnerMat._12, tempOwnerMat._13 );
	//	D3DXVECTOR3 sourceZaxis = D3DXVECTOR3( tempOwnerMat._31, tempOwnerMat._32, tempOwnerMat._33 );
	//	D3DXVECTOR3 yAxis = ( ( CPlayer* ) ( m_pOwner ) )->GetOtherObjectPos( );
	//
	//	D3DXVec3Normalize( &yAxis, &yAxis );
	//
	//	D3DXVECTOR3 zAxis;	
	//	D3DXVec3Cross( &zAxis, &WorldRight, &yAxis );
	//	D3DXVec3Normalize( &zAxis, &zAxis );
	//
	//	D3DXVECTOR3 xAxis;	
	//	D3DXVec3Cross( &xAxis, &yAxis,  &zAxis );
	//	D3DXVec3Normalize( &xAxis, &xAxis );
	//
	//	D3DXMATRIX mm; 
	//	D3DXMatrixIdentity( &mm );
	//
	//	mm._11 = xAxis.x;
	//	mm._12 = xAxis.y;
	//	mm._13 = xAxis.z;
	//
	//	mm._21 = yAxis.x;
	//	mm._22 = yAxis.y;
	//	mm._23 = yAxis.z;
	//
	//	mm._31 = zAxis.x;
	//	mm._32 = zAxis.y;
	//	mm._33 = zAxis.z;
	//
	//	SetMatrix( &(  d3dScaledMatrix * ( mm * *m_pOwner->GetMatrix( ) ) ) ) ;
	//	SetQuadMatrix( &(  d3dScaledMatrix * *m_pOwner->GetMatrix( ) ) ) ;
	//}
	//else
	//{
		D3DXMATRIX d3dScaledMatrix;
		D3DXMatrixIdentity( &d3dScaledMatrix );
	//	//int _type = m_pOwner->GetType();
	//	
		SetMatrix(m_pOwner->GetMatrix());
	//	
	//
	//	//if (_type == eYELLOW_PLAYER || _type == eRED_PLAYER || _type == eBLUE_PLAYER )
	//	//{
	//	//	D3DXMatrixScaling( &d3dScaledMatrix, 2.0f, 2.0f, 2.0f );
	//	//}
	//	//else
	//	//{
	//		if(((CEnemy*)m_pOwner)->GetPlayer())
	//		{
	//			CPhysics::LookAt( this, *((CPlayer*)((CEnemy*)m_pOwner)->GetPlayer())->GetCamera()->GetCurrentMatrix());
	//		}
			D3DXMatrixScaling( &d3dScaledMatrix, 8.0f, 8.0f, 8.0f );
		//}

		SetMatrix( &( d3dScaledMatrix  * *GetMatrix() ) ) ;
		//SetQuadMatrix( &( *GetMatrix() ) ) ;
	//}
	//
	//if(_type == eBLUE_ENEMY)
	//	SetQuadMatrix( &( *GetMatrix() ) ) ;
	//else
	//	SetQuadMatrix(GetOwner()->GetMatrix());
}