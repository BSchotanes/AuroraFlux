#ifndef FINALPASS_FX_
#define FINALPASS_FX_
#include "Globals.fx"

FINAL_OUTPUT finalPassVertexShader(float3 pos : POSITION0, float2 texCoords : TEXCOORD0)
{
	FINAL_OUTPUT output;
	
	output.position = float4(pos, 1.0f);
	output.screenPos = float4(pos, 1.0f);
	output.texCoord = texCoords;
	
	return output;
}

float4 finalPassPixelShader(FINAL_OUTPUT input) : COLOR
{
	//if(gBufferToRender == 1)
	//{
	//
	//	float depth = (2.0 * 1.0) / (160000.0 + 1.0 - tex2D(depthGBufferSampler, input.texCoord).r * (160000.0 - 1.0));
	//
	//	return float4(depth, depth, depth, 1.0);
	//}
	//else if(gBufferToRender == 2)
	//	return tex2D(diffuseGBufferSampler, input.texCoord);
	//else if(gBufferToRender == 3)
	//	return tex2D(normalGBufferSampler, input.texCoord);
	//else if(gBufferToRender == 4)
	//{
	//	float edgeColor = tex2D(specularGBufferSampler, input.texCoord).g;
	//
	//	if(edgeColor == 0.2f)
	//		return Yellow_Rim_Color;
	//	else if(edgeColor == 0.4f)
	//		return Blue_Rim_Color;
	//	else if(edgeColor == 0.6f)
	//		return Green_Rim_Color;
	//	else if(edgeColor == 0.8f)
	//		return Red_Rim_Color;
	//	else if(edgeColor == 0.0f)
	//		return White_Rim_Color;
	//	return float4(0.0f,0.0f,0.0f,1.0f);
	//}
	//else if(gBufferToRender == 5)
	//else if(gBufferToRender == 6)
	//else 
	if(gBufferToRender == 9)
		return tex2D(sourceSampler,input.texCoord);
	else if(gBufferToRender == 11)
		return tex2D(sourceSampler,input.texCoord) + gGamma;

	return saturate(tex2D(lightBufferSampler,input.texCoord) + gGamma) ;
	
}

technique finalPassTechnique
{
    pass FirstPass
    {
        vertexShader = compile vs_3_0 finalPassVertexShader();
        pixelShader  = compile ps_3_0 finalPassPixelShader();
		cullMode = CCW;
    }
}
#endif