#include "CAssetManager.h"
/***********************************************
 * Filename:  		CAssetManager.cpp
 * Date:      		11/29/2012
 * Mod. Date: 		11/29/2012
 * Mod. Initials:	BS
 * Author:    		Bryan W. Schotanes
 * Purpose:   		This class holds all objects within the game
 *
 ************************************************/

/*****************************************************************
	* CAssetManager():  
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

CAssetManager::CAssetManager(void)
{
	m_d3dpDefaultTexture = NULL;
}

/*****************************************************************
* ~CAssetManager():  
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

CAssetManager::~CAssetManager(void)
{

}
void CAssetManager::OnLostDevice()
{
	//vector<tMesh*> tempMeshs = m_pMeshs;
	//m_pMeshs.clear();
	//
	//for(unsigned int numOfMeshes = 0; numOfMeshes < m_pMeshs.size(); numOfMeshes++)
	//{
	//	tMesh* tempMesh = tempMeshs[numOfMeshes];
	//	if(tempMesh->m_chType == eSPH)
	//	{
	//		LoadMeshSphere(tempMesh->m_szMeshFileName.c_str(),tempMesh->m_nRadius);
	//	}
	//	else
	//	{
	//		LoadMeshCapsule(tempMesh->m_szMeshFileName.c_str(),tempMesh->m_nRadius,tempMesh->m_nLength);
	//	}
	//}
	//
	//for(unsigned int i = 0; i < tempMeshs.size(); i++)
	//{
	//	SAFE_RELEASE(tempMeshs[i]->m_d3dpMesh);
	//	delete tempMeshs[i];
	//}
	//
	//tempMeshs.clear();
	//
	//for(unsigned int numOfEffects = 0 ; numOfEffects < m_pEffects.size() ; numOfEffects++)
	//{
	//	m_pEffects[numOfEffects]->m_d3dpEffect->OnLostDevice();
	//}
}
/*****************************************************************
* LoadMesh():  
* 			      
*			      
*			      
*
* Ins:						LPCTSTR _chpMeshFileName
*  
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

short CAssetManager::LoadMeshSphere(LPCTSTR _chpMeshName,float _fRadius)
{
	string szTempFilePath = _chpMeshName;

	for(unsigned int i = 0; i < m_pMeshs.size(); i++)
	{
		if(m_pMeshs[i]->m_szMeshFileName == _chpMeshName)
		{
			return i;
		}
	}

	tMesh* _tSphere = new tMesh;

	//_tSphere->m_chType = eCAP;
	_tSphere->m_szMeshFileName = szTempFilePath;
	_tSphere->m_chTextureNumber = -1;
	//_tSphere->m_nRadius = _fRadius;
	//_tCapsule->m_nLength = -1;

	D3DXCreateSphere(m_d3dpDevice,_fRadius,15,15,&_tSphere->m_d3dpMesh,NULL);

	if(_tSphere->m_d3dpMesh == NULL )
	{
		cout << "FAILED TO LOAD MESH: " << szTempFilePath << endl;
	}

	m_pMeshs.push_back(_tSphere);

	return m_pMeshs.size()-1;
}

short CAssetManager::LoadMeshCapsule(LPCTSTR _chpMeshName,float _fRadius,float _fLength)
{
	string szTempFilePath = _chpMeshName;

	for(unsigned int i = 0; i < m_pMeshs.size(); i++)
	{
		if(m_pMeshs[i]->m_szMeshFileName == _chpMeshName)
		{
			return i;
		}
	}

	tMesh* _tCapsule = new tMesh;
	//_tCapsule->m_chType = eCAP;
	_tCapsule->m_szMeshFileName = szTempFilePath;
	_tCapsule->m_chTextureNumber = -1;
	//_tCapsule->m_nRadius = _fRadius;
	//_tCapsule->m_nLength = _fLength;

	D3DXCreateCylinder(m_d3dpDevice,_fRadius,_fRadius,_fLength,30,30,&_tCapsule->m_d3dpMesh,NULL);

	if(_tCapsule->m_d3dpMesh == NULL )
	{
		cout << "FAILED TO LOAD MESH: " << szTempFilePath << endl;
	}

	m_pMeshs.push_back(_tCapsule);

	return m_pMeshs.size()-1;
}

/*****************************************************************
* LoadTexture():  
* 			      
*			      
*			      
*
* Ins:						LPCTSTR _chpTextureFileName
*  
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

short CAssetManager::LoadTexture(LPCTSTR _chpTextureFileName)
{
	string szTexture = _chpTextureFileName;
	if(m_d3dpDefaultTexture == NULL && strcmp(_chpTextureFileName,szTexture.c_str()))
	{
		m_d3dpDefaultTexture = GetTexture(LoadTexture("Default.png"));
	}

	string szTempFilePath = "Resources/Textures/";
	szTempFilePath += _chpTextureFileName;

	//string szTexture = _chpTextureFileName;
	string sztemp;
	if(szTexture.size() > 12)
	{
		sztemp = _chpTextureFileName+(szTexture.size()-11);
		for(unsigned int iter = 0; iter < 4 ; iter++)
			sztemp.pop_back();
	}
	else
	{
		sztemp = "Skip";
	}
	

	for(unsigned int i = 0; i < m_pTextures.size(); i++)
	{
		if(m_pTextures[i]->m_szTextureFileName ==  _chpTextureFileName)
		{
			return i;
		}
	}

	tTexture* tpTempTexture = new tTexture();

	tpTempTexture->m_szTextureFileName = _chpTextureFileName;
	HRESULT check;

	check = D3DXCreateTextureFromFile(m_d3dpDevice,szTempFilePath.c_str(),&tpTempTexture->m_d3dpTexture);
	
	//if(tpTempTexture->m_d3dpTexture != NULL )
	//{
	//	cout << "Loaded Texture: " << szTempFilePath << endl;
	//}
	//else
	if(check != S_OK )
	{
		if(sztemp != "pecular" && sztemp != "_Normal")
		{
			cout << "FAILED TO LOAD TEXTURE: " << szTempFilePath << endl;
		}
		delete tpTempTexture;
		return LoadTexture("Default.png");
		//return -1;
	}

	m_pTextures.push_back(tpTempTexture);

	return m_pTextures.size()-1;
}

/*****************************************************************
* LoadEffect():  
* 			      
*			      
*			      
*
* Ins:						LPCTSTR _chpEffectFileName
*  
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/

short CAssetManager::LoadEffect(LPCTSTR _chpvEffectFileName)
{
	string szTempFilePath = "Resources/Shaders/";
	szTempFilePath += _chpvEffectFileName;

	for(unsigned int i = 0; i < m_pEffects.size(); i++)
	{
		if(m_pEffects[i]->m_szEffectFileName == _chpvEffectFileName)
		{
			return i;
		}
	}
	
	tEffect* tpTempEffect = new tEffect;

	tpTempEffect->m_szEffectFileName = _chpvEffectFileName;

	HRESULT check;
#ifdef _DEBUG
	check = D3DXCreateEffectFromFile(m_d3dpDevice,szTempFilePath.c_str(),0,0,D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION,m_d3dpEffectPool,&tpTempEffect->m_d3dpEffect,0);
#else
	check = D3DXCreateEffectFromFile(m_d3dpDevice,szTempFilePath.c_str(),0,0,0,m_d3dpEffectPool,&tpTempEffect->m_d3dpEffect,0);
#endif
	//if(tpTempEffect->m_d3dpEffect != NULL )
	//{
	//	cout << "Loaded Effect: " << szTempFilePath << endl;
	//}
	//else
	if(check != S_OK )
	{
		cout << "FAILED TO LOAD EFFECT: " << szTempFilePath << endl;
		delete tpTempEffect;
		//return LoadEffect("GBuffers.fx");
		return -1;
	}

	m_pEffects.push_back(tpTempEffect);

	return m_pEffects.size()-1;
}

/*****************************************************************
* LoadModel():  
* 			      
*			      
*			      
*
* Ins:						LPCTSTR _chpModelFileName
*  
*
* Outs:						None
*
* Returns:					void
*
* Mod. Date:				11/29/2012
* Mod. Initials:			BS
*****************************************************************/
short CAssetManager::LoadModel(LPCTSTR _chpModelFileName)
{
	string tempFileName = _chpModelFileName;

	for(unsigned int i = 0; i < m_pModels.size(); i++)
	{
		if(m_pModels[i]->m_szModelFileName == tempFileName)
		{
			return i;
		}
	}

	return LoadModel(_chpModelFileName,eMODEL_VECTOR);
}

short CAssetManager::LoadGeometry(LPCTSTR _chpModelFileName)
{
	string tempFileName = _chpModelFileName;

	for(unsigned int i = 0; i < m_pGeometry.size(); i++)
	{
		if(m_pGeometry[i]->m_szModelFileName == tempFileName)
		{
			return i;
		}
	}
	return LoadModel(_chpModelFileName,eGEOMETRY_VECTOR);
}
short CAssetManager::LoadMainMenuModel(LPCTSTR _chpModelFileName)
{
	string tempFileName = _chpModelFileName;

	for(unsigned int i = 0; i < m_pMainMenuModels.size(); i++)
	{
		if(m_pMainMenuModels[i]->m_szModelFileName == tempFileName)
		{
			return i;
		}
	}
	return LoadModel(_chpModelFileName,eMAIN_MENU_MODEL_VECTOR);
}
//int CAssetManager::LoadModelSpecNorm(LPCTSTR _chpModelFileName)
//{
//	return LoadModel(_chpModelFileName,eMODELSPECNORM_VECTOR);
//}

short CAssetManager::LoadModel(LPCTSTR _chpModelFileName,int _nVectorToPushTo)
{
	string szTempFilePath = "Resources/Models/";
	int nEffectNumber = -1;

	szTempFilePath += _chpModelFileName;

	tModel* tpTempModel = new tModel;

	tpTempModel->m_szModelFileName = _chpModelFileName;

	{
		D3DVERTEXELEMENT9 vertclr_definition[]=
		{
			{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
			{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},
			{0,24,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_BINORMAL,0},
			{0,36,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TANGENT,0},
			{0,48,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
			D3DDECL_END()
		};

		m_d3dpDevice->CreateVertexDeclaration(vertclr_definition,&(tpTempModel->m_d3dpVertexDeclaration));

		fstream fin;

		CMesh pTempMesh;

		fin.open(szTempFilePath.c_str(),ios::in | ios::binary);

		INT32 nModelNameLength;
		fin.read((char*)&nModelNameLength,sizeof(INT32));

		char chModelNameBuffer[200];

		fin.read(chModelNameBuffer,nModelNameLength);
		pTempMesh.m_strName = chModelNameBuffer;

		INT32 nNumberOfTextures;
		fin.read((char*)&nNumberOfTextures,sizeof(INT32));

		for(int textureNumberIterator = 0; textureNumberIterator < nNumberOfTextures; textureNumberIterator++)
		{
			INT32 nLengthOfTextureName;
			fin.read((char*)&nLengthOfTextureName,sizeof(nLengthOfTextureName));

			char chTextureNameBuffer[512];
			fin.read(chTextureNameBuffer,nLengthOfTextureName);

			string szTempBuffer;
			string szFowardString;
			string szDiff = "_Diffuse";
			string szNorm = "_Normal";
			string szSpec = "_Specular";
			string szPng = ".png";
			// TODO: Bern Cut Later
			string szPinkiePie = "gnp.eiPeikniP";
			string szGreenTexture = "gnp.erutxeTneerG";
			string szBlueTexture = "gnp.HP_erutxeTelissiM_D2_TR";
			string szRedTexture = "gnp.HP_erutxeTresaL_D2_TR";
			
			for(unsigned int nameBufferIter = strlen(chTextureNameBuffer) - 1 ; nameBufferIter >= 0 ; nameBufferIter-- )
			{
				if(chTextureNameBuffer[nameBufferIter] == '/')
				{
					break;
				}
				else
				{
					szTempBuffer.push_back(chTextureNameBuffer[nameBufferIter]);
				}
			}

			if(szTempBuffer  == szPinkiePie || szTempBuffer == szGreenTexture || szTempBuffer == szBlueTexture || szTempBuffer == szRedTexture)
			{
				for(int tempBufferIter = szTempBuffer.size() ; tempBufferIter  > 4 ; tempBufferIter--)
				{
					szFowardString.push_back( szTempBuffer[ tempBufferIter - 1 ] );
				}

				int nTempTextureNum = LoadTexture((szFowardString+szPng).c_str());
				if(nTempTextureNum != -1)
					tpTempModel->m_chTextureNumber = nTempTextureNum;
				else
					tpTempModel->m_chTextureNumber = -1;

				tpTempModel->m_chTextureNormNumber = LoadTexture(("Default"+szNorm+szPng).c_str());
				tpTempModel->m_chTextureSpecNumber = LoadTexture(("Default"+szSpec+szPng).c_str());
			}
			else
			{
				for(int tempBufferIter = szTempBuffer.size() ; tempBufferIter  > 12 ; tempBufferIter--)
				{
					szFowardString.push_back( szTempBuffer[ tempBufferIter - 1 ] );
				}

				int nTempTextureNum = LoadTexture((szFowardString+szDiff+szPng).c_str());
				if(nTempTextureNum != -1)
					tpTempModel->m_chTextureNumber = nTempTextureNum;
				else
					tpTempModel->m_chTextureNumber = -1;
				
				nTempTextureNum = LoadTexture((szFowardString+szNorm+szPng).c_str());
				if(nTempTextureNum != -1)
					tpTempModel->m_chTextureNormNumber = nTempTextureNum;
				else 
					tpTempModel->m_chTextureNormNumber = LoadTexture(("Default"+szNorm+szPng).c_str());

				nTempTextureNum = LoadTexture((szFowardString+szSpec+szPng).c_str());
				if(nTempTextureNum  != -1)
					tpTempModel->m_chTextureSpecNumber = nTempTextureNum;
				else
					tpTempModel->m_chTextureSpecNumber = LoadTexture(("Default"+szSpec+szPng).c_str());
			}

			pTempMesh.m_vTextureNames.push_back(szFowardString.c_str());
		}


		fin.read((char*)&tpTempModel->m_nNumberOfVerts,sizeof(INT32));

		for(INT32 vertexIter = 0; vertexIter < tpTempModel->m_nNumberOfVerts; vertexIter++)
		{
			tVertex tempVertex;

			fin.read((char*)&tempVertex,sizeof(tVertex));
			tpTempModel->m_vUniqueVerts.push_back(tempVertex);
		}

		fin.read((char*)&tpTempModel->m_nNumberOfTriangles,sizeof(INT32));

		for(INT32 triangleIter = 0; triangleIter < tpTempModel->m_nNumberOfTriangles; triangleIter++)
		{
			tTriangle tempTriangle;

			fin.read((char*)&tempTriangle,sizeof(tTriangle));

			tpTempModel->m_vTriangles.push_back(tempTriangle);
		}

		fin.close();

		tVERTCLRMESH* quadverts = new tVERTCLRMESH[ tpTempModel->m_nNumberOfVerts ];

		for(INT32 vertIter = 0 ; vertIter < tpTempModel->m_nNumberOfVerts; vertIter++)
		{
			quadverts[vertIter].m_d3dPos = tpTempModel->m_vUniqueVerts[vertIter].fPos;
			quadverts[vertIter].m_d3dNorm = tpTempModel->m_vUniqueVerts[vertIter].fNormal;
			quadverts[vertIter].m_d3dUV = tpTempModel->m_vUniqueVerts[vertIter].fTexCoord;
			quadverts[vertIter].m_d3dBinormal = tpTempModel->m_vUniqueVerts[vertIter].fBinormal;
			quadverts[vertIter].m_d3dTangent = tpTempModel->m_vUniqueVerts[vertIter].fTangent;
		}

		m_d3dpDevice->CreateVertexBuffer((unsigned int)(sizeof(tVERTCLRMESH)*tpTempModel->m_nNumberOfVerts),0,0,D3DPOOL_MANAGED,&tpTempModel->m_d3dpVertBuffer,0);
		
		void *mem = 0;
		
		tpTempModel->m_d3dpVertBuffer->Lock(0,0,&mem,0);
		memcpy(mem,&quadverts[0],tpTempModel->m_nNumberOfVerts*sizeof(tVERTCLRMESH));
		tpTempModel->m_d3dpVertBuffer->Unlock();

		delete[] quadverts;

		short* quadindicies = new short[ tpTempModel->m_nNumberOfTriangles * 3 ];
		
		int TriNumber = 0;
		
		for(INT32 i = 0 ; i < tpTempModel->m_nNumberOfTriangles * 3 ; i += 3)
		{
			quadindicies[i] = tpTempModel->m_vTriangles[TriNumber].m_uIndex0;
			quadindicies[i+1] = tpTempModel->m_vTriangles[TriNumber].m_uIndex1;
			quadindicies[i+2] = tpTempModel->m_vTriangles[TriNumber].m_uIndex2;
			TriNumber++;
		}

		m_d3dpDevice->CreateIndexBuffer((sizeof(SHORT)*tpTempModel->m_nNumberOfTriangles*3),0,D3DFMT_INDEX16,D3DPOOL_MANAGED,&tpTempModel->m_d3dpIndexBuffer,0);
		mem = 0;
		tpTempModel->m_d3dpIndexBuffer->Lock(0,0,&mem,0);
		memcpy(mem,&quadindicies[0],(sizeof(SHORT)*tpTempModel->m_nNumberOfTriangles*3));
		tpTempModel->m_d3dpIndexBuffer->Unlock();

		delete[] quadindicies;
	}
	//cout << "Loaded Model: " << szTempFilePath << endl;

	switch (_nVectorToPushTo)
	{
	case eMODEL_VECTOR:
		{
			m_pModels.push_back(tpTempModel);
			return m_pModels.size()-1;
			break;
		}
	case eGEOMETRY_VECTOR:
		{
			m_pGeometry.push_back(tpTempModel);
			return m_pGeometry.size()-1;
			break;
		}
	case eMAIN_MENU_MODEL_VECTOR:
		{
			m_pMainMenuModels.push_back(tpTempModel);
			return m_pMainMenuModels.size()-1;
			break;
		}
	}
	return -1;
}

CAssetManager::tMesh* CAssetManager::GetMesh(int _chMeshWanted)
{
	//if(_chMeshWanted >= 0)
	//{
		return m_pMeshs[_chMeshWanted];
	//}
	//else
	//{
	//	cout << "Mesh not found" << endl;
	//	return m_pMeshs[0];
	//}
}

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

IDirect3DTexture9* CAssetManager::GetTexture(int _chTextureWanted) 
{
	if(_chTextureWanted >= 0)
	{
		return m_pTextures[_chTextureWanted]->m_d3dpTexture;
	}
	else
	{
		cout << "Texture not found" << endl;
		return m_d3dpDefaultTexture;
		//return GetTexture(LoadTexture("Default.png"));
	}
}

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

ID3DXEffect* CAssetManager::GetEffect(int _chEffectWanted) 
{
	//if(_chEffectWanted >= 0)
	//{
		return m_pEffects[_chEffectWanted]->m_d3dpEffect;
	//}
	//else
	//{
	//	cout << "Effect not found" << endl;
		//return GetEffect(LoadEffect("GBuffers.fx"));
	//}
	
}

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

CAssetManager::tModel* CAssetManager::GetModel(int _chModelWanted) 
{
	//if(_chModelWanted >= 0)
	//{
		return m_pModels[_chModelWanted];
	//}
	//else
	//{
	//	cout << "Model not found" << endl;
	//	return GetModel(LoadModel("RT_3D_Barrier_ModelShape.mesh"));
	//}
}

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
CAssetManager::tModel* CAssetManager::GetGeometry(int _chGeometryWanted) 
{
	//if(_chGeometryWanted >= 0)
	//{
		return m_pGeometry[_chGeometryWanted];
		//}
		//else
		//{
		//	cout << "Geometry not found" << endl;
		//	return GetGeometry(LoadGeometry("RT_3D_Barrier_ModelShape.mesh"));
		//}
}

CAssetManager::tModel* CAssetManager::GetMainMenuModel(int _chGeometryWanted)
{
	return m_pMainMenuModels[_chGeometryWanted];
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

void CAssetManager::Shutdown()
{
	for(unsigned int i = 0; i < m_pMeshs.size(); i++)
	{
		SAFE_RELEASE(m_pMeshs[i]->m_d3dpMesh);
		delete m_pMeshs[i];
	}

	if(m_pMeshs.size() != 0)
		m_pMeshs.clear();

	for(unsigned int i = 0; i < m_pTextures.size(); i++)
	{
		SAFE_RELEASE(m_pTextures[i]->m_d3dpTexture);
		delete m_pTextures[i];
	}
	if(m_pTextures.size() != 0)
		m_pTextures.clear();

	for(unsigned int i = 0; i < m_pEffects.size(); i++)
	{
		SAFE_RELEASE(m_pEffects[i]->m_d3dpEffect);
		delete m_pEffects[i];
	}
	if(m_pEffects.size() != 0)
		m_pEffects.clear();

	for(unsigned int i = 0; i < m_pModels.size(); i++)
	{
		SAFE_RELEASE(m_pModels[i]->m_d3dpIndexBuffer);
		SAFE_RELEASE(m_pModels[i]->m_d3dpVertBuffer);
		SAFE_RELEASE(m_pModels[i]->m_d3dpVertexDeclaration);
		delete m_pModels[i];
	}
	if(m_pModels.size() != 0)
		m_pModels.clear();

	for(unsigned int i = 0; i < m_pGeometry.size(); i++)
	{
		SAFE_RELEASE(m_pGeometry[i]->m_d3dpIndexBuffer);
		SAFE_RELEASE(m_pGeometry[i]->m_d3dpVertBuffer);
		SAFE_RELEASE(m_pGeometry[i]->m_d3dpVertexDeclaration);
		delete m_pGeometry[i];
	}
	if(m_pGeometry.size() != 0)
		m_pGeometry.clear();
	
	for(unsigned int i = 0; i < m_pMainMenuModels.size(); i++)
	{
		SAFE_RELEASE(m_pMainMenuModels[i]->m_d3dpIndexBuffer);
		SAFE_RELEASE(m_pMainMenuModels[i]->m_d3dpVertBuffer);
		SAFE_RELEASE(m_pMainMenuModels[i]->m_d3dpVertexDeclaration);
		delete m_pMainMenuModels[i];
	}
	if(m_pMainMenuModels.size() != 0)
		m_pMainMenuModels.clear();

	//for(unsigned int i = 0; i < m_pModelsSpecNorm.size(); i++)
	//{
	//	SAFE_RELEASE(m_pModelsSpecNorm[i]->m_d3dpIndexBuffer);
	//	SAFE_RELEASE(m_pModelsSpecNorm[i]->m_d3dpVertBuffer);
	//	SAFE_RELEASE(m_pModelsSpecNorm[i]->m_d3dpVertexDeclaration);
	//	delete m_pModelsSpecNorm[i];
	//}
	//
	//m_pModelsSpecNorm.clear();
}