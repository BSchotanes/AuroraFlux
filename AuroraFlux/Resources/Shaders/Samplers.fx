#ifndef SAMPLERS_FX_
#define SAMPLERS_FX_

shared texture  gDiffuseMap;

sampler diffuseSampler =
sampler_state
{
    Texture = <gDiffuseMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture  gDiffuseShieldMap;

sampler diffuseShieldSampler =
sampler_state
{
    Texture = <gDiffuseShieldMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture 	gSpecularMap;

sampler specularSampler =
sampler_state
{
    Texture = <gSpecularMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

shared texture 	gNormalMap;

sampler normalSampler =
sampler_state
{
    Texture = <gNormalMap>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};


shared texture 	gAmbientMap;

sampler ambientSampler = 
sampler_state
{
	Texture = <gAmbientMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


shared texture specularGBuffer;
sampler specularGBufferSampler = 
sampler_state
{
	Texture = <specularGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture normalGBuffer;
sampler normalGBufferSampler = 
sampler_state
{
	Texture = <normalGBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};


shared texture depthGBuffer;

sampler depthGBufferSampler =
sampler_state
{
	Texture = <depthGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture diffuseGBuffer;

sampler diffuseGBufferSampler =
sampler_state
{
	Texture = <diffuseGBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture lightBuffer;

sampler lightBufferSampler =
sampler_state
{
	Texture = <lightBuffer>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

//shared texture transparencyBuffer;
//
//sampler transparencyBufferSampler =
//sampler_state
//{
//	Texture = <transparencyBuffer>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//};

shared texture sourceTexture;
sampler sourceSampler = 
sampler_state
{
	Texture = <sourceTexture>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

//shared texture finalTexture;
//sampler finalSampler = 
//sampler_state
//{
//	Texture = <finalTexture>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//};

//shared texture fogTexture;
//sampler fogSampler = 
//sampler_state
//{
//	Texture = <fogTexture>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//};

texture reflectionMap;

sampler reflectionSampler = 
sampler_state
{
	Texture = <reflectionMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

//texture reflectionMap2;
//
//sampler reflectionSampler2 = 
//sampler_state
//{
//	Texture = <reflectionMap2>;
//	MinFilter = LINEAR;
//	MagFilter = LINEAR;
//	MipFilter = LINEAR;
//
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//	AddressW = CLAMP;
//};
#endif