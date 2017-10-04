#include "CRenderer.h"
#include "../AI/Ambient Fighters/AmbientBoundingBox.h"
#include "../Entity/EnvironmentObjects.h"
/***********************************************
* Filename:  		CRenderer.cpp
* Date:      		11/29/2012
* Mod. Date: 		2/5/2013
* Mod. Initials:	BS
* Author:    		Bryan W. Schotanes
* Purpose:   		This is the class thats renders all objects
*
************************************************/

/*****************************************************************
* CRenderer():  
* 			      
*			      
*			      
*
* Ins:						Void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

CRenderer::CRenderer( ) : m_d3dPP( ),m_hWnd( ),m_d3dBackBufferDesc( ),m_pTime( NULL ), m_d3dpObject( NULL ),m_d3dpDevice( NULL ),
	m_pCamera( NULL ),m_d3dpSpriteManager( NULL ),m_d3dpQuadBuff( NULL ),m_d3dpQuadIndex( NULL ),
	m_d3dpSkyBoxTexture( NULL ),m_d3dpCubeVertBuffer( NULL ),m_d3dpCubeIndexBuffer( NULL ),m_d3dpQuadVertexDeclaration( NULL ),m_d3dpCubeVertexDeclaration( NULL ),m_chMenuShader( -1 ),
	m_chSkyBoxShader( -1 ),m_fTime(0),m_bIsPaused(false),m_pBoss(NULL),m_pBossTurret(NULL)
{
}

/*****************************************************************
* Init():  
* 			      
*			      
*			      
*
* Ins:						HWND _hWnd
*							float _nScreenWidth
*							float _nScreenHeight
*							bool _bIsWindowed
*							bool _bVsync
*      		      
*      		      
* Outs:						None
*
* Returns:					bool
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

bool CRenderer::Init( HWND _hWnd, float _fScreenWidth, float _fScreenHeight, bool _bIsWindowed, bool _bVsync, CCamera* _pCamera,CAssetManager* _pAssetManager )
{

	m_fXRatio = (_fScreenWidth / 1024.0f);
	m_fYRatio = (_fScreenHeight / 768.0f);

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					***** Setting Up Direct3D9 *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	if ( !_hWnd ) 
	{
		return false;
	}

	m_hWnd = _hWnd;
	m_d3dpObject = Direct3DCreate9( D3D_SDK_VERSION );

	if( !m_d3dpObject )
	{
		return false;
	}


	ZeroMemory( &m_d3dPP, sizeof( D3DPRESENT_PARAMETERS ) );

	m_d3dPP.BackBufferWidth				= ( int ) _fScreenWidth;
	m_d3dPP.BackBufferHeight			= ( int ) _fScreenHeight;
	m_d3dPP.BackBufferFormat			= ( _bIsWindowed ) ? D3DFMT_UNKNOWN : D3DFMT_A8R8G8B8;
	m_d3dPP.BackBufferCount				= 1;
	m_d3dPP.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_d3dPP.MultiSampleQuality			= 0;
	m_d3dPP.SwapEffect					= D3DSWAPEFFECT_COPY;
	m_d3dPP.hDeviceWindow				= _hWnd;
	m_d3dPP.Windowed					= _bIsWindowed;
	m_d3dPP.EnableAutoDepthStencil		= false;
	m_d3dPP.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	m_d3dPP.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	m_d3dPP.PresentationInterval		= ( _bVsync ) ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dPP.AutoDepthStencilFormat		= D3DFMT_D16;
	m_d3dPP.EnableAutoDepthStencil		= true;	

	if ( FAILED( m_d3dpObject->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _hWnd, D3DCREATE_MULTITHREADED | D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3dPP, &m_d3dpDevice) ) )
	{
		return false;
	}


	SetCamera( _pCamera );
	SetAssetManager(_pAssetManager);
	m_pAssetManager->SetDevice(m_d3dpDevice);

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					***** Creating Effect Pool *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	D3DXCreateEffectPool(&m_d3dpEffectPool);
	m_pAssetManager->SetEffectPool(m_d3dpEffectPool);

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//				 ***** Creating Sprite Manager *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	D3DXCreateSprite( m_d3dpDevice, &m_d3dpSpriteManager );

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					***** Creating Bitmap Font *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	m_ptBitmapfont = new CBitmapFont("Resources/Fonts/Neuropolitical.dat",m_pAssetManager->LoadTexture("NeuropoliticalBitmap.png"));

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//				***** Calculating BackBuffer Size *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	IDirect3DSurface9 *bBuffer = 0; 
	m_d3dpDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &bBuffer );
	bBuffer->GetDesc( &m_d3dBackBufferDesc );
	bBuffer->Release( );

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					  ***** Loading Shaders *****					 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	m_chGBufferShader = m_pAssetManager->LoadEffect("Gbuffers.fx");
	m_chPointLightShader = m_pAssetManager->LoadEffect("PointLight.fx");
	m_chPostProcessShader = m_pAssetManager->LoadEffect("PostProcess.fx");
	m_chMenuShader = m_pAssetManager->LoadEffect( "Menu.fx" );
	m_chFrustumShader = m_pAssetManager->LoadEffect("Frustum.fx");
	m_chFinalPassShader = m_pAssetManager->LoadEffect("FinalPass.fx");
	m_chLaserLightShader = m_pAssetManager->LoadEffect("LaserLight.fx");
	m_chTransparencyShader = m_pAssetManager->LoadEffect("Transparency.fx");

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					  ***** Loading Textures *****					 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	m_chButtonTexture = m_pAssetManager->LoadTexture("MenuButtons.png");
	m_chShieldTexture = m_pAssetManager->LoadTexture("LightningTexture.png");
	m_chWhiteTexture = m_pAssetManager->LoadTexture("WhiteTexture.png");
	m_chEnemyShieldTexture = m_pAssetManager->LoadTexture("Lightning2_Diffuse.png");

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					  ***** Creating Buffers *****					 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	m_DepthTarget.InitRenderTarget(m_d3dBackBufferDesc.Width,m_d3dBackBufferDesc.Height,m_d3dpDevice,D3DFMT_R32F);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("depthGBuffer",m_DepthTarget.GetTexture());

	m_DiffuseTarget.InitRenderTarget(m_d3dBackBufferDesc.Width, m_d3dBackBufferDesc.Height, m_d3dpDevice, D3DFMT_A8R8G8B8);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("diffuseGBuffer",m_DiffuseTarget.GetTexture());

	m_NormalTarget.InitRenderTarget(m_d3dBackBufferDesc.Width, m_d3dBackBufferDesc.Height, m_d3dpDevice, D3DFMT_A8R8G8B8);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("normalGBuffer",m_NormalTarget.GetTexture());

	m_SpecularTarget.InitRenderTarget(m_d3dBackBufferDesc.Width, m_d3dBackBufferDesc.Height, m_d3dpDevice, D3DFMT_A8R8G8B8);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("specularGBuffer",m_SpecularTarget.GetTexture());

	m_LightTarget.InitRenderTarget(m_d3dBackBufferDesc.Width, m_d3dBackBufferDesc.Height, m_d3dpDevice, D3DFMT_A8R8G8B8);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("lightBuffer",m_LightTarget.GetTexture());

	m_PostProcessTarget.InitRenderTarget(m_d3dBackBufferDesc.Width, m_d3dBackBufferDesc.Height, m_d3dpDevice, D3DFMT_A8R8G8B8);
	m_pAssetManager->GetEffect(m_chGBufferShader)->SetTexture("sourceTexture",m_PostProcessTarget.GetTexture());

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//				***** Creating SkyBox and Quads *****				 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	CreateSkyBox( );
	CreateQuad( );
	CreateFrustrum();

	///////////////////////////////////////////////////////////////////////
	//																	 //
	//					  ***** Creating Lights *****					 //
	//																	 //
	///////////////////////////////////////////////////////////////////////

	PointLight* tempPointLight = new PointLight();

	tempPointLight->SetName("PointLight0");
	tempPointLight->SetDiffuseColor(&D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0f));
	tempPointLight->SetSpecularColor(&D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));
	tempPointLight->SetAmbientColor(&D3DXVECTOR4(0.25f, 0.25f, 0.25f, 1.0f));
	tempPointLight->SetEnabled(true);
	tempPointLight->SetAttenuation(&D3DXVECTOR3(0.0f, 0.00015f, 0.000035f), 7200.0f);
	tempPointLight->SetSpecularIntensity(3.0f);
	tempPointLight->SetSpecularPower(66.0f);

	tempPointLight->SetMesh(m_pAssetManager->LoadMeshSphere("Light0",tempPointLight->GetRadius()));
	tempPointLight->SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_vLights.push_back(tempPointLight);

	PointLight* tempPointLight2 = new PointLight();

	tempPointLight2->SetName("PointLight0");
	tempPointLight2->SetDiffuseColor(&D3DXVECTOR4(1.0F, 0.0F, 0.0F, 1.0f));
	tempPointLight2->SetSpecularColor(&D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	tempPointLight2->SetAmbientColor(&D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
	tempPointLight2->SetEnabled(true);
	tempPointLight2->SetAttenuation(&D3DXVECTOR3(0.0f, 0.0015f, 0.00035f), 150.0f);
	tempPointLight2->SetSpecularIntensity(3.0f);
	tempPointLight2->SetSpecularPower(66.0f);

	tempPointLight2->SetMesh(m_pAssetManager->LoadMeshSphere("Light1",tempPointLight2->GetRadius()));
	tempPointLight2->SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_vLights.push_back(tempPointLight2);

	PointLight* tempPointLight3 = new PointLight();

	tempPointLight3->SetName("PointLight0");
	tempPointLight3->SetDiffuseColor(&D3DXVECTOR4(1.0F, 0.0F, 1.0F, 1.0f));
	tempPointLight3->SetSpecularColor(&D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	tempPointLight3->SetAmbientColor(&D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
	tempPointLight3->SetEnabled(true);
	tempPointLight3->SetAttenuation(&D3DXVECTOR3(0.0f, 0.00000015f, 0.000000035f), 24000.0f);
	tempPointLight3->SetSpecularIntensity(3.0f);
	tempPointLight3->SetSpecularPower(66.0f);

	tempPointLight3->SetMesh(m_pAssetManager->LoadMeshSphere("Light2",tempPointLight3->GetRadius()));
	tempPointLight3->SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_vLights.push_back(tempPointLight3);


	PointLight* tempPointLight4 = new PointLight();

	tempPointLight4->SetName("PointLight0");
	tempPointLight4->SetDiffuseColor(&D3DXVECTOR4(0.0F, 1.0F, 0.0F, 1.0f));
	tempPointLight4->SetSpecularColor(&D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f));
	tempPointLight4->SetAmbientColor(&D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f));
	tempPointLight4->SetEnabled(true);
	tempPointLight4->SetAttenuation(&D3DXVECTOR3(0.0f, 0.0000095f, 0.0000115f), 4200.0f);
	tempPointLight4->SetSpecularIntensity(3.0f);
	tempPointLight4->SetSpecularPower(66.0f);

	tempPointLight4->SetMesh(m_pAssetManager->LoadMeshSphere("Light3",tempPointLight4->GetRadius()));
	tempPointLight4->SetPosition(D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_vLights.push_back(tempPointLight4);

	m_fGammaValue = 0.0f;

	return true;
}

void CRenderer::Clear(long _lWhatToClear,unsigned char _cRed, unsigned char _cGreen, unsigned char _cBlue)
{
	m_d3dpDevice->Clear(0,0,_lWhatToClear, D3DCOLOR_XRGB(_cRed,_cGreen,_cBlue),1.0f,0);
}


void CRenderer::RenderObjects(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects, const vector<CEffectSystem*>& _vpEffects,const vector<CEntity *> * _vpTransparentObjects)
{
	if(!m_bIsPaused)
	{
		m_fTime += m_pTime->GetDeltaTime( );
		RenderGBuffers(_vpRenderableObjects,_vpRenderableAmbientObjects);

		m_LightTarget.ActiveTarget(0);
		Clear(D3DCLEAR_TARGET,0,0,0);

		RenderLights(_vpRenderableObjects,_vpRenderableAmbientObjects);

		RenderTransparency(_vpRenderableObjects,_vpTransparentObjects,m_chTransparencyShader,"Technique",_vpEffects );
		m_LightTarget.RevertTarget();
	}

	RenderFowardPass();
}

void CRenderer::RenderMainMenuObjects(const vector<CEntity *> * _vpRenderableObjects, const vector<CEffectSystem*>& _vpEffects)
{
	if(!m_bIsPaused)
	{
		m_fTime += m_pTime->GetDeltaTime( );
		RenderMainMenuGBuffers(_vpRenderableObjects);

		m_LightTarget.ActiveTarget(0);
		Clear(D3DCLEAR_TARGET,0,0,0);

		RenderMainMenuLights(_vpRenderableObjects);

		RenderMainMenuTransparency(_vpRenderableObjects,m_chTransparencyShader,"Technique",_vpEffects);
		m_LightTarget.RevertTarget();
	}

	RenderFowardPass();
}
/*****************************************************************
* RenderObjects():  
* 			      
*			      
*			      
*
* Ins:						const vector<CEntity *> * _vpRenderableObjects
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderAllTargets(const vector<CEntity*> *_vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects,int _shader, string _technique)
{
	m_bBossFound = false;

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_shader);

	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	for(unsigned int AmbientIter = 0; AmbientIter < _vpRenderableAmbientObjects->size() ; AmbientIter++)
	{
		CAmbientBoundingBox* pTempBoundingBox = (*_vpRenderableAmbientObjects )[AmbientIter];

		for( unsigned int nRenderIter = 0; nRenderIter < pTempBoundingBox->GetFighters().size(); nRenderIter++ )
		{
			CEntity* pTempEntity = pTempBoundingBox->GetFighters()[nRenderIter];

			if(pTempEntity->GetBV()->GetObjType() == eSPH)
			{
				CSphere temp;
				temp.SetPosition(CPhysics::GetMat4Vec(ePOS,pTempEntity->GetMatrix()));
				temp.SetRadius(((CSphere*)pTempEntity->GetBV())->GetRadius());

				if(CCollisionLib::FrustumToSphere(*m_pCamera->GetFrustum(),temp) == false)
					continue;
			}
			else if(pTempEntity->GetBV()->GetObjType() == eCAP)
			{
				if(CCollisionLib::FrustumToCapsule(*m_pCamera->GetFrustum(),*(CCapsule*)pTempEntity->GetBV()) == false)
					continue;
			}

			int nType = pTempEntity->GetType();

			if(nType == eAMBIENT_ENEMY)
				nType = eAMBIENT_ENEMY_LARGE;

			pTempEffect->SetTechnique(_technique.c_str());

			CAssetManager::tModel* pModel = m_pAssetManager->GetModel( nType );



			if( pTempEntity->GetIsVisable() == true )
			{
				pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
				pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
				pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

				pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
				pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
				pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

				m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
				m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
				m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

				unsigned int nPasses = 0;
				pTempEffect->Begin( &nPasses, 0 );
				{
					for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
					{
						pTempEffect->BeginPass( nPassIter );

						m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

						pTempEffect->EndPass( );
					}
				}
				pTempEffect->End( );
			}
		}
	}

	for( unsigned int nRenderIter = 0; nRenderIter < _vpRenderableObjects->size( ); nRenderIter++ )
	{

		CEntity* pTempEntity = ( *_vpRenderableObjects )[ nRenderIter ];

		if( pTempEntity == NULL)
			continue;

		if(pTempEntity->GetType() == eCAM_OBJ || pTempEntity->GetType() == eBOSSTHRUSTER)
			continue;

		int nType = pTempEntity->GetType();

		pTempEffect->SetTechnique(_technique.c_str());

		CAssetManager::tModel* pModel = m_pAssetManager->GetModel( nType );

		float fEdgeColor = 0.0f;

		if(nType == eRED_PLAYER || nType == eBLUE_PLAYER || nType == eYELLOW_PLAYER)
		{
			if(nType == eRED_PLAYER)
			{
				fEdgeColor = 0.8f;
			}
			else
			{
				fEdgeColor = 0.4f;
			}

			string playerShaderString = "player";
			playerShaderString += _technique.c_str();
			pTempEffect->SetTechnique(playerShaderString.c_str());
			m_pPlayer = pTempEntity;

			nType = eRED_PLAYER;
		}
		else if(nType == ePLANET2)
		{
			string SunShaderString = "Sun";
			SunShaderString += _technique.c_str();
			pTempEffect->SetTechnique(SunShaderString.c_str());
			fEdgeColor = 0.0f;
		}
		else if(nType == eBOSS)
		{
			m_pBoss = pTempEntity;
			m_bBossFound = true;
			fEdgeColor = 0.0f;
		}
		else if(nType == eHanger)
		{
			m_pBossTurret = pTempEntity;
			fEdgeColor = 0.0f;
		}
		else if(nType == eSHIELD)
		{
			bool shieldExists = false;
			for(unsigned int i = 0; i < m_vShields.size();i++)
			{
				if(m_vShields[i] == (CShield*)pTempEntity)
				{
					shieldExists = true;
					continue;
				}
			}

			if(shieldExists == false)
			{
				m_vShields.push_back((CShield*)pTempEntity);
			}

			continue;
		}
		else if(nType == eASTEROID)
		{
			if (pTempEntity->GetIsScenery() )
			{
				pModel = m_pAssetManager->GetModel( ((CEnvironmentObject*)pTempEntity)->GetAsteroidType() );
			}
			else
			{
				pModel = m_pAssetManager->GetModel( ((CAsteroid*)pTempEntity)->GetAsteroidType() );
			}
			fEdgeColor = 0.0f;
		}
		else if(nType == eGENERATOR)
		{
			string generatorShaderString = "generator";
			generatorShaderString += _technique.c_str();
			pTempEffect->SetTechnique(generatorShaderString.c_str());
			pTempEffect->SetFloat("gPercent",((CEnemy*)pTempEntity)->GetHealth()/600.0f);
			fEdgeColor = 0.8f;
		}
		else if(nType == eWAYPOINT)
		{
			string waypointShaderString = "Wireframe";
			waypointShaderString += _technique.c_str();
			pTempEffect->SetTechnique(waypointShaderString.c_str());
			fEdgeColor = 0.6f;
		}
		else if(nType == eRED_ENEMY)
		{
			fEdgeColor = 0.8f;
		}
		else if(nType == eBLUE_ENEMY)
		{
			fEdgeColor = 0.4f;
		}
		else if(nType == eYELLOW_ENEMY)
		{
			fEdgeColor = 0.2f;
		}
		else if(nType == eLASER)
		{
			fEdgeColor = 1.0f;
		}
		else if(nType == eMISSILE)
		{
			fEdgeColor = 0.4f;
		}
		else
		{
			fEdgeColor = 0.0f;
		}

		if( pTempEntity->GetIsVisable() == true )
		{
			pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
			pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
			pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

			pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
			pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
			pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );
			pTempEffect->SetMatrix("gViewInverse",&CameraInverse);
			pTempEffect->SetFloat("gTransitionValue",pTempEntity->GetTransitionValue());
			pTempEffect->SetFloat("gEdgeColor",fEdgeColor);
			pTempEffect->SetBool( "gIsHit", pTempEntity->GetIsHit( ) );

			m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
			m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
			m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

			unsigned int nPasses = 0;
			pTempEffect->Begin( &nPasses, 0 );
			{
				for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
				{
					pTempEffect->BeginPass( nPassIter );

					m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

					pTempEffect->EndPass( );
				}
			}
			pTempEffect->End( );
		}
	}

	if(m_bBossFound == false)
	{
		m_pBoss = NULL;
	}

}

void CRenderer::RenderMainMenuAllTargets(const vector<CEntity *> * _vpRenderableObjects,int _Shader, string _Technique)
{
	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_Shader);

	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	for( unsigned int nRenderIter = 0; nRenderIter < _vpRenderableObjects->size( ); nRenderIter++ )
	{

		CEntity* pTempEntity = ( *_vpRenderableObjects )[ nRenderIter ];

		if( pTempEntity == NULL)
			continue;

		if(pTempEntity->GetType() == eCAM_OBJ || pTempEntity->GetType() == eBOSSTHRUSTER)
			continue;

		int nType = pTempEntity->GetMainMenuType();

		pTempEffect->SetTechnique(_Technique.c_str());

		CAssetManager::tModel* pModel = m_pAssetManager->GetMainMenuModel( nType );

		float fEdgeColor = 0.0f;


		if( pTempEntity->GetIsVisable() == true )
		{
			pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
			pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
			pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

			pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
			pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
			pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );
			pTempEffect->SetMatrix("gViewInverse",&CameraInverse);
			pTempEffect->SetFloat("gTransitionValue",pTempEntity->GetTransitionValue());
			pTempEffect->SetFloat("gEdgeColor",fEdgeColor);
			pTempEffect->SetBool( "gIsHit", pTempEntity->GetIsHit( ) );

			m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
			m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
			m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

			unsigned int nPasses = 0;
			pTempEffect->Begin( &nPasses, 0 );
			{
				for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
				{
					pTempEffect->BeginPass( nPassIter );

					m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

					pTempEffect->EndPass( );
				}
			}
			pTempEffect->End( );
		}
	}
}

void CRenderer::CreateFrustrum()
{
	D3DVERTEXELEMENT9 vertclr_definition[ ]=
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	m_d3dpDevice->CreateVertexDeclaration( vertclr_definition, &m_d3dpFrustumVertexDeclaration );

	tFrustum* temp = m_pCamera->GetFrustum();

	struct tVERTPOS arrtScreenVerts[ ] =
	{
		temp->m_d3darrCorners[eFTL],
		temp->m_d3darrCorners[eFBL],
		temp->m_d3darrCorners[eFBR],
		temp->m_d3darrCorners[eFTR],
		temp->m_d3darrCorners[eNTL],
		temp->m_d3darrCorners[eNTR],
		temp->m_d3darrCorners[eNBR],
		temp->m_d3darrCorners[eNBL],
	};	

	short arrsScreenIndicies[ ] = 
	{
		//	Front Face
		eFTL,eFTR,eFBL,
		eFTR,eFBR,eFBL,
		//	Back Face
		eNTR,eNTL,eNBR,
		eNTL,eNBL,eNBR,
		//	Left Face
		eNTL,eFTL,eNBL,
		eFTL,eFBL,eNBL,
		//	Right Face
		eFTR,eNTR,eFBR,
		eNTR,eNBR,eFBR,
		//	Top Face
		eFTR,eNTR,eNTL,
		eNTL,eFTL,eFTR,
		//	Bottom Face
		eFBL,eFBR,eNBL,
		eFBR,eNBR,eNBL,
	};
	void * pMem = 0;

	m_d3dpDevice->CreateVertexBuffer(unsigned int( sizeof(tVERTPOS) * 8), 0, 0, D3DPOOL_MANAGED, &m_d3dpFrustumBuff, 0);
	pMem = 0;

	m_d3dpFrustumBuff->Lock(0,0,&pMem,0);
	memcpy( pMem, arrtScreenVerts, 8 * sizeof( tVERTPOS ));
	m_d3dpFrustumBuff->Unlock();

	m_d3dpDevice->CreateIndexBuffer( sizeof( short ) * 24, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_d3dpFrustumIndex, 0);
	pMem = 0;

	m_d3dpFrustumIndex->Lock( 0, 0, &pMem, 0);
	memcpy( pMem, arrsScreenIndicies, 24 * sizeof( short ));
	m_d3dpFrustumIndex->Unlock();
}

void CRenderer::RenderFrustrum()
{
	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect( m_chFrustumShader );

	pTempEffect->SetMatrix( "gMVP", &( *m_pPlayer->GetMatrix() * ( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) ) );

	m_d3dpDevice->SetVertexDeclaration( m_d3dpFrustumVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, m_d3dpFrustumBuff, 0, sizeof( tVERTPOS ) );
	m_d3dpDevice->SetIndices( m_d3dpFrustumIndex );

	unsigned int nPasses = 0;
	pTempEffect->Begin( &nPasses, 0 );
	{
		for(unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter)
		{
			pTempEffect->BeginPass( nPassIter );

			m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 24, 0, 8 );

			pTempEffect->EndPass( );
		}
	}
	pTempEffect->End( );

}
/*****************************************************************
* RenderGBuffers():  This Function will draw the pass 
* 			      
*			      
*			      
*
* Ins:						const vector<CEntity *> * _vpRenderableObjects
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderGBuffers(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects)
{

	m_DiffuseTarget.ActiveTarget(0);
	m_NormalTarget.ActiveTarget(1);
	m_SpecularTarget.ActiveTarget(2);
	m_DepthTarget.ActiveTarget(3);

	Clear(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,255,255,255);

	RenderAllTargets(_vpRenderableObjects,_vpRenderableAmbientObjects,m_chGBufferShader,"gbuffersTechnique");

	m_DiffuseTarget.RevertTarget();
	m_NormalTarget.RevertTarget();
	m_SpecularTarget.RevertTarget();
	m_DepthTarget.RevertTarget();
}

void CRenderer::RenderMainMenuGBuffers(const vector<CEntity *> * _vpRenderableObjects)
{

	m_DiffuseTarget.ActiveTarget(0);
	m_NormalTarget.ActiveTarget(1);
	m_SpecularTarget.ActiveTarget(2);
	m_DepthTarget.ActiveTarget(3);

	Clear(D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,255,255,255);

	RenderMainMenuAllTargets(_vpRenderableObjects,m_chGBufferShader,"gbuffersTechnique");

	m_DiffuseTarget.RevertTarget();
	m_NormalTarget.RevertTarget();
	m_SpecularTarget.RevertTarget();
	m_DepthTarget.RevertTarget();
}

void CRenderer::RenderLights(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects)
{	
	D3DXVECTOR3 tempPosition = D3DXVECTOR3(m_pCamera->GetCurrentMatrix()->_41,m_pCamera->GetCurrentMatrix()->_42,m_pCamera->GetCurrentMatrix()->_43);

	D3DXMATRIX CameraInverse;

	D3DXMatrixIdentity(&CameraInverse);
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	Clear( D3DCLEAR_TARGET | D3DCLEAR_STENCIL,0,0,0);

	D3DXMATRIX invViewProj;

	D3DXMatrixIdentity(&invViewProj);
	D3DXMatrixInverse(&invViewProj, 0, &( CameraInverse* *m_pCamera->GetProjectionMatrix()));

	ID3DXEffect* PointLightEffect = m_pAssetManager->GetEffect(m_chPointLightShader);
	ID3DXEffect* LaserLightEffect = m_pAssetManager->GetEffect(m_chLaserLightShader);

	int nObjectType = -1;
	int lightIter = 0;

	PointLightEffect->SetMatrix("gInvViewProj",&invViewProj);
	PointLightEffect->SetVector("gHalfPixel", &D3DXVECTOR4(0.5f / m_d3dBackBufferDesc.Width, 0.5f / m_d3dBackBufferDesc.Height, 0, 1));
	PointLightEffect->SetVector("gCameraPos",&D3DXVECTOR4(tempPosition,1.0f));

	// Main Light
	if(m_vLights[0]->GetEnabled())
	{
		lightIter = 0;

		m_vLights[lightIter]->SetPosition(D3DXVECTOR3(tempPosition.x,tempPosition.y,tempPosition.z));

		m_vLights[lightIter]->ApplyLight(PointLightEffect);


		D3DXMATRIX world = *m_vLights[lightIter]->GetWorldMatrixPtr();
		D3DXMATRIX proj = *m_pCamera->GetProjectionMatrix();
		D3DXMATRIX final = (world * CameraInverse * proj);

		PointLightEffect->SetMatrix("gMVP",&final);

		unsigned int passes = 0;
		PointLightEffect->Begin(&passes,0);
		for(unsigned int passIter = 0; passIter < passes; ++passIter)
		{
			PointLightEffect->BeginPass(passIter);	

			m_pAssetManager->GetMesh( m_vLights[lightIter]->GetMeshNumber() )->m_d3dpMesh->DrawSubset( 0 );

			PointLightEffect->EndPass();
		}
		PointLightEffect->End();

	}
	//Other Lights

	for(unsigned int objectIter = 0 ; objectIter < _vpRenderableObjects->size(); objectIter++ )
	{
		Clear(D3DCLEAR_STENCIL,0,0,0);
		CEntity* pTempEntity = ( *_vpRenderableObjects )[ objectIter ];
		nObjectType = pTempEntity->GetType();

		switch (nObjectType)
		{
#ifdef _DEBUG
#else
		case eLASER:
			{
				if(((CLaser*)pTempEntity)->GetOwner() == m_pPlayer)
				{
					lightIter = 1;
					LaserLightEffect->SetTechnique("RenderPointLight2");
					m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(1.0F, 0.0F, 0.0F, 1.0f));

					D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
					m_vLights[lightIter]->SetPosition(tempObjectPosition);

					LaserLightEffect->SetFloat("gTime",0.0f);
				}
				else
				{
					continue;
				}

				break;
			}
#endif
		case eMISSILE:
			{
				lightIter = 1;
				LaserLightEffect->SetTechnique("RenderPointLight2");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 0.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("gTime",0.0f);
				break;
			}
		case eTURRET_LASER:
			{
				lightIter = 1;
				LaserLightEffect->SetTechnique("RenderPointLight2");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 0.75F, 0.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",m_fTime-((int)m_fTime));
				break;
			}
		case eTURRET_MISSILE:
			{
				lightIter = 1;
				LaserLightEffect->SetTechnique("RenderPointLight2");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 0.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",m_fTime-((int)m_fTime));
				break;
			}
		case eBLUE_PLAYER:
			{
				lightIter = 1;
				LaserLightEffect->SetTechnique("RenderPointLight2");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 0.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				tempObjectPosition.z -= 10.0f;
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		case eRED_PLAYER:
			{
				lightIter = 1;
				LaserLightEffect->SetTechnique("RenderPointLight2");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(1.0F, 0.0F, 0.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				tempObjectPosition.z -= 10.0f;

				m_vLights[lightIter]->SetPosition(tempObjectPosition);
				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		case eSPACE_STATION:
			{
				lightIter = 2;
				LaserLightEffect->SetTechnique("RenderPointLight1");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				tempObjectPosition.y += 100.0f;
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		case eSPACESTATION1:
			{
				lightIter = 3;
				LaserLightEffect->SetTechnique("RenderPointLight1");
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		default:
			{
				continue;
			}
		}

		m_vLights[lightIter]->ApplyLight(LaserLightEffect);

		D3DXMATRIX world = *m_vLights[lightIter]->GetWorldMatrixPtr();
		D3DXMATRIX proj = *m_pCamera->GetProjectionMatrix();
		D3DXMATRIX final = (world * CameraInverse * proj);

		LaserLightEffect->SetMatrix("gMVP",&final);

		unsigned int passes = 0;
		LaserLightEffect->Begin(&passes,0);
		for(unsigned int passIter = 0; passIter < passes; ++passIter)
		{
			LaserLightEffect->BeginPass(passIter);	
			m_pAssetManager->GetMesh( m_vLights[lightIter]->GetMeshNumber() )->m_d3dpMesh->DrawSubset( 0 );
			LaserLightEffect->EndPass();
		}
		LaserLightEffect->End();
	}
}

void CRenderer::RenderMainMenuLights(const vector<CEntity *> * _vpRenderableObjects)
{
	D3DXVECTOR3 tempPosition = D3DXVECTOR3(m_pCamera->GetCurrentMatrix()->_41,m_pCamera->GetCurrentMatrix()->_42,m_pCamera->GetCurrentMatrix()->_43);

	D3DXMATRIX CameraInverse;

	D3DXMatrixIdentity(&CameraInverse);
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	Clear(D3DCLEAR_TARGET,0,0,0);

	D3DXMATRIX invViewProj;

	D3DXMatrixIdentity(&invViewProj);
	D3DXMatrixInverse(&invViewProj, 0, &( CameraInverse* *m_pCamera->GetProjectionMatrix()));

	ID3DXEffect* PointLightEffect = m_pAssetManager->GetEffect(m_chPointLightShader);
	ID3DXEffect* LaserLightEffect = m_pAssetManager->GetEffect(m_chLaserLightShader);

	int nObjectType = -1;
	int lightIter = 0;

	PointLightEffect->SetMatrix("gInvViewProj",&invViewProj);
	PointLightEffect->SetVector("gHalfPixel", &D3DXVECTOR4(0.5f / m_d3dBackBufferDesc.Width, 0.5f / m_d3dBackBufferDesc.Height, 0, 1));
	PointLightEffect->SetVector("gCameraPos",&D3DXVECTOR4(tempPosition,1.0f));

	// Main Light
	if(m_vLights[0]->GetEnabled())
	{
		lightIter = 0;

		m_vLights[lightIter]->SetPosition(D3DXVECTOR3(tempPosition.x,tempPosition.y,tempPosition.z));

		m_vLights[lightIter]->ApplyLight(PointLightEffect);


		D3DXMATRIX world = *m_vLights[lightIter]->GetWorldMatrixPtr();
		D3DXMATRIX proj = *m_pCamera->GetProjectionMatrix();
		D3DXMATRIX final = (world * CameraInverse * proj);

		PointLightEffect->SetMatrix("gMVP",&final);

		unsigned int passes = 0;
		PointLightEffect->Begin(&passes,0);
		for(unsigned int passIter = 0; passIter < passes; ++passIter)
		{
			PointLightEffect->BeginPass(passIter);	

			m_pAssetManager->GetMesh( m_vLights[lightIter]->GetMeshNumber() )->m_d3dpMesh->DrawSubset( 0 );

			PointLightEffect->EndPass();
		}
		PointLightEffect->End();

	}
	//Other Lights

	for(unsigned int objectIter = 0 ; objectIter < _vpRenderableObjects->size(); objectIter++ )
	{
		CEntity* pTempEntity = ( *_vpRenderableObjects )[ objectIter ];
		nObjectType = pTempEntity->GetMainMenuType();

		switch (nObjectType)
		{
		case eTURRET_LASER:
			{
				lightIter = 1;

				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 0.75F, 0.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",m_fTime-((int)m_fTime));
				break;
			}
		case 6:
			{
				lightIter = 2;
				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(1.0F, 1.0F, 1.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				tempObjectPosition.y += 100.0f;
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		case eSPACESTATION1:
			{
				lightIter = 3;

				m_vLights[lightIter]->SetDiffuseColor(&D3DXVECTOR4(0.0F, 1.0F, 0.0F, 1.0f));

				D3DXVECTOR3 tempObjectPosition = pTempEntity->GetPosition();
				m_vLights[lightIter]->SetPosition(tempObjectPosition);

				LaserLightEffect->SetFloat("fTime",0.0f);
				break;
			}
		default:
			{
				continue;
			}
		}

		m_vLights[lightIter]->ApplyLight(LaserLightEffect);

		D3DXMATRIX world = *m_vLights[lightIter]->GetWorldMatrixPtr();
		D3DXMATRIX proj = *m_pCamera->GetProjectionMatrix();
		D3DXMATRIX final = (world * CameraInverse * proj);

		LaserLightEffect->SetMatrix("gMVP",&final);

		unsigned int passes = 0;
		LaserLightEffect->Begin(&passes,0);
		for(unsigned int passIter = 0; passIter < passes; ++passIter)
		{
			LaserLightEffect->BeginPass(passIter);	
			m_pAssetManager->GetMesh( m_vLights[lightIter]->GetMeshNumber() )->m_d3dpMesh->DrawSubset( 0 );
			LaserLightEffect->EndPass();
		}
		LaserLightEffect->End();
	}


}
/*****************************************************************
* RenderFowardPass():  This function will render everything to the screen quad  
* 			      
*			      
*			      
*
* Ins:						const vector<CEntity *> * _vpRenderableObjects
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderFowardPass()
{
	RenderPostProcess();

	ID3DXEffect* finalEffect = m_pAssetManager->GetEffect(m_chFinalPassShader);
	finalEffect->SetTechnique("finalPassTechnique");

	unsigned int passes = 0;

	m_d3dpDevice->SetVertexDeclaration(m_d3dpQuadVertexDeclaration);
	m_d3dpDevice->SetStreamSource(0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	m_d3dpDevice->SetIndices(m_d3dpQuadIndex);

	finalEffect->SetFloat("gGamma", m_fGammaValue);

	finalEffect->Begin(&passes,0);
	for(unsigned int j = 0; j < passes; ++j)
	{
		finalEffect->BeginPass(j);

		m_d3dpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		finalEffect->EndPass();
	}

	finalEffect->End();
}

void CRenderer::RenderPostProcess()
{
	m_PostProcessTarget.ActiveTarget(0);
	Clear(D3DCLEAR_TARGET,255,0,255);

	ID3DXEffect* postEffect = m_pAssetManager->GetEffect(m_chPostProcessShader);
	postEffect->SetTechnique("PostProcessGreyScale");

	m_d3dpDevice->SetVertexDeclaration(m_d3dpQuadVertexDeclaration);
	m_d3dpDevice->SetStreamSource(0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	m_d3dpDevice->SetIndices(m_d3dpQuadIndex);

	unsigned int passes = 0;
	postEffect->Begin(&passes,0);
	for(unsigned int j = 0; j < passes; ++j)
	{
		postEffect->BeginPass(j);

		m_d3dpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		postEffect->EndPass();
	}

	postEffect->End();
	m_PostProcessTarget.RevertTarget();

	//	Bloom
	//	m_FinalTarget.ActiveTarget(0);
	//
	//	Clear(D3DCLEAR_TARGET,255,0,255);
	//
	//	postEffect->SetTechnique("PostProcessBloom");
	//
	//	m_d3dpDevice->SetVertexDeclaration(m_d3dpQuadVertexDeclaration);
	//	m_d3dpDevice->SetStreamSource(0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	//	m_d3dpDevice->SetIndices(m_d3dpQuadIndex);
	//
	//	passes = 0;
	//	postEffect->Begin(&passes,0);
	//	for(unsigned int j = 0; j < passes; ++j)
	//	{
	//		postEffect->BeginPass(j);
	//
	//		m_d3dpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//
	//		postEffect->EndPass();
	//	}
	//
	//	postEffect->End();
	//
	//	m_FinalTarget.RevertTarget();


	//	*** FOG *** 
	//	m_PostProcessTarget.ActiveTarget(0);
	//	m_d3dpDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,0,255),1,0);
	//	
	//	ID3DXEffect* postEffect = m_pAssetManager->GetEffect(m_chPostProcessShader);
	//	postEffect->SetTechnique("PostProcessFog");
	//	D3DXMATRIX CameraInverse;
	//	
	//	D3DXMatrixIdentity(&CameraInverse);
	//	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );
	//	
	//	D3DXMATRIX invViewProj;
	//	
	//	D3DXMatrixIdentity(&invViewProj);
	//	D3DXMatrixInverse(&invViewProj, 0, &( CameraInverse* *m_pCamera->GetProjectionMatrix()));
	//	
	//	postEffect->SetMatrix("gInvViewProj",&invViewProj);
	//	D3DXVECTOR3 tempPosition = D3DXVECTOR3(m_pCamera->GetCurrentMatrix()->_41,m_pCamera->GetCurrentMatrix()->_42,m_pCamera->GetCurrentMatrix()->_43);
	//	
	//	postEffect->SetVector("gCameraPos",&D3DXVECTOR4(tempPosition,1.0f));
	//	
	//	// TODO BERN
	//	postEffect->SetFloat("gNear",eNEAR_PLANE);
	//	postEffect->SetFloat("gFar",eFAR_PLANE);
	//	
	//	m_d3dpDevice->SetVertexDeclaration(m_d3dpQuadVertexDeclaration);
	//	m_d3dpDevice->SetStreamSource(0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	//	m_d3dpDevice->SetIndices(m_d3dpQuadIndex);
	//	
	//	unsigned int passes = 0;
	//	postEffect->Begin(&passes,0);
	//	for(unsigned int j = 0; j < passes; ++j)
	//	{
	//	postEffect->BeginPass(j);
	//	
	//	m_d3dpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//	
	//	postEffect->EndPass();
	//	}
	//	
	//	postEffect->End();
	//	m_PostProcessTarget.RevertTarget();

	//*** MOTION BLUR *** 
	//m_PostProcessTarget.ActiveTarget(0);
	//m_d3dpDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,0,255),1,0);
	//
	//ID3DXEffect* postEffect = m_pAssetManager->GetEffect(m_chPostProcessShader);
	//postEffect->SetTechnique("PostProcessMotionBlur");
	//D3DXMATRIX CameraInverse;
	//
	//D3DXMatrixIdentity(&CameraInverse);
	//D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );
	//
	//D3DXMATRIX invViewProj;
	//
	//D3DXMatrixIdentity(&invViewProj);
	//D3DXMatrixInverse(&invViewProj, 0, &( CameraInverse* *m_pCamera->GetProjectionMatrix()));
	//
	//postEffect->SetMatrix("gInvViewProj",&invViewProj);
	//postEffect->SetMatrix("gViewProj",m_pCamera->GetCurrentMatrix( ));
	//postEffect->SetMatrix("gPrevViewProj",&m_pCamera->GetPreviousProjectionMatrix());
	//D3DXVECTOR3 tempPosition = D3DXVECTOR3(m_pCamera->GetCurrentMatrix()->_41,m_pCamera->GetCurrentMatrix()->_42,m_pCamera->GetCurrentMatrix()->_43);
	//
	//postEffect->SetVector("gCameraPos",&D3DXVECTOR4(tempPosition,1.0f));
	//postEffect->SetInt("g_numSamples",10);
	//
	//m_d3dpDevice->SetVertexDeclaration(m_d3dpQuadVertexDeclaration);
	//m_d3dpDevice->SetStreamSource(0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	//m_d3dpDevice->SetIndices(m_d3dpQuadIndex);
	//
	//unsigned int passes = 0;
	//postEffect->Begin(&passes,0);
	//for(unsigned int j = 0; j < passes; ++j)
	//{
	//postEffect->BeginPass(j);
	//
	//m_d3dpDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
	//
	//postEffect->EndPass();
	//}
	//
	//postEffect->End();
	//m_PostProcessTarget.RevertTarget();

}

void CRenderer::RenderTransparency(const vector<CEntity*> *_vpRenderableObjects,const vector<CEntity *> * _vpTransparentObjects,int _shader, string _technique, const vector<CEffectSystem*>& _vpEffects)
{

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_shader);

	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	for( unsigned int nRenderIter = 0; nRenderIter < _vpTransparentObjects->size( ); nRenderIter++ )
	{

		CEntity* pTempEntity = ( *_vpTransparentObjects )[ nRenderIter ];

		if(pTempEntity == NULL)
			continue;

		if(pTempEntity->GetType() == eCAM_OBJ || pTempEntity->GetType() == eBOSSTHRUSTER)
			continue;

		int nType = pTempEntity->GetType();
		string TransparentObject = "TransparentObject";
		TransparentObject += _technique.c_str();

		pTempEffect->SetTechnique(TransparentObject.c_str());

		CAssetManager::tModel* pModel = m_pAssetManager->GetModel( nType );

		float fEdgeColor = 0.0f;

		if(nType == eASTEROID)
		{
			if (pTempEntity->GetIsScenery() )
			{
				pModel = m_pAssetManager->GetModel( ((CEnvironmentObject*)pTempEntity)->GetAsteroidType() );
			}
			else
			{
				pModel = m_pAssetManager->GetModel( ((CAsteroid*)pTempEntity)->GetAsteroidType() );
			}

		}
		else if(nType == eWAYPOINT)
		{
			string WayPointShaderString = "Fade";
			WayPointShaderString += TransparentObject;
			pTempEffect->SetTechnique(WayPointShaderString.c_str());
			pTempEffect->SetFloat("gTransitionValue",pTempEntity->GetTransitionValue());
		}


		if( pTempEntity->GetIsVisable() == true )
		{
			pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
			pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
			pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

			pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
			pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
			pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );
			pTempEffect->SetMatrix("gViewInverse",&CameraInverse);

			pTempEffect->SetFloat("gPercent",D3DXVec3Length(&(pTempEntity->GetPosition() - m_pCamera->GetCurrentPosition())) / 54);

			m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
			m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
			m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

			unsigned int nPasses = 0;
			pTempEffect->Begin( &nPasses, 0 );
			{
				for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
				{
					pTempEffect->BeginPass( nPassIter );

					m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

					pTempEffect->EndPass( );
				}
			}
			pTempEffect->End( );
		}
	}

	if(m_vShields.size() > 0)
	{
		if(m_vShields.size() >= 2)
			QuickSort(m_vShields, 0, m_vShields.size()-1);

		for( unsigned int nRenderIter = 0; nRenderIter < m_vShields.size(); nRenderIter++ )
		{
			CShield* pTempShield = ((CShield*)( m_vShields )[ nRenderIter ]);

			int nType =  pTempShield->GetType();

			string ShieldShaderString = "EnemyShield";
			ShieldShaderString += _technique.c_str();
			pTempEffect->SetTechnique(ShieldShaderString.c_str());

			CAssetManager::tModel* pModel = m_pAssetManager->GetModel( nType );

			if(  pTempShield->GetIsVisable() == true )
			{

				pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(m_chEnemyShieldTexture));
				pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
				pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

				pTempEffect->SetMatrix( "gMVP", &( *pTempShield->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

				pTempEffect->SetMatrix( "gWorld",  pTempShield->GetMatrix( ));
				pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

				pTempEffect->SetFloat("gPercent", pTempShield->GetShieldPercent());
				pTempEffect->SetBool( "gIsHit",  pTempShield->GetIsHit( ) );
				pTempEffect->SetFloat( "gTime", m_fTime );

				m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
				m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
				m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

				unsigned int nPasses = 0;
				pTempEffect->Begin( &nPasses, 0 );
				{
					for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
					{
						pTempEffect->BeginPass( nPassIter );

						m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

						pTempEffect->EndPass( );
					}
				}
				pTempEffect->End( );
			}
			m_vShields.clear();
		}
	}

	string ShieldShaderString = "Shield";
	ShieldShaderString += _technique.c_str();

	CPlayer* pTempEntity = ((CPlayer*)m_pPlayer);
	CAssetManager::tModel* pModel = m_pAssetManager->GetModel( pTempEntity->GetType() );

	pTempEffect->SetTechnique(ShieldShaderString.c_str());

	float fTempShield = (((CPlayer*)pTempEntity)->GetShields() *1.0f);

	if(pTempEntity->GetType() == eRED_PLAYER)
	{
		fTempShield /= RED_STATE_SHIELD;
	}
	else if(pTempEntity->GetType() == eBLUE_PLAYER)
	{
		fTempShield /= BLUE_STATE_SHIELD;
	}

	pTempEffect->SetTexture("gDiffuseShieldMap",m_pAssetManager->GetTexture(m_chShieldTexture));
	pTempEffect->SetFloat("gPercent",fTempShield);

	pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
	pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

	pTempEffect->SetFloat( "gTime", (m_fTime) );
	pTempEffect->SetFloat("gTransitionValue",pTempEntity->GetTransitionValue());
	pTempEffect->SetBool( "gIsHit", pTempEntity->GetShieldHit() );

	m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
	m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

	unsigned int nPasses = 0;
	pTempEffect->Begin( &nPasses, 0 );
	{
		for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
		{
			pTempEffect->BeginPass( nPassIter );

			m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

			pTempEffect->EndPass( );
		}
	}
	pTempEffect->End( );

	unsigned int uiEffectsSize = _vpEffects.size();

	for(unsigned int uiEffectsIndex = 0; uiEffectsIndex < uiEffectsSize; ++uiEffectsIndex)
	{
		CEffectSystem* pEffectSystem = _vpEffects[uiEffectsIndex];

		if(pEffectSystem->GetPlay() == false )
		{
			continue;
		}

		vector<CGeometrySystem>*  pGeometrySystem = pEffectSystem->GetGeometrySystem();
		unsigned int nGeometrySystemSize = pGeometrySystem->size();
		for(unsigned int uiGeometryIndex = 0; uiGeometryIndex < nGeometrySystemSize; ++uiGeometryIndex)
		{

			RenderGeometry((*pGeometrySystem)[uiGeometryIndex].GetGeometry(), (*pGeometrySystem)[uiGeometryIndex].GetTextureID(), (*pGeometrySystem)[uiGeometryIndex].GetMeshID(), (*pGeometrySystem)[uiGeometryIndex].getSourceBlend(), (*pGeometrySystem)[uiGeometryIndex].getDesitinationBlend(),_shader,_technique );
		}

		vector<CParticleSystem>*  pParticleSystem = pEffectSystem->GetParticleSystem(); 
		unsigned int nParticleSystemSize = pParticleSystem->size();
		for(unsigned int uiParticleIndex = 0; uiParticleIndex < nParticleSystemSize; ++uiParticleIndex)
		{

			RenderParticles( (*pParticleSystem)[uiParticleIndex].GetParticles(), (*pParticleSystem)[uiParticleIndex].getTextureID(), (*pParticleSystem)[uiParticleIndex].getSourceBlend(), (*pParticleSystem)[uiParticleIndex].getDesitinationBlend(),_shader,_technique, pEffectSystem->GetType());

		}
	}

	CBoss* pBossEntity = ((CBoss*)m_pBoss);

	if(m_pBoss != NULL && pBossEntity->GetIsVisable( ) )
	{
		if(pBossEntity->GetPhase() == eSEEK_AND_DESTROY && m_pBossTurret != NULL)
		{
			static float bossTurretShieldPercent = 0.7f;
			CBossWinTurret* tempTurret = (CBossWinTurret*)m_pBossTurret;

			string BossTurretShieldShaderString = "BossTurretShield";
			BossTurretShieldShaderString += _technique.c_str();
			pTempEffect->SetTechnique(BossTurretShieldShaderString.c_str());

			CAssetManager::tModel* pModel = m_pAssetManager->GetModel( eBOSS_SHIELD );
			D3DXMATRIX tempScale;
			D3DXMatrixIdentity(&tempScale);
			D3DXMatrixScaling(&tempScale,4.0f,4.0f,7.0f);

			D3DXMATRIX tempTranslate;
			D3DXMatrixIdentity(&tempTranslate);
			D3DXMatrixTranslation(&tempTranslate,0.0f,-10.0f,0.0f);

			unsigned int nPasses = 0;

			pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(m_chEnemyShieldTexture));
			pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
			pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

			pTempEffect->SetMatrix( "gMVP", &(tempScale * tempTranslate * *tempTurret->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

			pTempEffect->SetMatrix( "gWorld", &(tempScale * tempTranslate * *tempTurret ->GetMatrix( )));
			pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

			float tempPercent = (tempTurret->GetShields() / 200.0f);

			if(tempPercent >= 1.0f)
				bossTurretShieldPercent = 0.7f;

			if(tempTurret->GetRecharging())
			{
				bossTurretShieldPercent = tempPercent - 0.3f;
			}
			else
			{
				if(tempPercent < (bossTurretShieldPercent + 0.3f))
				{
					bossTurretShieldPercent  -= m_pTime->GetDeltaTime() / 10.0f;

					if(tempPercent <= 0.0999f && bossTurretShieldPercent <= 0.0999f)
						bossTurretShieldPercent = 0.1f;

					if(tempPercent <= 0.0f)
						bossTurretShieldPercent = 0.0f;
				}
			}

			if(bossTurretShieldPercent != 0.0f)
			{
				pTempEffect->SetFloat("gPercent",bossTurretShieldPercent);
				pTempEffect->SetBool( "gIsHit", tempTurret ->GetIsHit( ) );

				m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
				m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
				m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer);

				pTempEffect->Begin( &nPasses, 0 );
				{
					for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
					{
						pTempEffect->BeginPass( nPassIter );

						m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles);

						pTempEffect->EndPass( );
					}
				}
				pTempEffect->End( );
			}

			string BossTurretLaserShaderString = "BossLaser";
			BossTurretLaserShaderString += _technique.c_str();
			pTempEffect->SetTechnique(BossTurretLaserShaderString.c_str());

			D3DXMatrixIdentity(&tempScale);
			D3DXMatrixIdentity(&tempTranslate);

			bool render = false;

			if(tempTurret->GetChargingTimer() > 0.0f)
			{
				D3DXMatrixScaling(&tempScale,0.01f,0.01f,20.0f);
				render = true;
			}
			else if(tempTurret->GetFireLaser() || tempTurret->GetFireLaserTowardPlanet())
			{
				D3DXMatrixScaling(&tempScale,2.0f,2.0f,20.0f);
				render = true;
			}

			if(render)
			{
				D3DXMatrixTranslation(&tempTranslate,0.0f,-16.0f,0.0f);

				pModel = m_pAssetManager->GetModel(eBOSS_LASER);

				pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
				pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
				pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

				pTempEffect->SetMatrix( "gMVP", &(tempScale * tempTranslate * *tempTurret->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

				pTempEffect->SetMatrix( "gWorld", &(tempScale * tempTranslate * *tempTurret ->GetMatrix( )));
				pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );
				pTempEffect->SetFloat("gTransitionValue",tempTurret->GetTransitionValue());
				m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
				m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
				m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer);

				pTempEffect->Begin( &nPasses, 0 );
				{
					for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
					{
						pTempEffect->BeginPass( nPassIter );

						m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles);

						pTempEffect->EndPass( );
					}
				}
				pTempEffect->End( );

			}
		}
		else if((pBossEntity->GetPhase() == eLASER_PHASE || pBossEntity->GetPhase() == eMISSILE_PHASE) && m_pBossTurret != NULL)
		{
			string BossTurretLaserShaderString = "BossLaser";
			BossTurretLaserShaderString += _technique.c_str();
			pTempEffect->SetTechnique(BossTurretLaserShaderString.c_str());

			CBossWinTurret* tempTurret = (CBossWinTurret*)m_pBossTurret;

			D3DXMATRIX tempScale;
			D3DXMATRIX tempTranslate;
			D3DXMatrixIdentity(&tempScale);
			D3DXMatrixIdentity(&tempTranslate);

			bool render = false;

			if(tempTurret->GetFireLaserTowardPlanet())
			{
				D3DXMatrixScaling(&tempScale,2.0f,2.0f,20.0f);
				render = true;
			}

			if(render)
			{
				D3DXMatrixTranslation(&tempTranslate,0.0f,-16.0f,0.0f);

				pModel = m_pAssetManager->GetModel(eBOSS_LASER);

				pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
				pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
				pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

				pTempEffect->SetMatrix( "gMVP", &(tempScale * tempTranslate * *tempTurret->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

				pTempEffect->SetMatrix( "gWorld", &(tempScale * tempTranslate * *tempTurret ->GetMatrix( )));
				pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );
				pTempEffect->SetFloat("gTransitionValue",tempTurret->GetTransitionValue());
				m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
				m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
				m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer);

				pTempEffect->Begin( &nPasses, 0 );
				{
					for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
					{
						pTempEffect->BeginPass( nPassIter );

						m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles);

						pTempEffect->EndPass( );
					}
				}
				pTempEffect->End( );
			}
		}
		static float bossShieldPercent = 0.7f;

		string BossShieldShaderString = "BossShield";
		BossShieldShaderString += _technique.c_str();
		pTempEffect->SetTechnique(BossShieldShaderString.c_str());

		D3DXMATRIX tempScale;
		D3DXMatrixIdentity(&tempScale);
		D3DXMatrixScaling(&tempScale,0.87f,1.25f,1.10f);

		CAssetManager::tModel* pModel = m_pAssetManager->GetModel( eBOSS_SHIELD );

		unsigned int nPasses = 0;

		pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(pModel->m_chTextureNumber));
		pTempEffect->SetTexture("gSpecularMap",m_pAssetManager->GetTexture(pModel->m_chTextureSpecNumber));
		pTempEffect->SetTexture("gNormalMap",m_pAssetManager->GetTexture(pModel->m_chTextureNormNumber));

		pTempEffect->SetMatrix( "gMVP", &(tempScale * *pBossEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );

		pTempEffect->SetMatrix( "gWorld", &(tempScale *  *pBossEntity->GetMatrix( ) ));
		pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

		if(pBossEntity->GetNumGenerators() == 3.0f)
		{
			bossShieldPercent = 0.7f;
		}
		else if(pBossEntity->GetNumGenerators() == 2.0f && bossShieldPercent >= 0.5f)
		{
			bossShieldPercent -= m_pTime->GetDeltaTime( )/10.0f;
			if(bossShieldPercent <= 0.5f)
				bossShieldPercent = 0.5f;
		}
		else if(pBossEntity->GetNumGenerators() == 1.0f && bossShieldPercent >= 0.3f)
		{
			bossShieldPercent -= m_pTime->GetDeltaTime( )/10.0f;
			if(bossShieldPercent <= 0.3f)
				bossShieldPercent = 0.3f;
		}
		else if(pBossEntity->GetNumGenerators() == 0.0f && bossShieldPercent > 0.0f)
		{
			bossShieldPercent -= m_pTime->GetDeltaTime( )/10.0f;
			if(bossShieldPercent <= 0.0f)
				bossShieldPercent = 0.0f;
		}

		if(bossShieldPercent != 0.0f)
		{
			pTempEffect->SetFloat("gPercent",bossShieldPercent);
			pTempEffect->SetBool( "gIsHit", pBossEntity->GetIsHit( ) );

			m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
			m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
			m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer);

			pTempEffect->Begin( &nPasses, 0 );
			{
				for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
				{
					pTempEffect->BeginPass( nPassIter );

					m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles);

					pTempEffect->EndPass( );
				}
			}
			pTempEffect->End( );
		}

	}

	RenderSkyBox();
}

void CRenderer::RenderMainMenuTransparency(const vector<CEntity*> *_vpRenderableObjects,int _shader, string _technique, const vector<CEffectSystem*>& _vpEffects)
{
	unsigned int uiEffectsSize = _vpEffects.size();

	for(unsigned int uiEffectsIndex = 0; uiEffectsIndex < uiEffectsSize; ++uiEffectsIndex)
	{
		CEffectSystem* pEffectSystem = _vpEffects[uiEffectsIndex];

		if(pEffectSystem->GetPlay() == false )
		{
			continue;
		}

		vector<CGeometrySystem>*  pGeometrySystem = pEffectSystem->GetGeometrySystem();
		unsigned int nGeometrySystemSize = pGeometrySystem->size();
		for(unsigned int uiGeometryIndex = 0; uiGeometryIndex < nGeometrySystemSize; ++uiGeometryIndex)
		{

			RenderGeometry((*pGeometrySystem)[uiGeometryIndex].GetGeometry(), (*pGeometrySystem)[uiGeometryIndex].GetTextureID(), (*pGeometrySystem)[uiGeometryIndex].GetMeshID(), (*pGeometrySystem)[uiGeometryIndex].getSourceBlend(), (*pGeometrySystem)[uiGeometryIndex].getDesitinationBlend(),_shader,_technique );

		}

		vector<CParticleSystem>*  pParticleSystem = pEffectSystem->GetParticleSystem(); 
		unsigned int nParticleSystemSize = pParticleSystem->size();
		for(unsigned int uiParticleIndex = 0; uiParticleIndex < nParticleSystemSize; ++uiParticleIndex)
		{

			RenderMainMenuParticles( (*pParticleSystem)[uiParticleIndex].GetParticles(), (*pParticleSystem)[uiParticleIndex].getTextureID(), (*pParticleSystem)[uiParticleIndex].getSourceBlend(), (*pParticleSystem)[uiParticleIndex].getDesitinationBlend(),_shader,_technique, pEffectSystem->GetType());
		}
	}

	RenderSkyBox();
}

void CRenderer::RenderButton(float _fTopLeftPosX,float _fTopLeftPosY,int _nButton, float _fScaleX, float _fScaleY, bool _bButtonSelected, LONG _rLeft, LONG _rTop, LONG _rRight, LONG _rBottom, D3DCOLOR _color)
{
	D3DXMATRIX	_d3dMatrix;
	D3DXMatrixIdentity( &_d3dMatrix );
	D3DXMatrixScaling(&_d3dMatrix,_fScaleX * m_fXRatio,_fScaleY * m_fYRatio,.70f);

	m_d3dpSpriteManager->SetTransform( &_d3dMatrix );

	m_d3dpSpriteManager->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );
	{

		if(_nButton > eNONE && _nButton <= eNUM_BUTTONS_MAX)
		{
			RECT _tButton1Rect;

			if(_bButtonSelected == true)
			{
				_tButton1Rect.left = _rRight;
				_tButton1Rect.right = _rRight + 512; 
			}
			else
			{
				_tButton1Rect.left = _rLeft;
				_tButton1Rect.right = _rRight;
			}


			_tButton1Rect.top = _rTop;
			_tButton1Rect.bottom = _rBottom;

			m_d3dpSpriteManager->Draw( m_pAssetManager->GetTexture( m_chButtonTexture ), &_tButton1Rect, NULL, &D3DXVECTOR3(_fTopLeftPosX,_fTopLeftPosY,0.0f),_color );
		}


	}
	m_d3dpSpriteManager->End( );

	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity( &tempMatrix );

	m_d3dpSpriteManager->SetTransform( &tempMatrix );
}

/*****************************************************************
* Render2d():  
* 			      
*			      
*			      
*
* Ins:						int _ntextureToRender
*
* Outs:						None
*							
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::Render2d( int _nTextureToRender )
{
	ID3DXEffect * d3dMenuEffect = m_pAssetManager->GetEffect( m_chMenuShader );

	d3dMenuEffect->SetTechnique( "myTechnique" );
	d3dMenuEffect->SetFloat("gGamma", m_fGammaValue);
	d3dMenuEffect->SetTexture( "gDiffuseMap", m_pAssetManager->GetTexture( _nTextureToRender ) );
	d3dMenuEffect->SetFloat( "gAlpha", 1.0f );

	m_d3dpDevice->SetVertexDeclaration( m_d3dpQuadVertexDeclaration );

	m_d3dpDevice->SetStreamSource( 0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	m_d3dpDevice->SetIndices( m_d3dpQuadIndex );

	unsigned int nPasses = 0;
	d3dMenuEffect->Begin( &nPasses, 0 );
	for(unsigned int nPassIter = 0; nPassIter < nPasses; nPassIter++ )
	{
		d3dMenuEffect->BeginPass( nPassIter );

		m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2 );

		d3dMenuEffect->EndPass( );
	}
	d3dMenuEffect->End( );
}

/*****************************************************************
* Render2d():  
* 			      
*			      
*			      
*
* Ins:						IDirect3DTexture9* _d3dpTextureToRender
*      						float _fAlpha
*
* Outs:						None
*							
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::Render2d( IDirect3DTexture9* _d3dpTextureToRender, float _fAlpha )
{
	ID3DXEffect * d3dMenuEffect = m_pAssetManager->GetEffect( m_chMenuShader );

	d3dMenuEffect->SetTechnique( "myTechniqueAlpha" );
	d3dMenuEffect->SetFloat("gGamma", m_fGammaValue);
	d3dMenuEffect->SetTexture( "gDiffuseMap", _d3dpTextureToRender);
	d3dMenuEffect->SetFloat( "gAlpha", _fAlpha );

	m_d3dpDevice->SetVertexDeclaration( m_d3dpQuadVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	m_d3dpDevice->SetIndices( m_d3dpQuadIndex );

	unsigned int nPasses = 0;
	d3dMenuEffect->Begin( &nPasses, 0 );
	for(unsigned int nPassIter = 0; nPassIter < nPasses; nPassIter++ )
	{
		d3dMenuEffect->BeginPass( nPassIter );

		m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2 );

		d3dMenuEffect->EndPass( );
	}
	d3dMenuEffect->End( );
}

/*****************************************************************
* Render2d():  
* 			      
*			      
*			      
*
* Ins:						int _ntextureToRender
*      						float _fAlpha
*
* Outs:						None
*							
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::Render2d( int _nTextureToRender, float _fAlpha )
{
	ID3DXEffect * d3dMenuEffect = m_pAssetManager->GetEffect( m_chMenuShader );

	d3dMenuEffect->SetTechnique( "myTechniqueAlpha" );
	d3dMenuEffect->SetFloat("gGamma", m_fGammaValue);
	d3dMenuEffect->SetTexture( "gDiffuseMap" , m_pAssetManager->GetTexture( _nTextureToRender ) );
	d3dMenuEffect->SetFloat( "gAlpha", _fAlpha );

	m_d3dpDevice->SetVertexDeclaration( m_d3dpQuadVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, m_d3dpQuadBuff, 0, sizeof( tVERTPOSUV ) );
	m_d3dpDevice->SetIndices( m_d3dpQuadIndex );

	unsigned int nPasses = 0;
	d3dMenuEffect->Begin( &nPasses, 0 );
	for(unsigned int nPassIter = 0; nPassIter < nPasses ; nPassIter++ )
	{
		d3dMenuEffect->BeginPass(nPassIter);

		m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2 );

		d3dMenuEffect->EndPass( );
	}
	d3dMenuEffect->End( );
}

/*****************************************************************
* RenderArrow():  
* 			      
*			      
*			      
*
* Ins:						int _ntextureToRender
*							RECT *_tRectToRender
*      						D3DXMATRIX _d3dMartix
*
* Outs:						None
*							
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderArrow( int _nTextureToRender, RECT *_ptRectToRender, D3DXMATRIX _d3dMatrix )
{
	D3DXMATRIX tempMat = _d3dMatrix;

	m_d3dpSpriteManager->SetTransform( &tempMat );

	m_d3dpSpriteManager->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );
	m_d3dpSpriteManager->Draw( m_pAssetManager->GetTexture( _nTextureToRender ), _ptRectToRender, &D3DXVECTOR3( _ptRectToRender->right * 0.5f, _ptRectToRender->bottom * 0.5f, 0.0f ), NULL,D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	m_d3dpSpriteManager->End( );

	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity( &tempMatrix );

	m_d3dpSpriteManager->SetTransform( &tempMatrix );


}

void CRenderer::RenderArrowAndrewR( int _nTextureToRender, RECT *_ptRectToRender, D3DXMATRIX _d3dMatrix,  D3DCOLOR color , bool _center, bool _ScreenRatioScale )
{
	D3DXMATRIX tempMat = _d3dMatrix;
	if(_ScreenRatioScale)
	{
		tempMat._41 *= m_fXRatio;
		tempMat._42 *= m_fYRatio;
	}

	m_d3dpSpriteManager->SetTransform( &tempMat );

	m_d3dpSpriteManager->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_BACKTOFRONT );
	if (_center)
	{
		m_d3dpSpriteManager->Draw( m_pAssetManager->GetTexture( _nTextureToRender ), _ptRectToRender, &D3DXVECTOR3( float(_ptRectToRender->right * .5f), float(_ptRectToRender->bottom*.5f), 0.0f ), NULL, color );
	}
	else
	{
		m_d3dpSpriteManager->Draw( m_pAssetManager->GetTexture( _nTextureToRender ), _ptRectToRender, &D3DXVECTOR3( float(_ptRectToRender->right), float(_ptRectToRender->bottom), 0.0f ), NULL, color );
	}
	m_d3dpSpriteManager->End( );

	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity( &tempMatrix );

	m_d3dpSpriteManager->SetTransform( &tempMatrix );


}

/*****************************************************************
* RenderCollisionDEBUG():  
* 			      
*			      
*			      
*
* Ins:						CEntity * _pEntity
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderCollisionDEBUG( const vector<CEntity*> *_vpRenderableObjects,int _shader, string _technique )
{

	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	ID3DXEffect * pTempEffect = m_pAssetManager->GetEffect( _shader );
	string tempTechnique = _technique;
	tempTechnique += "CollisionSphereWF";
	pTempEffect->SetTechnique( tempTechnique.c_str());

	for( unsigned int nRenderIter = 0; nRenderIter < _vpRenderableObjects->size( ); nRenderIter++ )
	{
		CEntity* pTempEntity = ( *_vpRenderableObjects )[ nRenderIter ];

		if(pTempEntity->GetMesh( ) >= 0)
		{
			D3DXVECTOR4 collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f);
			switch (pTempEntity->GetType())
			{
			case eRED_ENEMY:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f);
					break;
				}
			case eBLUE_ENEMY:
				{
					collisonSphereColor = D3DXVECTOR4(0.0f,0.0f,1.0f,1.0f);
					break;
				}
			case eYELLOW_ENEMY: 
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f);
					break;
				}
			case eRED_PLAYER:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f);
					break;
				}
			case eBLUE_PLAYER:
				{
					collisonSphereColor = D3DXVECTOR4(0.0f,0.0f,1.0f,1.0f);
					break;
				}
			case eYELLOW_PLAYER:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f);
					break;
				}
			case eMISSILE:
				{
					collisonSphereColor = D3DXVECTOR4(0.0f,0.0f,0.5f,1.0f);
					break;
				}
			case eLASER:
				{
					collisonSphereColor = D3DXVECTOR4(0.5f,0.0f,0.0f,1.0f);
					break;
				}
			case eASTEROID:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.5f,0.0f,1.0f);
					break;
				}
			case eWAYPOINT:
				{
					collisonSphereColor = D3DXVECTOR4(0.0f,1.0f,0.0f,1.0f);
					break;
				}
			case eSHIELD:
				{
					continue;
					break;
				}
			case eSHIELD_QUAD:
				{
					continue;
					break;
				}
			case eBOSS:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f);
					break;
				}
			case eGENERATOR:
				{
					collisonSphereColor = D3DXVECTOR4(0.5f,0.75f,0.0f,1.0f);
					break;
				}
			case eHanger:
				{
					continue;
					break;
				}
			case eTURRET_SMALL:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,1.0f,0.0f,1.0f);
					break;
				}
			case eTURRET_LASER:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,0.0f,1.0f);
					break;
				}
			case eTURRET_MISSILE:
				{
					collisonSphereColor = D3DXVECTOR4(0.0f,0.0f,1.0f,1.0f);
					break;
				}
			case eLARGE_ASTEROID:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);
					break;
				}
			case eBOSS_SHIELD:
				{
					continue;
					break;
				}
			default:
				{
					collisonSphereColor = D3DXVECTOR4(1.0f,0.0f,1.0f,1.0f);
					break;
				}
			}

			pTempEffect->SetVector("gColor",&collisonSphereColor);
			pTempEffect->SetMatrix( "gWorld", pTempEntity->GetMatrix( ) );
			pTempEffect->SetMatrix( "gMVP", &(*pTempEntity->GetMatrix( ) * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
			pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

			unsigned int nPasses = 0;
			pTempEffect->Begin( &nPasses, 0);
			for( unsigned int nPassIter = 0; nPassIter < nPasses; nPassIter++)
			{
				pTempEffect->BeginPass( nPassIter );

				m_pAssetManager->GetMesh( pTempEntity->GetMesh() )->m_d3dpMesh->DrawSubset( 0 );

				pTempEffect->EndPass( );
			}
			pTempEffect->End( );
		}
	}
}

/*****************************************************************
* CreateQuad():  
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::CreateQuad( )
{
	D3DVERTEXELEMENT9 vertclr_definition[ ]=
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	m_d3dpDevice->CreateVertexDeclaration( vertclr_definition, &m_d3dpQuadVertexDeclaration );

	struct tVERTPOSUV arrtScreenVerts[ ] =
	{
		{ D3DXVECTOR3( -1.0f, 1.0f, 0.0f), D3DXVECTOR2( 0 + ( 1 / ( m_d3dBackBufferDesc.Width * .5f ) ), 0 + ( 1 / m_d3dBackBufferDesc.Height * 0.5f ) ) },
		{ D3DXVECTOR3( 1.0f, 1.0f, 0.0f), D3DXVECTOR2( 1 + ( 1 / m_d3dBackBufferDesc.Width * .5f ), 0 + ( 1 / m_d3dBackBufferDesc.Height * 0.5f ) ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, 0.0f), D3DXVECTOR2( 1 + ( 1 / m_d3dBackBufferDesc.Width * .5f ), 1 + ( 1 / m_d3dBackBufferDesc.Height * 0.5f ) ) },
		{ D3DXVECTOR3( -1.0f, -1.0f, 0.0f), D3DXVECTOR2( 0 + ( 1 / m_d3dBackBufferDesc.Width * .5f ), 1 + ( 1 / m_d3dBackBufferDesc.Height * 0.5f ) ) },
	};	

	short arrsScreenIndicies[ ] = 
	{
		0, 1, 2,
		0, 2, 3
	};
	void * pMem = 0;

	m_d3dpDevice->CreateVertexBuffer(unsigned int( sizeof( tVERTPOSUV ) * 4), 0, 0, D3DPOOL_MANAGED, &m_d3dpQuadBuff, 0);
	pMem = 0;

	m_d3dpQuadBuff->Lock(0,0,&pMem,0);
	memcpy( pMem, arrtScreenVerts, 4 * sizeof( tVERTPOSUV ));
	m_d3dpQuadBuff->Unlock();

	m_d3dpDevice->CreateIndexBuffer( sizeof( short ) * 6, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_d3dpQuadIndex, 0);
	pMem = 0;

	m_d3dpQuadIndex->Lock( 0, 0, &pMem, 0);
	memcpy( pMem, arrsScreenIndicies, 6 * sizeof( short ));
	m_d3dpQuadIndex->Unlock();

}

/*****************************************************************
* CreateSkyBox():  
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::CreateSkyBox( )
{
	m_chSkyBoxShader = m_pAssetManager->LoadEffect( "SkyBox.fx" );
	D3DXCreateCubeTextureFromFile( m_d3dpDevice, "Resources/Textures/SkyBox6.dds", &m_d3dpSkyBoxTexture );

	D3DVERTEXELEMENT9 vertclr_definition[]=
	{
		{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
		{0,12,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
		D3DDECL_END()
	};

	m_d3dpDevice->CreateVertexDeclaration( vertclr_definition, &m_d3dpCubeVertexDeclaration );



	tVERTPOSNORMUV tarrQuadVerts[ ] =
	{
		// front
		{ D3DXVECTOR3( -1.0f, 1.0f, -1.0f), D3DXVECTOR2( 0.0f,0.0f )},
		{ D3DXVECTOR3( 1.0f, 1.0f, -1.0f), D3DXVECTOR2( 0.0f,1.0f )},
		{ D3DXVECTOR3( -1.0f, -1.0f, -1.0f), D3DXVECTOR2( 1.0f,0.0f )},
		{ D3DXVECTOR3( 1.0f, -1.0f, -1.0f), D3DXVECTOR2( 1.0f,1.0f )},
		//back		   
		{ D3DXVECTOR3( 1.0f, 1.0f, 1.0f), D3DXVECTOR2( 0.0f, 0.0f ) },
		{ D3DXVECTOR3( -1.0f, 1.0f, 1.0f), D3DXVECTOR2( 0.0f, 1.0f ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, 1.0f), D3DXVECTOR2( 1.0f, 0.0f ) },
		{ D3DXVECTOR3( -1.0f, -1.0f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },
		//left		   
		{ D3DXVECTOR3( -1.0f, 1.0f, 1.0f), D3DXVECTOR2( 0.0f, 0.0f ) },
		{ D3DXVECTOR3( -1.0f, 1.0f, -1.0f), D3DXVECTOR2( 0.0f, 1.0f ) },
		{ D3DXVECTOR3( -1.0f, -1.0f, 1.0f), D3DXVECTOR2( 1.0f, 0.0f ) },
		{ D3DXVECTOR3( -1.0f, -1.0f, -1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },
		//right		   
		{ D3DXVECTOR3( 1.0f, 1.0f, -1.0f), D3DXVECTOR2( 0.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, 1.0f, 1.0f), D3DXVECTOR2( 0.0f, 1.0f ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, -1.0f), D3DXVECTOR2( 1.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) },
		//top		   
		{ D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },
		{ D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) },
		//bottom
		{ D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) },
		{ D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 0.0f ) },
		{ D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR2( 1.0f, 1.0f ) }
	};

	m_d3dpDevice->CreateVertexBuffer( sizeof( tVERTPOSNORMUV ) * 24, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_d3dpCubeVertBuffer, 0 );
	void * pMem = 0;

	m_d3dpCubeVertBuffer->Lock( 0, 0, &pMem, 0);
	memcpy( pMem, &tarrQuadVerts, 24 * sizeof( tVERTPOSNORMUV ) );
	m_d3dpCubeVertBuffer->Unlock();

	short arrsQuadindicies[ 36 ] = 
	{
		//front
		0, 1, 2,
		1, 3, 2,
		//back
		4, 5, 6,
		5, 7, 6, 
		//left
		8, 9, 10,
		9, 11, 10,
		//right
		12, 13, 14,
		13, 15, 14,
		//top
		16, 17, 18,
		17, 19, 18,
		//bottom
		20, 21, 22,
		21, 23, 22
	};

	m_d3dpDevice->CreateIndexBuffer( sizeof( short ) * 36, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_d3dpCubeIndexBuffer, 0 );
	pMem = 0;

	m_d3dpCubeIndexBuffer->Lock( 0, 0, &pMem, 0);
	memcpy( pMem, arrsQuadindicies, sizeof( short ) * 36 );
	m_d3dpCubeIndexBuffer->Unlock( );

	m_d3dSkyBoxMatrix = *m_pCamera->GetCurrentMatrix( );

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation,3.14f);
	m_d3dSkyBoxMatrix = m_d3dSkyBoxMatrix *rotation;
}

/*****************************************************************
* RenderSkyBox():  
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

void CRenderer::RenderSkyBox()
{
	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	m_d3dSkyBoxMatrix._41 = m_pCamera->GetCurrentMatrix( )->_41;
	m_d3dSkyBoxMatrix._42 = m_pCamera->GetCurrentMatrix( )->_42;
	m_d3dSkyBoxMatrix._43 = m_pCamera->GetCurrentMatrix( )->_43 - 1.0f;

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect( m_chSkyBoxShader );
	pTempEffect->SetTechnique( "Basic" );

	pTempEffect->SetTexture( "reflectionMap", m_d3dpSkyBoxTexture );
	//pTempEffect->SetTexture( "reflectionMap2", m_d3dpStarBoxTexture );

	//pTempEffect->SetFloat("gTime",(m_fTime));

	pTempEffect->SetMatrix( "gMVP", &( m_d3dSkyBoxMatrix * ( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) ) );
	pTempEffect->SetVector( "gCameraPos", &D3DXVECTOR4(CPhysics::GetMat4Vec( ePOS, m_pCamera->GetProjectionMatrix( ) ),1.0f) );

	m_d3dpDevice->SetVertexDeclaration( m_d3dpCubeVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, m_d3dpCubeVertBuffer, 0, sizeof( tVERTPOSNORMUV ) );
	m_d3dpDevice->SetIndices( m_d3dpCubeIndexBuffer );

	unsigned int nPasses = 0;
	pTempEffect->Begin( &nPasses, 0 );
	{
		for(unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter)
		{
			pTempEffect->BeginPass( nPassIter );

			m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 36, 0, 12 );

			pTempEffect->EndPass( );
		}
	}
	pTempEffect->End( );
}

void CRenderer::RenderText(const char* szTextToPrint ,int nPosX ,int nPosY ,float fScale ,float fRotX ,float fRotY ,float fRot ,DWORD dwColor ,bool _ScreenRatioScale)
{
	m_d3dpSpriteManager->Begin(D3DXSPRITE_ALPHABLEND);	
	int len = strlen(szTextToPrint);

	float fScaleX = fScale;
	float fScaleY = fScale;

	int nOriginalX = nPosX;
	int nCharacterY = nPosY; 
	int nCharacterX = nPosX;

	if(_ScreenRatioScale)
	{
		fScaleX *=		m_fXRatio;
		fScaleY *=		m_fYRatio;
		nOriginalX =	(int)( nOriginalX *  m_fXRatio );
		nCharacterY =	(int)( nCharacterY * m_fYRatio );
		nCharacterX =	(int)( nCharacterX * m_fXRatio );
	}

	for(int i = 0; i < len; i++)
	{
		//	Get char from array
		char ch = szTextToPrint[i];
		if(ch == '\n')
		{
			nCharacterX  = nOriginalX;
			nCharacterY += (int)(m_ptBitmapfont->m_nCellHeight * fScaleY);
			continue;
		}

		int nRow = (ch - m_ptBitmapfont->m_cFirstChar) / m_ptBitmapfont->m_nCharactersPerLine;
		int nCol = (ch - m_ptBitmapfont->m_cFirstChar) - (nRow *m_ptBitmapfont->m_nCharactersPerLine);

		RECT rLetter;
		rLetter.left = nCol * m_ptBitmapfont->m_nCellWidth;
		rLetter.top = nRow * m_ptBitmapfont->m_nCellHeight;
		rLetter.right = rLetter.left + m_ptBitmapfont->m_cWidths[ch];
		rLetter.bottom = rLetter.top + m_ptBitmapfont->m_nCellHeight;

		D3DXMATRIX scale;
		D3DXMATRIX rotation;
		D3DXMATRIX translate;
		D3DXMATRIX combined;

		// Initialize the Combined matrix.
		D3DXMatrixIdentity(&combined);

		// Scale the sprite.
		D3DXMatrixScaling(&scale, fScaleX, fScaleY, 1.0f);
		combined *= scale;

		// Rotate the sprite.
		D3DXMatrixTranslation(&translate, -fRotX * fScaleX, -fRotY * fScaleY, 0.0f);
		combined *= translate;
		D3DXMatrixRotationZ(&rotation, fRot);
		combined *= rotation;
		D3DXMatrixTranslation(&translate, fRotX * fScaleX, fRotY * fScaleY, 0.0f);
		combined *= translate;

		// Translate the sprite
		D3DXMatrixTranslation(&translate, (float)nCharacterX, (float)nCharacterY, 0.0f);
		combined *= translate;

		// Apply the transform.
		m_d3dpSpriteManager->SetTransform(&combined);

		// &D3DXVECTOR3(m_ptBitmapfont->m_nCellWidth*0.5f,m_ptBitmapfont->m_nCellHeight*0.5f,0.0f)
		m_d3dpSpriteManager->Draw(m_pAssetManager->GetTexture(m_ptBitmapfont->m_nImageID), &rLetter,&D3DXVECTOR3(m_ptBitmapfont->m_nCellWidth*0.5f,m_ptBitmapfont->m_nCellHeight*0.5f,0.0f), NULL, dwColor);

		D3DXMatrixIdentity(&combined);
		m_d3dpSpriteManager->SetTransform(&combined);

		nCharacterX += (int)(m_ptBitmapfont->m_cWidths[ch] * fScaleX);
	}
	m_d3dpSpriteManager->End();
}

void CRenderer::RenderParticles( const vector<tParticles>* _pvParticles, int _textureID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID /*= 0*/ )
{
	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	CAssetManager::tModel* pModel = m_pAssetManager->GetModel(eSHIELD_QUAD);

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_shader);

	string ShaderString = "Particle";
	ShaderString += _technique.c_str();

	pTempEffect->SetTechnique(ShaderString.c_str());

	D3DXMATRIX tempParticleMatrix;
	D3DXMATRIX tempParticleRotation;
	D3DXMATRIX tempParticleScale;
	D3DXMATRIX tempParticleTranslate;

	float Red;
	float Blue;
	float Green;
	float Alpha;

	D3DXVECTOR4 RGBA;

	unsigned int uiParticleSize = _pvParticles->size();

	pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(_textureID));

	pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

	pTempEffect->SetInt("gSourceBlend", _sourceBlend);
	pTempEffect->SetInt("gDestinationBlend", _destinationBlend);

	m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
	m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

	for(unsigned int particleIter = 0; particleIter < uiParticleSize; particleIter++)
	{
		Red   = 0.0f;
		Blue  = 0.0f;
		Green = 0.0f;
		Alpha = 0.0f;

		tParticles tempParticle = (*_pvParticles)[particleIter];

		if(!tempParticle.m_bAlive)
		{
			continue;
		}


		D3DXMatrixIdentity(&tempParticleMatrix);

		D3DXMatrixScaling(&tempParticleScale, tempParticle.m_d3dScale.x, tempParticle.m_d3dScale.y, 0.0f);
		if(_nEffectID == eLASER_SPARK)
		{
			D3DXMatrixRotationX(&tempParticleRotation, tempParticle.m_fRotation);
		}
		else
		{
			D3DXMatrixRotationZ(&tempParticleRotation, tempParticle.m_fRotation);
		}

		//Translation
		D3DXMatrixTranslation(&tempParticleTranslate, tempParticle.m_d3dPosition.x, tempParticle.m_d3dPosition.y, tempParticle.m_d3dPosition.z);

		// Make the thruster effects local to teh player
		if( _nEffectID == ePLAYER_THRUSTERS || _nEffectID == ePLAYER_IDLE || _nEffectID == ePLAYER_JET_STREAMERS)
		{
			D3DXMatrixMultiply(&tempParticleTranslate, &tempParticleTranslate, m_pPlayer->GetMatrix());
		}

		if(tempParticle.m_bFacePlayer)
		{
			CPhysics::LookAt(&tempParticleTranslate, m_pCamera->GetCurrentMatrix());
		}
		else if(tempParticle.m_bNoRotation == false)
		{
			D3DXMATRIX tempRot;
			D3DXMatrixRotationX(&tempRot, D3DXToRadian(90));
			tempParticleTranslate = tempRot * tempParticleTranslate;
		}

		tempParticleMatrix = tempParticleScale * tempParticleRotation  * tempParticleTranslate;

		if(CCollisionLib::FrustumToPoint(*m_pCamera->GetFrustum(),CPhysics::GetMat4Vec(ePOS, &tempParticleMatrix)) == false && m_pCamera->GetInMainMenu() == false)
			continue;

		unsigned int Color = tempParticle.m_uiColor;

		Blue  = (float) ( Color << 24 >> 24) / 255; 
		Green = (float) ( Color << 16 >> 24) / 255;
		Red   = (float) ( Color << 8 >> 24) / 255; 
		Alpha = (float) ( Color >> 24) / 255;

		RGBA = D3DXVECTOR4(Red,Green,Blue,Alpha);


		pTempEffect->SetMatrix( "gWorld", &tempParticleMatrix );
		pTempEffect->SetMatrix( "gMVP", &(tempParticleMatrix * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
		pTempEffect->SetVector( "gColor", &D3DXVECTOR4(Red, Green, Blue, Alpha));

		unsigned int nPasses = 0;

		pTempEffect->Begin( &nPasses, 0 );
		{
			for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
			{
				pTempEffect->BeginPass( nPassIter );

				m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

				pTempEffect->EndPass( );
			}
		}
		pTempEffect->End( );
	}

}


void CRenderer::RenderMainMenuParticles( const vector<tParticles>* _pvParticles, int _textureID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID /*= 0*/ )
{
	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	CAssetManager::tModel* pModel = m_pAssetManager->GetMainMenuModel(10);

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_shader);

	string ShaderString = "Particle";
	ShaderString += _technique.c_str();

	pTempEffect->SetTechnique(ShaderString.c_str());

	D3DXMATRIX tempParticleMatrix;
	D3DXMATRIX tempParticleRotation;
	D3DXMATRIX tempParticleScale;
	D3DXMATRIX tempParticleTranslate;

	float Red;
	float Blue;
	float Green;
	float Alpha;

	D3DXVECTOR4 RGBA;

	unsigned int uiParticleSize = _pvParticles->size();

	pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(_textureID));

	pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

	pTempEffect->SetInt("gSourceBlend", _sourceBlend);
	pTempEffect->SetInt("gDestinationBlend", _destinationBlend);

	m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
	m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

	for(unsigned int particleIter = 0; particleIter < uiParticleSize; particleIter++)
	{
		Red   = 0.0f;
		Blue  = 0.0f;
		Green = 0.0f;
		Alpha = 0.0f;

		tParticles tempParticle = (*_pvParticles)[particleIter];

		if(!tempParticle.m_bAlive)
		{
			continue;
		}


		D3DXMatrixIdentity(&tempParticleMatrix);

		D3DXMatrixScaling(&tempParticleScale, tempParticle.m_d3dScale.x, tempParticle.m_d3dScale.y, 0.0f);
		if(_nEffectID == eLASER_SPARK)
		{
			D3DXMatrixRotationX(&tempParticleRotation, tempParticle.m_fRotation);
		}
		else
		{
			D3DXMatrixRotationZ(&tempParticleRotation, tempParticle.m_fRotation);
		}

		//Translation
		D3DXMatrixTranslation(&tempParticleTranslate, tempParticle.m_d3dPosition.x, tempParticle.m_d3dPosition.y, tempParticle.m_d3dPosition.z);

		// Make the thruster effects local to teh player
		if( _nEffectID == ePLAYER_THRUSTERS || _nEffectID == ePLAYER_IDLE || _nEffectID == ePLAYER_JET_STREAMERS)
		{
			D3DXMatrixMultiply(&tempParticleTranslate, &tempParticleTranslate, m_pPlayer->GetMatrix());
		}

		if(tempParticle.m_bFacePlayer)
		{
			CPhysics::LookAt(&tempParticleTranslate, m_pCamera->GetCurrentMatrix());
		}
		else if(tempParticle.m_bNoRotation == false)
		{
			D3DXMATRIX tempRot;
			D3DXMatrixRotationX(&tempRot, D3DXToRadian(90));
			tempParticleTranslate = tempRot * tempParticleTranslate;
		}

		tempParticleMatrix = tempParticleScale * tempParticleRotation  * tempParticleTranslate;

		if(CCollisionLib::FrustumToPoint(*m_pCamera->GetFrustum(),CPhysics::GetMat4Vec(ePOS, &tempParticleMatrix)) == false && m_pCamera->GetInMainMenu() == false)
			continue;

		unsigned int Color = tempParticle.m_uiColor;

		Blue  = (float) ( Color << 24 >> 24) / 255; 
		Green = (float) ( Color << 16 >> 24) / 255;
		Red   = (float) ( Color << 8 >> 24) / 255; 
		Alpha = (float) ( Color >> 24) / 255;

		RGBA = D3DXVECTOR4(Red,Green,Blue,Alpha);

		pTempEffect->SetMatrix( "gWorld", &tempParticleMatrix );
		pTempEffect->SetMatrix( "gMVP", &(tempParticleMatrix * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
		pTempEffect->SetVector( "gColor", &D3DXVECTOR4(Red, Green, Blue, Alpha));

		unsigned int nPasses = 0;

		pTempEffect->Begin( &nPasses, 0 );
		{
			for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
			{
				pTempEffect->BeginPass( nPassIter );

				m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

				pTempEffect->EndPass( );
			}
		}
		pTempEffect->End( );
	}

}


void CRenderer::RenderGeometry( const vector<tGeometry>* _pvGeometry, int _textureID, int _meshID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID /*= 0 */ )
{
	D3DXMATRIX CameraInverse;
	D3DXMatrixInverse( &CameraInverse, 0, m_pCamera->GetCurrentMatrix( ) );

	ID3DXEffect* pTempEffect = m_pAssetManager->GetEffect(_shader);

	string ShaderString = "Particle";
	ShaderString += _technique.c_str();

	pTempEffect->SetTechnique(ShaderString.c_str());

	CAssetManager::tModel* pModel = m_pAssetManager->GetGeometry(_meshID);
	D3DXMATRIX tempGeometryMatrix;

	D3DXMATRIX tempGeometryRotation;

	D3DXMATRIX tempGeometryScale;
	D3DXMATRIX tempGeometryTranslate;

	float Red;
	float Blue;
	float Green;
	float Alpha;

	unsigned int uiGeometrySize = _pvGeometry->size();

	pTempEffect->SetTexture("gDiffuseMap",m_pAssetManager->GetTexture(_textureID));
	pTempEffect->SetMatrix( "gViewProjection", &( CameraInverse * *m_pCamera->GetProjectionMatrix( ) ) );

	pTempEffect->SetInt("gSourceBlend", _sourceBlend);
	pTempEffect->SetInt("gDestinationBlend", _destinationBlend);

	m_d3dpDevice->SetVertexDeclaration( pModel->m_d3dpVertexDeclaration );
	m_d3dpDevice->SetStreamSource( 0, pModel->m_d3dpVertBuffer, 0, sizeof( CAssetManager::tVERTCLRMESH ) );
	m_d3dpDevice->SetIndices( pModel->m_d3dpIndexBuffer );

	for(unsigned int geometryIter = 0; geometryIter < uiGeometrySize; geometryIter++)
	{
		Red   = 0.0f;
		Blue  = 0.0f;
		Green = 0.0f;
		Alpha = 0.0f;

		tGeometry tempGeometry = (*_pvGeometry)[geometryIter];
		D3DXMatrixIdentity(&tempGeometryMatrix);

		D3DXMatrixScaling(&tempGeometryScale, tempGeometry.m_d3dScale.x, tempGeometry.m_d3dScale.y,tempGeometry.m_d3dScale.z);
		D3DXMatrixRotationYawPitchRoll(&tempGeometryRotation, tempGeometry.m_d3dRotation.x, tempGeometry.m_d3dRotation.y, tempGeometry.m_d3dRotation.z);

		D3DXMatrixTranslation(&tempGeometryTranslate, tempGeometry.m_d3dPosition.x, tempGeometry.m_d3dPosition.y, tempGeometry.m_d3dPosition.z);

		tempGeometryMatrix = tempGeometryScale * tempGeometryRotation /*tempGeometryRotationX * tempGeometryRotationY * tempGeometryRotationZ*/  * tempGeometryTranslate;

		unsigned int Color = tempGeometry.m_uiColor;

		Blue  = (float) ( Color << 24 >> 24) / 255; 
		Green = (float) ( Color << 16 >> 24) / 255;
		Red   = (float) ( Color << 8 >> 24) / 255; 
		Alpha = (float) ( Color >> 24) / 255;


		pTempEffect->SetMatrix( "gMVP", &(tempGeometryMatrix * (CameraInverse* *m_pCamera->GetProjectionMatrix( ) ) ) );
		pTempEffect->SetMatrix( "gWorld", &tempGeometryMatrix);
		pTempEffect->SetVector( "gColor", &D3DXVECTOR4(Red, Green, Blue, Alpha));

		unsigned int nPasses = 0;

		pTempEffect->Begin( &nPasses, 0 );
		{
			for( unsigned int nPassIter = 0; nPassIter < nPasses; ++nPassIter )
			{
				pTempEffect->BeginPass( nPassIter );

				m_d3dpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pModel->m_nNumberOfVerts, 0, pModel->m_nNumberOfTriangles );

				pTempEffect->EndPass( );
			}
		}
		pTempEffect->End( );
	}
}

void CRenderer::SetGamma(float _Gamma)
{
	WORD sRamp[3][256];

	HDC hDC = GetDC(m_hWnd);

	if(GetDeviceGammaRamp(hDC, sRamp) == false)
	{
		cout << "WARNING!!! Gamma not supported" << '\n';
		return;
	}

	if(m_fGammaValue < -1.0f)
	{
		m_fGammaValue = -1.0f;
	}
	else if(m_fGammaValue > 1.0f)
	{
		m_fGammaValue = 1.0f;
	}
	else
	{
		m_fGammaValue = _Gamma;
	}

	for(int nGammaIndex = 0; nGammaIndex < 256; ++nGammaIndex)
	{
		sRamp[0][nGammaIndex] = (WORD)(min(255, nGammaIndex * (m_fGammaValue + 1.0f))) << 8;
		sRamp[1][nGammaIndex] = (WORD)(min(255, nGammaIndex * (m_fGammaValue + 1.0f))) << 8;
		sRamp[2][nGammaIndex] = (WORD)(min(255, nGammaIndex * (m_fGammaValue + 1.0f))) << 8;
	}
}

void CRenderer::QuickSort(vector<CEntity *> & _vpRenderableObjects, int _nLeft, int _nRight)
{
	int nLeftSide = _nLeft, nRightSide = _nRight;
	CEntity* pEntity;
	CEntity* pPivot = (CEntity*)_vpRenderableObjects[(_nLeft + _nRight) / 2];

	//Loop to sort through the array
	while (nLeftSide <= nRightSide)
	{
		while (D3DXVec3Length(&(_vpRenderableObjects[nLeftSide]->GetPosition() - m_pCamera->GetPosition( ) ) ) > D3DXVec3Length(&(pPivot->GetPosition( ) - m_pCamera->GetPosition( ) ) ) )
		{
			nLeftSide++;
		}
		while (D3DXVec3Length(&(_vpRenderableObjects[nRightSide]->GetPosition() - m_pCamera->GetPosition( ) ) ) > D3DXVec3Length(&(pPivot->GetPosition( ) - m_pCamera->GetPosition( ) ) ) )
		{
			nRightSide--;
		}

		//Swap the values inside
		if (nLeftSide <= nRightSide)
		{
			pEntity = _vpRenderableObjects[nLeftSide];
			_vpRenderableObjects[nLeftSide] = _vpRenderableObjects[nRightSide];
			_vpRenderableObjects[nRightSide] = pEntity;
			nLeftSide++;
			nRightSide--;
		}
	}

	if (_nLeft < nRightSide)
	{
		QuickSort(_vpRenderableObjects, _nLeft, nRightSide);
	}

	if (nLeftSide < _nRight)
	{
		QuickSort(_vpRenderableObjects, nLeftSide, _nRight);
	}
}

/*****************************************************************
* Shutdown():  
* 			      
*			      
*			      
*
* Ins:						void
*      		      
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/
void CRenderer::Shutdown( )
{
	m_pAssetManager->Shutdown( );
	for(unsigned int i = 0 ; i < m_vLights.size(); i++)
	{
		delete m_vLights[i];
	}
	m_vLights.clear();

	delete m_ptBitmapfont;

	SAFE_RELEASE( m_d3dpSkyBoxTexture );

	SAFE_RELEASE( m_d3dpQuadBuff );
	SAFE_RELEASE( m_d3dpQuadIndex );
	SAFE_RELEASE( m_d3dpQuadVertexDeclaration );

	SAFE_RELEASE( m_d3dpCubeVertBuffer );
	SAFE_RELEASE( m_d3dpCubeIndexBuffer );
	SAFE_RELEASE( m_d3dpCubeVertexDeclaration );

	SAFE_RELEASE(m_d3dpEffectPool);

	SAFE_RELEASE( m_d3dpSpriteManager );
	SAFE_RELEASE( m_d3dpDevice );
	SAFE_RELEASE( m_d3dpObject );
}