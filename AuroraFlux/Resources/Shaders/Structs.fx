#ifndef STRUCTS_FX_
#define STRUCTS_FX_

struct GENERAL_INPUT
{
    float3 pos			: POSITION0;
	float3 norm			: NORMAL0;
	float3 tangent		: TANGENT0;
	float3 binormal		: BINORMAL;
	float2 uv			: TEXCOORD0;
};

struct COLOR_OUTPUT
{
	float4 diffuse 		: COLOR0;
	float4 normal 		: COLOR1;
	float4 specular 	: COLOR2;
	float4 depth		: COLOR3;
};

struct GBUFFERS_OUTPUT
{
	float4 position		: POSITION0;
	float4 normal		: TEXCOORD0;
	float3 binormal		: BINORMAL0;
	float3 tangent		: TANGENT0;
	float2 texCoord		: TEXCOORD1;
	float2 depth		: TEXCOORD2;
};

struct TRANSPARENCY_OUTPUT
{
	float4 position		: POSITION0;
	float2 texCoord		: TEXCOORD0;
	float2 depth		: TEXCOORD1;
};

struct VertexShaderOutput
{
    float4 Position		: POSITION0;
    float2 TexCoord		: TEXCOORD0;
    
};

struct PointLightVertexOut
{
	float4 position		: POSITION0;
	float4 screenPos	: TEXCOORD0;
};

struct FINAL_OUTPUT
{
	float4 position		: POSITION0;
	float2 texCoord		: TEXCOORD1;
	float4 screenPos	: TEXCOORD2;
};

struct VS_INPUT
{
    float3 pos			: POSITION0;
	float2 uv			: TEXCOORD0;
};

struct CubeVertOut
{
	float4 position		 :POSITION0;
	float3 texCoord		 :TEXCOORD0;
};

struct PointLight
{
    bool		Enabled;
	float4x4	ViewProjection[6];

    float4		Position;
    float4		DiffuseColor;
    float4		AmbientColor;
	float4		Attenuation;
	float		Range;
    float4		SpecularColor;
    float		SpecularPower;
    float		SpecularIntensity;
};

shared PointLight PointLight0;

#endif