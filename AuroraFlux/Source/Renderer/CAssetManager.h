#ifndef	ASSETMANAGER_H
#define ASSETMANAGER_H

/***********************************************
 * Filename:  		CAssetManager.h
 * Date:      		11/29/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan W. Schotanes
 * Purpose:   		This class holds all objects within the game
 *
 ************************************************/

#include "../StdAfx.h"
#include "../Definitions.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

class CAssetManager
{
public:
	enum ModelVectorNumbers{eMODEL_VECTOR = 0, eGEOMETRY_VECTOR, eMAIN_MENU_MODEL_VECTOR};

	struct tVERTCLR
	{
		D3DXVECTOR3						m_d3dPos;
		D3DXVECTOR2						m_d3dUV;
	};

	struct tVERTCLRMESH
	{
		D3DXVECTOR3						m_d3dPos;
		D3DXVECTOR3						m_d3dNorm;
		D3DXVECTOR3						m_d3dBinormal;
		D3DXVECTOR3						m_d3dTangent;
		D3DXVECTOR2						m_d3dUV;
	};

	struct tMesh
	{
		string							m_szMeshFileName;
		short							m_chTextureNumber;
		//char							m_chType;
		//int								m_nRadius;
		//int								m_nLength;
		ID3DXMesh*						m_d3dpMesh;
	};
	
	struct tTexture
	{
		string							m_szTextureFileName;
		IDirect3DTexture9*				m_d3dpTexture;
	};
	
	struct tEffect
	{
		string							m_szEffectFileName;
		ID3DXEffect*					m_d3dpEffect;
	};

	struct tModel
	{
		string							m_szModelFileName;

		short							m_chTextureNumber;
		short							m_chTextureNormNumber;
		short							m_chTextureSpecNumber;

		INT32 							m_nNumberOfTriangles;
		INT32 							m_nNumberOfVerts;

		vector<tVertex>					m_vUniqueVerts;	
		vector<tTriangle>				m_vTriangles;	

		IDirect3DVertexBuffer9*			m_d3dpVertBuffer;
		IDirect3DIndexBuffer9*			m_d3dpIndexBuffer;
		IDirect3DVertexDeclaration9*	m_d3dpVertexDeclaration;
	};

private:

	vector<tMesh*>						m_pMeshs;
	vector<tTexture*>					m_pTextures;
	vector<tEffect*>					m_pEffects;
	vector<tModel*>						m_pModels;
	vector<tModel*>						m_pGeometry;
	vector<tModel*>						m_pMainMenuModels;

	IDirect3DDevice9*					m_d3dpDevice;
	ID3DXEffectPool*					m_d3dpEffectPool;
	IDirect3DTexture9*					m_d3dpDefaultTexture;
	
	/*****************************************************************
	* LoadModel():  Helper function to Push back to the selected vector
	* 			      
	*			      
	*			      
	*
	* Ins:						LPCTSTR _chpModelFileName
	*							int _nVectorToPushTo
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadModel(LPCTSTR _chpModelFileName,int _nVectorToPushTo);

public:
	void OnLostDevice();
	/*****************************************************************
	* CAssetManager():  Default Constructor
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

	CAssetManager(void);

	/*****************************************************************
	* ~CAssetManager():  Destructor
	* 
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

	~CAssetManager(void);
	
	/*****************************************************************
	* Shutdown():  Shuts down and sets everything to null
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

	void Shutdown();

	/*****************************************************************
	* LoadMesh():		Loads a mesh from file into a vector
	* 					and returns the mesh number
	*			      
	*			      
	*
	* Ins:						float _fRadius
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadMeshSphere(LPCTSTR _chpMeshName, float _fRadius);

	
	/*****************************************************************
	* LoadMesh():		Loads a mesh from file into a vector
	* 					and returns the mesh number
	*			      
	*			      
	*
	* Ins:						float _fRadius
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadMeshCapsule(LPCTSTR _chpMeshName, float _fRadius,float _fLength);

	/*****************************************************************
	* LoadTexture():			Loads a texture from file into a vector
	* 			      			and returns the texture number
	*			      
	*			      
	*
	* Ins:						LPCTSTR _chpTextureFileName
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadTexture(LPCTSTR _chpTextureFileName);
	
	/*****************************************************************
	* LoadEffect():				Loads a effect from file into a vector
	* 			      			and returns the effect number
	*			      
	*			      
	*
	* Ins:						LPCTSTR _chpEffectFileName
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadEffect(LPCTSTR _chpEffectFileName);

	/*****************************************************************
	* LoadModel():				Loads a model from file into a vector
	* 			      			and returns the model number
	*			      
	*			      
	*
	* Ins:						LPCTSTR _chpModelFileName
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	short LoadModel(LPCTSTR _chpModelFileName);

	
	/*****************************************************************
	* LoadGeometry():			Loads a model from file into a vector
	* 			      			and returns the model number(used for Geometry Particles)
	*			      
	*			      
	*
	* Ins:						LPCTSTR _chpModelFileName
	*  
	*
	* Outs:						None
	*
	* Returns:					unsigned char
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/
	short LoadGeometry(LPCTSTR _chpModelFileName);

	short LoadMainMenuModel(LPCTSTR _chpModelFileName);
	/***************
	*  Accessors
	***************/


	/*****************************************************************
	* GetMesh():				Returns from the selected slot in the mesh vector
	* 			      
	*			      
	*			      
	*
	* Ins:						unsigned char _MeshWanted
	*  
	*
	* Outs:						None
	*
	* Returns:					ID3DXMesh*
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	tMesh* GetMesh(int _chMeshWanted);

	/*****************************************************************
	* GetTexture():				Returns from the selected slot in the texture vector
	* 			      
	*			      
	*			      
	*
	* Ins:						unsigned char _nTextureWanted
	*  
	*
	* Outs:						None
	*
	* Returns:					IDirect3DTexture9*
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	IDirect3DTexture9* GetTexture(int _chTextureWanted);

	/*****************************************************************
	* GetEffect():			Returns from the selected slot in the Effect vector
	* 			      
	*			      
	*			      
	*
	* Ins:						unsigned char _nEffectWanted
	*  
	*
	* Outs:						None
	*
	* Returns:					ID3DXEffect*
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	ID3DXEffect* GetEffect(int _chEffectWanted);

	/*****************************************************************
	* GetModel():				Returns from the selected slot in the Model vector
	* 			      
	*			      
	*			      
	*
	* Ins:						_nModelWanted
	*  
	*
	* Outs:						None
	*
	* Returns:					tModel*
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	tModel* GetModel(int _chModelWanted);

	/*****************************************************************
	* GetGeometry():				Returns from the selected slot in the GetGeometry vector
	* 			      
	*			      
	*			      
	*
	* Ins:						_nModelWanted
	*  
	*
	* Outs:						None
	*
	* Returns:					tModel*
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/
	tModel* GetGeometry(int _chGeometryWanted);

	tModel* GetMainMenuModel(int _chGeometryWanted);
	/***************
	* Mutators
	***************/


	/*****************************************************************
	* SetDevice():				sets the d3ddevice pointer
	* 			      
	*			      
	*			      
	*
	* Ins:						IDirect3DDevice9* _D3DDevice
	*  
	*
	* Outs:						None
	*
	* Returns:					void
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	void SetDevice(IDirect3DDevice9* _D3DDevice) {m_d3dpDevice = _D3DDevice;}

	/*****************************************************************
	* SetEffectPool():			Sets the effect pool pointer
	* 			      
	*			      
	*			      
	*
	* Ins:						ID3DXEffectPool* _EffectPool
	*  
	*
	* Outs:						None
	*
	* Returns:					void
	*
	* Mod. Date:				11/29/2012
	* Mod. Initials:			BS
	*****************************************************************/

	void SetEffectPool(ID3DXEffectPool* _EffectPool) {m_d3dpEffectPool = _EffectPool;}
};

#endif