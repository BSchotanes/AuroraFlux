#ifndef SKY_BOX_FX
#define SKY_BOX_FX
#include "globals.fx"

CubeVertOut SkyBoxVS(VS_INPUT input)
{
	CubeVertOut outVert;
	outVert.texCoord = float3(input.pos.x, input.pos.y, input.pos.z);
	float3 pos = input.pos.xyz + gCameraPos;
	outVert.position = mul(float4(pos, 1.0), gMVP);
	outVert.position.z = outVert.position.w;

	return outVert;
}

float4 SkyBoxPS(CubeVertOut inVert) : COLOR 
{
	return texCUBE(reflectionSampler, inVert.texCoord);
}

//float4 StarBoxPS(CubeVertOut inVert) : COLOR 
//{
//	float4 color = texCUBE(reflectionSampler2, inVert.texCoord);
//	//color.r = sin(gTime);
//	if(color.a <= 0.9f)
//		discard;
//
//	color.a = 0.75f;
//
//	return lerp(color,float4(1.0f,1.0f,1.0f,1.0f),sin(gTime));
//}
technique Basic
{
	pass P0
	{
		CullMode = CW;
		ZWriteEnable = FALSE;

		vertexShader = compile vs_3_0 SkyBoxVS();
		pixelShader  = compile ps_3_0 SkyBoxPS();
	}
	//pass P1
	//{
	//	AlphaBlendEnable = TRUE;
	//	SrcBlend = SRCALPHA;
	//	DestBlend = INVSRCALPHA;
	//	CullMode = CW;
	//	
	//	ZWriteEnable = FALSE;
	//
	//	vertexShader = compile vs_3_0 SkyBoxVS();
	//	pixelShader  = compile ps_3_0 StarBoxPS();
	//}
}

#endif