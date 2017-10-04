#ifndef MENU_FX_
#define MENU_FX_
#include "Globals.fx"

float gAlpha;

struct VS_OUTPUT
{
    float4 transformed_pos		: POSITION0;
	float2 uv					: TEXCOORD1;
};

VS_OUTPUT ScreenSpaceQuad(VS_INPUT input)
{
	VS_OUTPUT output;
	output.transformed_pos = float4(input.pos,1);
	output.uv = input.uv;
	return output;
}

float4 PassAlong(VS_OUTPUT input) : COLOR
{
	float4 tempColor = saturate(tex2D(diffuseSampler,input.uv) + gGamma);
	
	if(tempColor.w == 0.0f)
		discard;

	return tempColor;
}
float4 PassAlongAlpha(VS_OUTPUT input) :COLOR
{
	float4 outColor = tex2D(diffuseSampler,input.uv);

	if(outColor.w == 0.0f)
		discard;

	outColor.w *= gAlpha;
	return saturate(outColor+ gGamma);
}
technique myTechnique
{
	
   	pass FirstPass
    {
		AlphaBlendEnable=true;
		SrcBlend=srcalpha;
		DestBlend=invsrcalpha;
    	vertexShader = compile vs_3_0 ScreenSpaceQuad();
		pixelShader = compile ps_3_0 PassAlong();
    }
}
technique myTechniqueAlpha
{
   	pass FirstPass
    {
		AlphaBlendEnable=true;
		SrcBlend=srcalpha;
		DestBlend=invsrcalpha;
    	vertexShader = compile vs_3_0 ScreenSpaceQuad();
		pixelShader = compile ps_3_0 PassAlongAlpha();
    }
}
#endif