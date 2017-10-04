#include "EffectInformation.h"

#include "../Renderer/CAssetManager.h"
#include "../Object Manager/ObjectManager.h"

vector<tEffectSystemCreationInfo> EffectInformation::vtEffectInfo;
CObjectManager * EffectInformation::m_pObjectManager;

bool EffectInformation::LoadAllEffects( char* _cpFilePath, CAssetManager* _pAssetManager, CObjectManager* _pObjMag )
{
	m_pObjectManager = _pObjMag;
	//vtEffectInfo.clear();
	EffectInformation::resetEffects();
	vtEffectInfo.resize(eMAX_EFFECTS);

	string laserEffect = "Laser_Spark.txt";

	string szTempFilePath = "Resources/Particles/";
	szTempFilePath += _cpFilePath;

	fstream fsMasterScriptIn(szTempFilePath, std::ios_base::in);



	if(!fsMasterScriptIn)
	{
#if _DEBUG
		cout << szTempFilePath.c_str() << " Script did not load properly!" << '\n';
#endif

		return false;
	}

	string currEffect;
	int totalEffectsLoaded = 0;

	if(fsMasterScriptIn.is_open())
	{
		while(!fsMasterScriptIn.eof())
		{
			if(fsMasterScriptIn.is_open())
			{
				currEffect.clear();
				fsMasterScriptIn >> currEffect;
				++totalEffectsLoaded;

				if(currEffect.find(".txt") == string::npos)
				{
					cout << "Effect Error! not a valid filename" << currEffect.c_str() << endl;
					continue;
				}


				//Added for WU
				if(totalEffectsLoaded > eMAX_EFFECTS)
				{
					cout << currEffect.c_str() << " Effect Error! to many Effects! Did you add your effect to the Enum?" << endl;
					break;
				}


				bool isNew = true;
				unsigned int uiEffectSize = vtEffectInfo.size();
				unsigned int effectIndex = 0;
				for(; effectIndex <  uiEffectSize; ++effectIndex)
				{
					if(currEffect.compare(EffectNames[effectIndex]) == 0)
					{
						if(strcmp(vtEffectInfo[effectIndex].m_nEffectName.c_str(),"") == 0)
						{
							break;
						}
						else
						{
							isNew = false;
							break;
						}
					}
				}

				if(!isNew)
				{
					continue;
				}


				string szTempFilePath = "Resources/Particles/";
				szTempFilePath += currEffect;

				fstream fsScriptIn(szTempFilePath, std::ios_base::in);


				if(fsScriptIn.is_open())
				{
					

					//			if(fsScriptIn == NULL)
					//			{
					//#if _DEBUG
					//			cout << "Script did not load properly!" << '\n';
					//#endif
					//
					//			return false;
					//			}


					//Number of effects
					int nNumberofEffects = 0;

					fsScriptIn >> nNumberofEffects;

					for(int nEffectIndex = 0; nEffectIndex < nNumberofEffects; ++nEffectIndex)
					{
						if(!fsScriptIn.eof())
						{
							tEffectSystemCreationInfo tESCI;
							int nNumberofParticleSystems = 0;

							//Emitter Lifetime
							fsScriptIn >> tESCI.m_fLifetime;

							//Number of Particle Systems
							fsScriptIn >> nNumberofParticleSystems;

							tESCI.m_nNumberOfParticleSystems = nNumberofParticleSystems;

							tESCI.m_vParticleGeneralInfo.resize(nNumberofParticleSystems);
							tESCI.m_vParticleColorInfo.resize(nNumberofParticleSystems);
							tESCI.m_vParticleRotationInfo.resize(nNumberofParticleSystems);
							tESCI.m_vParticleScaleInfo.resize(nNumberofParticleSystems);
							tESCI.m_vParticleVelocityInfo.resize(nNumberofParticleSystems);

							string sLoadVariable;

							for(int nParticleIndex = 0; nParticleIndex < nNumberofParticleSystems; ++nParticleIndex)
							{

								//Frame offset
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_fFrameOffset;

								//Lifetime
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_fLifetime;

								//Face Player
								fsScriptIn >> (bool)tESCI.m_vParticleGeneralInfo[nParticleIndex].m_bFacePlayer;

								if(currEffect == laserEffect)
									tESCI.m_vParticleGeneralInfo[nParticleIndex].m_bNoRotation = true;

								//Scale
								int nTotalScale = 0;
								fsScriptIn >> nTotalScale;

								tESCI.m_vParticleScaleInfo[nParticleIndex].m_d3dValues.resize(nTotalScale);
								tESCI.m_vParticleScaleInfo[nParticleIndex].m_fTime.resize(nTotalScale);
								for(int nScaleIndex = 0; nScaleIndex < nTotalScale; ++nScaleIndex)
								{
									fsScriptIn >> tESCI.m_vParticleScaleInfo[nParticleIndex].m_d3dValues[nScaleIndex].x \
										>>  tESCI.m_vParticleScaleInfo[nParticleIndex].m_d3dValues[nScaleIndex].y \
										>> tESCI.m_vParticleScaleInfo[nParticleIndex].m_fTime[nScaleIndex];
								}



								//Rotation
								int nTotalRotation = 0;
								fsScriptIn >> nTotalRotation;

								tESCI.m_vParticleRotationInfo[nParticleIndex].m_fTime.resize(nTotalRotation);
								tESCI.m_vParticleRotationInfo[nParticleIndex].m_fValues.resize(nTotalRotation);
								for(int nRotationIndex = 0; nRotationIndex < nTotalRotation; ++nRotationIndex)
								{
									fsScriptIn >> tESCI.m_vParticleRotationInfo[nParticleIndex].m_fValues[nRotationIndex] 
									>> tESCI.m_vParticleRotationInfo[nParticleIndex].m_fTime[nRotationIndex];
								}


								//Velocity
								int nTotalVelocity = 0;
								fsScriptIn >> nTotalVelocity;

								tESCI.m_vParticleVelocityInfo[nParticleIndex].m_d3dValues.resize(nTotalVelocity);
								tESCI.m_vParticleVelocityInfo[nParticleIndex].m_fTime.resize(nTotalVelocity);
								for(int nVelocityIndex = 0; nVelocityIndex < nTotalVelocity; ++nVelocityIndex)
								{
									fsScriptIn >> tESCI.m_vParticleVelocityInfo[nParticleIndex].m_d3dValues[nVelocityIndex].x 
										>> tESCI.m_vParticleVelocityInfo[nParticleIndex].m_d3dValues[nVelocityIndex].y 
										>> tESCI.m_vParticleVelocityInfo[nParticleIndex].m_d3dValues[nVelocityIndex].z;

									fsScriptIn >> tESCI.m_vParticleVelocityInfo[nParticleIndex].m_fTime[nVelocityIndex];
								}


								//Color
								int nTotalColor = 0;
								fsScriptIn >> nTotalColor;

								tESCI.m_vParticleColorInfo[nParticleIndex].m_fTime.resize(nTotalColor);
								tESCI.m_vParticleColorInfo[nParticleIndex].m_uiValues.resize(nTotalColor);
								for(int nColorIndex = 0; nColorIndex < nTotalColor; ++nColorIndex)
								{
									fsScriptIn >> tESCI.m_vParticleColorInfo[nParticleIndex].m_uiValues[nColorIndex] 
									>> tESCI.m_vParticleColorInfo[nParticleIndex].m_fTime[nColorIndex];
								}


								//Gravity
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_fGravity;


								//position offset
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_d3dPositionOffset.x 
									>> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_d3dPositionOffset.y 
									>> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_d3dPositionOffset.z;

								//Source Blend Mode
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_uiSourceBlend;

								//Destination Blend Mode
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_uiDestinationBlend;

								//number of particles
								fsScriptIn >>  tESCI.m_vParticleGeneralInfo[nParticleIndex].m_nNumberOfParticles;

								//Emitter Rate
								fsScriptIn >> tESCI.m_vParticleGeneralInfo[nParticleIndex].m_fEmitterRate;

								//texture ID
								sLoadVariable.clear();
								fsScriptIn >> sLoadVariable;
								tESCI.m_vParticleGeneralInfo[nParticleIndex].m_nTextureID = _pAssetManager->LoadTexture(sLoadVariable.c_str());
							}

							//Geometry Code
							int nNumberofGeometrySystems = 0;
							fsScriptIn >> nNumberofGeometrySystems;

							tESCI.m_nNumberOfGeometrySystems = nNumberofGeometrySystems;

							tESCI.m_vGeometryGeneralInfo.resize(nNumberofGeometrySystems);
							tESCI.m_vGeometryColorInfo.resize(nNumberofGeometrySystems);
							tESCI.m_vGeometryRotationInfo.resize(nNumberofGeometrySystems);
							tESCI.m_vGeometryScaleInfo.resize(nNumberofGeometrySystems);
							tESCI.m_vGeometryVelocityInfo.resize(nNumberofGeometrySystems);

							for(int nGeometryIndex = 0; nGeometryIndex < nNumberofGeometrySystems; ++nGeometryIndex)
							{


								//Time offset
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_fTimeOffset;

								//Lifetime
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_fLifetime;

								//Scale
								int nTotalScale = 0;
								fsScriptIn >> nTotalScale;

								tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_d3dValues.resize(nTotalScale);
								tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_fTime.resize(nTotalScale);
								for(int nScaleIndex = 0; nScaleIndex < nTotalScale; ++nScaleIndex)
								{
									fsScriptIn >> tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_d3dValues[nScaleIndex].x 
										>> tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_d3dValues[nScaleIndex].y
										>> tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_d3dValues[nScaleIndex].z
										>> tESCI.m_vGeometryScaleInfo[nGeometryIndex].m_fTime[nScaleIndex];
								}


								//Rotation
								int nTotalRotation = 0;
								fsScriptIn >> nTotalRotation;

								tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_fTime.resize(nTotalRotation);
								tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_d3dValues.resize(nTotalRotation);
								for(int nRotationIndex = 0; nRotationIndex < nTotalRotation; ++nRotationIndex)
								{
									fsScriptIn >> tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_d3dValues[nRotationIndex].x 
										>> tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_d3dValues[nRotationIndex].y
										>> tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_d3dValues[nRotationIndex].z
										>> tESCI.m_vGeometryRotationInfo[nGeometryIndex].m_fTime[nRotationIndex];
								}


								//Velocity
								int nTotalVelocity = 0;
								fsScriptIn >> nTotalVelocity;

								tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_d3dValues.resize(nTotalVelocity);
								tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_fTime.resize(nTotalVelocity);
								for(int nVelocityIndex = 0; nVelocityIndex < nTotalVelocity; ++nVelocityIndex)
								{
									fsScriptIn >> tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_d3dValues[nVelocityIndex].x 
										>> tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_d3dValues[nVelocityIndex].y 
										>> tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_d3dValues[nVelocityIndex].z;

									fsScriptIn >> tESCI.m_vGeometryVelocityInfo[nGeometryIndex].m_fTime[nVelocityIndex];
								}


								//Color
								int nTotalColor = 0;
								fsScriptIn >> nTotalColor;

								tESCI.m_vGeometryColorInfo[nGeometryIndex].m_fTime.resize(nTotalColor);
								tESCI.m_vGeometryColorInfo[nGeometryIndex].m_uiValues.resize(nTotalColor);
								for(int nColorIndex = 0; nColorIndex < nTotalColor; ++nColorIndex)
								{
									fsScriptIn >> tESCI.m_vGeometryColorInfo[nGeometryIndex].m_uiValues[nColorIndex] 
									>> tESCI.m_vGeometryColorInfo[nGeometryIndex].m_fTime[nColorIndex];
								}


								//Gravity
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_fGravity;

								//position offset
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_d3dPositionOffset.x 
									>> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_d3dPositionOffset.y 
									>> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_d3dPositionOffset.z;

								//Source Blend Mode
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_uiSourceBlend;

								//Destination Blend Mode
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_uiDestinationBlend;

								//number of Geometry
								fsScriptIn >>  tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_uiNumberOfGeometry;

								//Emitter Rate
								fsScriptIn >> tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_fEmitterRate;

								//texture ID
								sLoadVariable.clear();
								fsScriptIn >> sLoadVariable;
								tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_nTextureID = _pAssetManager->LoadTexture(sLoadVariable.c_str());

								//Mesh ID
								sLoadVariable.clear();
								fsScriptIn >> sLoadVariable;
								tESCI.m_vGeometryGeneralInfo[nGeometryIndex].m_nMeshID = _pAssetManager->LoadGeometry(sLoadVariable.c_str());
							}

							tESCI.m_nEffectName = currEffect;
							if( effectIndex < vtEffectInfo.size() )
							{
								vtEffectInfo[effectIndex] = tESCI;
							}
						}
					}
					fsScriptIn.close();
				}
				else
				{
#if _DEBUG
				cout << "Script did not load properly!" << '\n';
#endif
				return false;
				}
			}
		}
	}
	fsMasterScriptIn.close();
	return true;


}


bool EffectInformation::CreateEffect(Effects _eType, CEffectSystem* _pEffect, CEntity* pOwner, bool _bRepeatable, bool _bPlay ) 
{
	if(_eType < 0 || _eType > eMAX_EFFECTS)
		return false;

	_pEffect->SetOwner(pOwner);
	_pEffect->CreateEffect(&vtEffectInfo[_eType], _bRepeatable, _bPlay);
	_pEffect->SetEffectType(_eType);

	m_pObjectManager->AddEffect(_pEffect);
	return true;
}

void EffectInformation::resetEffects()
{
	vtEffectInfo.clear();

}