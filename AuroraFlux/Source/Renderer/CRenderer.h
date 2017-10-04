#ifndef RENDERER_H
#define RENDERER_H

/***********************************************
* Filename:  		CAssetManager.h
* Date:      		11/29/2012
* Mod. Date: 		2/5/2013
* Mod. Initials:	BS
* Author:    		Bryan W. Schotanes
* Purpose:   		This class holds all objects within the game
*
************************************************/

#include "../StdAfx.h"

#include "CBitmapFont.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "CRenderTarget.h"

#include "../Object Manager/ObjectManager.h"

#include "../Camera/Camera.h"

#include "../Collision/Physics.h"

#include "../Timer/CTimer.h"

#include "../Entity/Shield.h"
#include "../Entity/Missile.h"
#include "../Entity/Player.h"
#include "../Entity/Asteroid.h"
#include "../Entity/Lasers.h"
#include "../Entity/Boss.h"
#include "../Entity/BossWinTurret.h"

#include "../Particle System/EffectSystem.h"
#include "../Particle System/ParticleSystem.h"
#include "../Particle System/GeometrySystem.h"
#include "../Input/Input.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

class CRenderer 
{
	public:
		/*****************************************************************
		* CRenderer():				Default Constructor
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

		CRenderer( );

		/*****************************************************************
		* Init():					Inititalizes default values
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

		bool Init( HWND _hWnd, float _fScreenWidth, float _fScreenHeight, bool _bIsWindowed, bool _bVsync, CCamera* _pCamera,CAssetManager* _pAssetManager );
		/*****************************************************************
		* RenderObjects():			Renders the 
		*							- skybox
		*							- Collision Spheres (if Debug)
		*							- list of objects
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
		
		void RenderObjects( const vector< CEntity * >* _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects, const vector<CEffectSystem*>& _vpEffects ,const vector<CEntity *> * _vpTransparentObjects);

		void RenderMainMenuObjects( const vector< CEntity * >* _vpRenderableObjects, const vector<CEffectSystem*>& _vpEffects );
		/*****************************************************************
		* Render2d():				Renders a 2d Object to a full screen plane
		*							1.0f Alpha
		* 			      
		*			      
		*
		* Ins:						int _ntextureToRender
		*      						//RECT* _tRectToRender
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				11/29/2012
		* Mod. Initials:			BS
		*****************************************************************/

		void Render2d( int _nTextureToRender );

		/*****************************************************************
		* Render2d():				renders a 2d object to a full screen plane
		* 							_fAlpha alpha
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

		void Render2d( int _nTextureToRender, float _fAlpha );

		/*****************************************************************
		* Render2d():				renders a 2d object to a full screen plane
		* 			      			_fAlpha alpha
		*							(ONLY USED FOR RENDERING DURING LOADING!)
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

		void Render2d( IDirect3DTexture9* _d3dpTextureToRender, float _fAlpha );

		/*****************************************************************
		* RenderArrow():			Draws the arrow with the sprite manager
		*							_tRectToRender = Image size rect
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

		void RenderArrow( int _nTextureToRender, RECT *_ptRectToRender, D3DXMATRIX _d3dMatrix );
		void RenderArrowAndrewR( int _nTextureToRender, RECT *_ptRectToRender, D3DXMATRIX _d3dMatrix, D3DCOLOR color, bool _center = true, bool _ScreenRatioScale = true );

		/*****************************************************************
		* RenderText():				Text in the Bitmap Font to the screen
		* 			      
		*			      
		*			      
		*
		* Ins:						const char* szTextToPrint
		*							int nPosX
		*							int nPosY
		*							float fScale
		*							float fRotX
		*							float fRotY
		*							float fRot
		*							DWORD dwColor
		*							float ArmorPercent
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderText( const char* szTextToPrint ,int nPosX ,int nPosY ,float fScale ,float fRotX ,float fRotY ,float fRot ,DWORD dwColor,bool _ScreenRatioScale = true );

		
		/*****************************************************************
		* RenderButton():			Renders Buttons to the screen, top left position, button number(enum) , scale and if the button is selected
		* 			      
		*			      
		*			      
		*
		* Ins:						float _fTopLeftPosX
		*							float _fTopLeftPosY
		*							int _nButton
		*							float _fScaleX
		*							float _fScaleY
		*							bool _bButtonSelected
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderButton(float _fTopLeftPosX,float _fTopLeftPosY,int _nButton, float _fScaleX, float _fScaleY, bool _bButtonSelected, LONG _rLeft = 0, LONG _rTop = 0, LONG _rRight = 512, LONG _rBottom = 128, D3DCOLOR _color = D3DCOLOR_ARGB(255,255,255,255));

		/*****************************************************************
		* Shutdown():				shuts down the class and deletes all objects
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

		void Shutdown( void );

		/***************
		* Mutators
		***************/
		CCamera* GetCamera() {return m_pCamera;}
		void SetCamera( CCamera* _pCamera ) { m_pCamera = _pCamera; }
		
		void SetTime( CTimer* _pVal ) { m_pTime = _pVal; }
		void SetPaused(bool _paused) {m_bIsPaused = _paused;}
		void SetPlayer( CEntity* _player ) { m_pPlayer = _player; }
		void SetAssetManager(CAssetManager* _assetManager) {m_pAssetManager = _assetManager;}
		void SetGamma(float _Gamma);
		void Clear(long _lWhatToClear,unsigned char _cRed, unsigned char _cGreen, unsigned char _cBlue);

		/***************
		*  Accessors
		***************/

		IDirect3DDevice9* GetDevice( ) { return m_d3dpDevice; }
		ID3DXEffectPool* GetEffectPool() {return m_d3dpEffectPool;}
		CAssetManager* GetAssetManager( ) {return m_pAssetManager; }

		float GetGamma() {return m_fGammaValue;}
		float GetXRatio() {return m_fXRatio;}
		float GetYRatio() {return m_fYRatio;}
		bool GetIsPaused(void) {return m_bIsPaused;}

	private:

		struct tVERTPOS
		{
			D3DXVECTOR3 m_d3dPos;
		};

		struct tVERTPOSUV
		{
			D3DXVECTOR3 m_d3dPos;
			D3DXVECTOR2 m_d3dUV;
		};

		struct tVERTPOSNORMUV
		{
			D3DXVECTOR3						m_d3dPos;
			D3DXVECTOR3						m_d3dNorm;
			D3DXVECTOR2						m_d3dUV;
		};

		struct tParticleMatColor
		{
			D3DXMATRIX m_d3dMat;
			D3DXVECTOR4 m_d3dColor;
		};
		HWND							m_hWnd;
		D3DPRESENT_PARAMETERS			m_d3dPP;
		D3DSURFACE_DESC					m_d3dBackBufferDesc;

		CTimer*							m_pTime;
		CCamera*						m_pCamera;
		CBitmapFont*					m_ptBitmapfont;

		IDirect3D9*						m_d3dpObject;
		IDirect3DDevice9*				m_d3dpDevice;
		ID3DXSprite*					m_d3dpSpriteManager;
		ID3DXEffectPool*				m_d3dpEffectPool;

		CAssetManager*					m_pAssetManager;

		float							m_fTime;
		float							m_fGammaValue;
		float							m_fXRatio;
		float							m_fYRatio;

		// SHADERS
		short					m_chMenuShader;
		short					m_chGBufferShader;
		short					m_chTransparencyShader;
		short					m_chPostProcessShader;
		short					m_chSkyBoxShader;
		short					m_chPointLightShader;
		short					m_chLaserLightShader;
		short					m_chFrustumShader;
		short					m_chFinalPassShader;

		// TEXTURES
		short					m_chButtonTexture;
		short					m_chShieldTexture;
		short					m_chWhiteTexture;
		short					m_chEnemyShieldTexture;

		// Quad Matrix
		D3DXMATRIX						m_d3dSkyBoxMatrix;

		// Cube Textures
		IDirect3DCubeTexture9*			m_d3dpSkyBoxTexture;
		IDirect3DCubeTexture9*			m_d3dpStarBoxTexture;
		IDirect3DCubeTexture9*			m_d3dpFogTexture;

		//	Cube Buffers
		IDirect3DVertexBuffer9*			m_d3dpCubeVertBuffer;
		IDirect3DIndexBuffer9*			m_d3dpCubeIndexBuffer;

		//	Vertex Declarations
		IDirect3DVertexDeclaration9*	m_d3dpCubeVertexDeclaration;
		IDirect3DVertexDeclaration9*	m_d3dpQuadVertexDeclaration;
		IDirect3DVertexDeclaration9*	m_d3dpFrustumVertexDeclaration;

		//	Quad Buffers
		IDirect3DIndexBuffer9*			m_d3dpQuadIndex;

		IDirect3DVertexBuffer9*			m_d3dpQuadBuff;
		
		//	Frustum Buffers
		
		IDirect3DIndexBuffer9*			m_d3dpFrustumIndex;
		IDirect3DVertexBuffer9*			m_d3dpFrustumBuff;

		// Render Targets
		CRenderTarget					m_DepthTarget;
		CRenderTarget					m_DiffuseTarget;
		CRenderTarget					m_NormalTarget;
		CRenderTarget					m_SpecularTarget;
		CRenderTarget					m_LightTarget;
		CRenderTarget					m_PostProcessTarget;

		//	Light lists
		vector<Light*>					m_vLights;

		//	Shield Lists, Laser Lists
		vector<CEntity *>				m_vShields;

		// Entity Points
		CEntity*						m_pPlayer;
		CEntity*						m_pBoss;
		CEntity*						m_pBossTurret;
		bool							m_bIsPaused;
		bool							m_bBossFound;
		/*****************************************************************
		* RenderCollisionDEBUG():  renders collision debug spheres
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

		void RenderCollisionDEBUG( const vector<CEntity*> *_vpRenderableObjects,int _shader, string _technique );

		/*****************************************************************
		* CreateSkyBox():			creates the skybox and sets default values
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

		void CreateSkyBox( );

		/*****************************************************************
		* CreateQuad():				creates the quad and sets default values
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

		void CreateQuad( );

		/*****************************************************************
		* RenderSkyBox():			renders the skybox
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

		void RenderSkyBox();

		/*****************************************************************
		* RenderAllTargets():  This function will render all targets with the selected shader and technique
		* 			      
		*			      
		*			      
		*
		* Ins:						const vector<CEntity *> * _vpRenderableObjects
		*      						string _szTechnique
		*
		* Outs:						None
		*
		* Returns:					void
		*
		* Mod. Date:				11/29/2012
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderAllTargets(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects,int _nShader, string _szTechnique);
		void RenderMainMenuAllTargets(const vector<CEntity *> * _vpRenderableObjects,int _nShader, string _szTechnique);

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

		void RenderGBuffers(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects);
		void RenderMainMenuGBuffers(const vector<CEntity *> * _vpRenderableObjects);
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

		void RenderFowardPass();

		/*****************************************************************
		* RenderLights():				Renders the Lights
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
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderLights(const vector<CEntity *> * _vpRenderableObjects,const vector< CAmbientBoundingBox * >* _vpRenderableAmbientObjects);
		void RenderMainMenuLights(const vector<CEntity *> * _vpRenderableObjects);
		/*****************************************************************
		* RenderPostProcess():		Renders the Post Process Pass
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
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderPostProcess();

		/*****************************************************************
		* RenderFrustrum():			Renders the Frustrum (IN PROGRESS)
		* 			      
		*
		* Ins:						void
		*      		      
		*
		* Outs:						None
		*
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/
		void CreateFrustrum();
		void RenderFrustrum();


		/*****************************************************************
		* RenderGeometry():			Renders the geometry array, with the selected texture, mesh , and source/dest blend modes
		* 			      
		*			      
		*			      
		*
		* Ins:						const vector<tGeometry>* _pvGeometry
		*							int _textureID
		*							int _meshID
		*							int _sourceBlend
		*							int _destinationBlend
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderGeometry(const vector<tGeometry>* _pvGeometry, int _textureID, int _meshID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID = 0 );

		/*****************************************************************
		* RenderParticles():			Renders the geometry array, with the selected texture, mesh , and source/dest blend modes
		* 			      
		*			      
		*			      
		*
		* Ins:						vector<tParticles>* _pvParticles
		*							int _textureID
		*							int _sourceBlend
		*							int _destinationBlend
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderParticles(const vector<tParticles>* _pvParticles, int _textureID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID = 0);
		void RenderMainMenuParticles( const vector<tParticles>* _pvParticles, int _textureID, int _sourceBlend, int _destinationBlend,int _shader, string _technique, int _nEffectID /*= 0*/ );

		/*****************************************************************
		* RenderTransparency():			Renders transparent objects to the screen
		* 			      
		*			      
		*			      
		*
		* Ins:						void
		*
		* Outs:						None
		*							
		* Returns:					void
		*
		* Mod. Date:				1/30/2013
		* Mod. Initials:			BS
		*****************************************************************/

		void RenderTransparency(const vector<CEntity*> *_vpRenderableObjects,const vector<CEntity *> * _vpTransparentObjects,int _shader, string _technique, const vector<CEffectSystem*>& _vpEffects);
		void RenderMainMenuTransparency(const vector<CEntity*> *_vpRenderableObjects,int _shader, string _technique, const vector<CEffectSystem*>& _vpEffects);

		void QuickSort(vector<CEntity *> & _vpRenderableObjects, int _nLeft, int _nRight);

};
#endif 