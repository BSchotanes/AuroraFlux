#ifndef TRANSPARENCY_FX_
#define TRANSPARENCY_FX_
#include "Globals.fx"

//	Vertex Shaders

TRANSPARENCY_OUTPUT TransparencyVertexShader(GENERAL_INPUT input)
{
	TRANSPARENCY_OUTPUT buffer;
	buffer.position = mul(float4(input.pos,1.0f),gMVP);
	
	buffer.texCoord = input.uv;
	buffer.depth = buffer.position.zw;

	return buffer;
}

TRANSPARENCY_OUTPUT BossShieldTransparencyVertexShader(GENERAL_INPUT input)
{
	TRANSPARENCY_OUTPUT buffer;
	buffer.position = float4(input.pos,1.0f);
	
	if(gIsHit)
		buffer.position.xz *= (1+sin(gTime*16+buffer.position.y*48)/96);
		
	buffer.position = mul(buffer.position,gMVP);
	
	buffer.texCoord = float2(input.uv.x,input.uv.y-(gTime/2));
	buffer.depth = buffer.position.zw;

	return buffer;
}

TRANSPARENCY_OUTPUT ScrollingUVTransparencyVertexShader(GENERAL_INPUT input)
{
	TRANSPARENCY_OUTPUT buffer;
	buffer.position = mul(float4(input.pos,1.0f),gMVP);
	
	buffer.texCoord = float2(input.uv.x + sin(gTime),input.uv.y - gTime);
	buffer.depth = buffer.position.zw;

	return buffer;
}

TRANSPARENCY_OUTPUT BossLaserScrollingUVTransparencyVertexShader(GENERAL_INPUT input)
{
	TRANSPARENCY_OUTPUT buffer;
	buffer.position = mul(float4(input.pos,1.0f),gMVP);

	buffer.texCoord = float2(input.uv.x,input.uv.y - gTime);
	buffer.depth = buffer.position.zw;

	return buffer;
}
TRANSPARENCY_OUTPUT BossLaserScrollingUVTransparencySmallerVertexShader(GENERAL_INPUT input)
{
	TRANSPARENCY_OUTPUT buffer;
	buffer.position = mul(float4((input.pos*0.6f),1.0f),gMVP);

	buffer.texCoord = float2(input.uv.x,input.uv.y - gTime);
	buffer.depth = buffer.position.zw;

	return buffer;
}
//	Pixel Shaders

float4 StencilFS(GBUFFERS_OUTPUT  inVert) : COLOR0 
{
	return float4(1, 0, 0, 1);
}

float4 TransparentFadeObjectPixelShader(TRANSPARENCY_OUTPUT input) : COLOR0
{
	float4 output;

	output.rgb = tex2D(diffuseSampler,input.texCoord).rgb;
	output.a = gTransitionValue;
	
	return output;

}

float4 TransparentObjectPixelShader(TRANSPARENCY_OUTPUT input) : COLOR0
{
	float4 output;

	output = tex2D(diffuseSampler,input.texCoord);

	output = lerp(float4(output.rgb,0.60f),float4(output.rgb,1.0f),gPercent);
	
	return output;

}
float4 BossShieldTransparencyPixelShader(TRANSPARENCY_OUTPUT input) : COLOR
{
	float4 output;

	output = tex2D(diffuseSampler,input.texCoord);

	output.a *= gPercent;

	if(output.a <= 0.420f)
		output = float4(0.8f,0.65f,0.0f,gPercent);

	return output;
}

float4 EnemyShieldTransparencyPixelShader(TRANSPARENCY_OUTPUT input) : COLOR
{
	float4 output;
	
	output = tex2D(diffuseSampler,input.texCoord);

	if(output.a <= 0.420f)
		output = float4(0.8f,0.0f,0.0f,gPercent);
	else
		output.a = gPercent;

	if(output.a <= 0.149)
		discard;

	return output;
}

float4 ParticleTransparencyPixelShader(TRANSPARENCY_OUTPUT input) : COLOR
{
	float4 output;

	float4 tempColor = tex2D(diffuseSampler,input.texCoord);
			
	if(tempColor.a < 0.1f)
		discard;
		
	tempColor = saturate(tempColor*gColor/2);
	
	if(tempColor.a < 0.1f)
		discard;

	output = tempColor;
	
	return output;
}

float4 TrasparentTransparencyPixelShader(TRANSPARENCY_OUTPUT input) : COLOR
{
	float4 output;
	
	output = tex2D(diffuseSampler,input.texCoord);	

	output.r = 1.0f;
	output.g = 0.0f;
	output.b = 0.0f;

	if(output.a <= 0.2f)
		discard;

	return output;
}

float4 ShieldTransparencyPixelShader(TRANSPARENCY_OUTPUT input) : COLOR
{
	
 	if(gPercent >= 1.0f)
		discard;

	float4 output;

	float4 tempShield = tex2D( diffuseShieldSampler, input.texCoord);

	if(tempShield.a < 0.01f)
		discard;

	tempShield.a = -(gPercent - 1);
	
	output = tempShield;	
	
	if(gTransitionValue < 0.0f && !gIsHit)
		discard;

	float4 tempAlpha = 0.6f + ( ( ( (int) gTime ) % 10 + 1 ) / 40 );

	output.a *= tempAlpha;

	if(gIsHit)
	{
		
		output.r = 1.0f;
		output.g = 1.0f;
		output.b = 0.0f;
		return output;
	}

	return output;
}

float4 BossLaserPixelShader(TRANSPARENCY_OUTPUT input) : COLOR0
{
	float4 output;

	float4 tempColor = tex2D(diffuseSampler,input.texCoord);
	output = tempColor;
	
	output.a = 0.6f;
	return output;

}
float4 BossLaserWhitePixelShader(TRANSPARENCY_OUTPUT input) : COLOR0
{
	float4 output = 0;

	float4 tempColor = tex2D(diffuseSampler,input.texCoord);
	
	output.rgb = float3(tempColor.r,tempColor.r,tempColor.r);
	
	output.a = 0.8f;
	return output;

}

//	Techniques
technique TransparentObjectTechnique
{
	pass P1
    {
		AlphaBlendEnable = TRUE; 
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
		
        vertexShader = compile vs_3_0 TransparencyVertexShader();
        pixelShader  = compile ps_3_0 TransparentObjectPixelShader();
    }
}

technique FadeTransparentObjectTechnique
{
	pass P1
    {
		AlphaBlendEnable = TRUE; 
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
		FillMode = WIREFRAME;

        vertexShader = compile vs_3_0 TransparencyVertexShader();
        pixelShader  = compile ps_3_0 TransparentFadeObjectPixelShader();
    }
}
technique EnemyShieldTechnique
{
	pass P0
    {
		AlphaBlendEnable = TRUE; 
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
		
        vertexShader = compile vs_3_0 BossShieldTransparencyVertexShader();
        pixelShader  = compile ps_3_0 EnemyShieldTransparencyPixelShader();
    }
}

technique BossShieldTechnique
{
	pass P0
    {
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
		
        vertexShader = compile vs_3_0 BossShieldTransparencyVertexShader();
        pixelShader  = compile ps_3_0 BossShieldTransparencyPixelShader();
    }

}

technique BossTurretShieldTechnique
{
	pass P0
    {
		AlphaBlendEnable = TRUE;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
		
        vertexShader = compile vs_3_0 BossShieldTransparencyVertexShader();
        pixelShader  = compile ps_3_0 EnemyShieldTransparencyPixelShader();
    }
}

technique ParticleTechnique
{
	pass P0
	{
		AlphaBlendEnable=true;
		SrcBlend=gSourceBlend;
		DestBlend=gDestinationBlend;
		CullMode = NONE;
		
		vertexShader = compile vs_3_0 TransparencyVertexShader();
        pixelShader  = compile ps_3_0 ParticleTransparencyPixelShader();
	}
}

technique ShieldTechnique
{
	pass P0
	{
		AlphaBlendEnable=true;
 		SrcBlend = SrcAlpha; 
  		DestBlend = InvSrcAlpha;
		cullMode = CCW;

		vertexShader = compile vs_3_0 ScrollingUVTransparencyVertexShader();
		pixelShader = compile ps_3_0 ShieldTransparencyPixelShader();
		
	}
}

technique BossLaserTechnique
{
	
	pass P0
    {
		AlphaBlendEnable = TRUE; 
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CW;
        
		vertexShader = compile vs_3_0 BossLaserScrollingUVTransparencyVertexShader();
        pixelShader  = compile ps_3_0 BossLaserPixelShader();
    }
	pass P1
    {
		AlphaBlendEnable = TRUE; 
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CullMode = CCW;
        
		vertexShader = compile vs_3_0 BossLaserScrollingUVTransparencySmallerVertexShader();
        pixelShader  = compile ps_3_0 BossLaserWhitePixelShader();
    }
}
#endif