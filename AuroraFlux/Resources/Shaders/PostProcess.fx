#ifndef POSTPROCESS_FX
#define POSTPROCESS_FX
#include "Globals.fx"

//float3 gFogColor = float3(0.42f, 0.42f, 0.42f);
//float fogNegX = -5000.0f;
//float fogPosX = 5000.0f;
//float fogPosZ = 5000.0f;
//float fogNegZ = -5000.0f;
//float gFogRate = 0.45f;
//float gFogDensity = 0.001f;

//float gNear;
//float gFar;

//int g_numSamples = 1;
//float4x4 gPrevViewProj;
//float4x4 gViewProj;

struct PostProcessVertexOut
{
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
};

//struct PostProcessBloomVertexOut
//{
//	float4 position : POSITION0;
//	float4 texCoords[samples/2] : TEXCOORD0;
//}
PostProcessVertexOut PostProcessVS(float3 position : POSITION0, float2 texCoord : TEXCOORD0)
{
	PostProcessVertexOut outVert;

	outVert.position = float4(position, 1);
	outVert.texCoord = texCoord;

	return outVert;
}

//PostProcessVertexOut PostProcessBloomVS(float3 position : POSITION0, float2 texCoord : TEXCOORD0)
//{
//	PostProcessVertexOut outVert;
//
//	outVert.position = float4(position, 1);
//	for(int i = 0 ; i < samples/2; i++)
//	{
//		output.texCoords[i].xy = input.texCoord.xy + sampleOffsets[i*2];
//		output.texCoords[i].zw = input.texCoord.xy + sampleOffsets[i*2+1];
//	}
//
//	return outVert;
//}
//float4 PostProcessFogColorPS(PostProcessVertexOut inVert) : COLOR0
//{		
//	
//	
//
//	float3 sceneColor = tex2D(lightBufferSampler, inVert.texCoord).rgb;
//	float3 finalColor = sceneColor;
//	//float3 gFogColor = tex2D(fogSampler,inVert.texCoord);
//
//	float depth = tex2D(depthGBufferSampler, inVert.texCoord).r;
//
//	//float linearDepth =  (2.0 * gNear) / (gFar + gNear - depth * (gFar - gNear));	
//
//	float4 position = float4(inVert.texCoord, depth, 1);
//
//	position.y = 1 - position.y;
//	
//	position.xy = 2 * position.xy - 1;
//
//	position = mul(position, gInvViewProj);
//
//	float3 worldPos = position.xyz / position.w;	
//
//	//float fogLevel = 0.0f;
//
//	float3 worldView = worldPos - gCameraPos;
//	
//	float  viewLength = length(worldView);
//
//	//float3 worldNorm = worldView / viewLength;
//	
//	//float camDepth = fogLevel - gCameraPos.y;
//
//	//float ratio = (fogLevel - gCameraPos.y) / worldNorm.y;
//	//float3 fogSurface = gCameraPos + worldNorm * ratio;
//
//	//camDepth = fogSurface.y - gCameraPos.y;
//	
//	//if(worldPos.y < fogSurface.y)
//	//{
//		float fogIntensity = saturate(gFogRate * viewLength);
//		
//		finalColor = fogIntensity * gFogColor + (1 - fogIntensity) * finalColor;
//	//}
//
//
//	//worldPos.x < (gCameraPos.x + 300.0f) && worldPos.x > (gCameraPos.x - 300.0f) &&
//	//	worldPos.y < (gCameraPos.y + 300.0f) && worldPos.y > (gCameraPos.y - 300.0f) &&
//	//if(worldPos.z < (gCameraPos.z + 300.0f) )
//	//{
//	//	return float4(sceneColor, 1);
//	//}
//
//	return float4(lerp(sceneColor,finalColor,depth),1.0f);
//}

//float4 PostProcessMotionBlurPS(PostProcessVertexOut inVert) : COLOR0
//{		
//	float depthVal = tex2D(depthGBufferSampler,inVert.texCoord).r;
//	float2 texCoord = inVert.texCoord;
//    float4 ScreenPos;
//    ScreenPos.x = inVert.texCoord.x * 2.0f - 1.0f;
//    ScreenPos.y = -(inVert.texCoord.y * 2.0f - 1.0f);
//    ScreenPos.z = depthVal;
//    ScreenPos.w = 1.0f;
//
//	float4 worldPos= mul(ScreenPos, gViewProj);
//	worldPos /= worldPos.w;
//
//	float4 currentPos = ScreenPos;  
//	
//	float4 previousPos = mul(worldPos, gPrevViewProj);  
//
//	previousPos /= previousPos.w;  
//
//	float2 velocity = (currentPos - previousPos)/2.0f; 
//
//	float4 outColor = tex2D(lightBufferSampler,texCoord);
//	texCoord += velocity;  
//
//	float4 currentColor = tex2D(lightBufferSampler, texCoord);  
//
//	outColor += currentColor;  
//	
//	texCoord += velocity;  
//	
//
//	return outColor / 2;
//}

float4 PostProcessGreyScalePS(PostProcessVertexOut inVert) :COLOR0
{
	float3 sceneColor = tex2D(lightBufferSampler, inVert.texCoord).rgb;

	float4 outColor = ( (sceneColor.r+sceneColor.g+sceneColor.b) / 3.0f );
	outColor.a = 1.0f;

	return outColor;
}

//float4 PostProcessBloomPS(PostProcessVertexOut inVert) :COLOR0
//{
//	float4 outColor = 0;
//
//	for(int i = 0 ; i < samples/2; i++)
//	{
//		outColor.rgb += sampleWeights[i*2] *tex2D(sourceSampler, inVert.texCoords[i].xy);
//		outColor.rgb += sampleWeights[i*2+1] *tex2D(sourceSampler, inVert.texCoords[i].zw);
//	}
//
//	return outColor;
//}
//technique PostProcessFog
//{
//	pass Pass0
//	{
//		VertexShader = compile vs_3_0 PostProcessVS();
//		PixelShader	 = compile ps_3_0 PostProcessFogColorPS();
//	}
//}
//
//technique PostProcessMotionBlur
//{
//	pass Pass0
//	{
//		VertexShader = compile vs_3_0 PostProcessVS();
//		PixelShader	 = compile ps_3_0 PostProcessMotionBlurPS();
//	}
//}

technique PostProcessGreyScale
{
	pass Pass0
	{
		VertexShader = compile vs_3_0 PostProcessVS();
		PixelShader  = compile ps_3_0 PostProcessGreyScalePS();
	}
}
//technique PostProcessBloom
//{
//	pass Pass0
//	{
//		VertexShader = compile vs_3_0 PostProcessBloomVS();
//		PixelShader  = compile ps_3_0 PostProcessBloomPS();
//	}
//}
#endif