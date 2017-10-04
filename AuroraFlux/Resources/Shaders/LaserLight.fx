#ifndef LASER_LIGHT_FX
#define LASER_LIGHT_FX
#include "Globals.fx"

float fTime;

float4 StencilFS(PointLightVertexOut inVert) : COLOR0 
{
	return float4(1, 0, 0, 1);
}

PointLightVertexOut PointLightVS(float3 position : POSITION0)
{
	PointLightVertexOut buffer;

	buffer.position = mul(float4(position.xyz,1.0f),gMVP);
	buffer.position.z = min(buffer.position.z, buffer.position.w);
	buffer.screenPos = buffer.position;
	return buffer;
}

float4 PointLightLaserFS(PointLightVertexOut inVert) : COLOR
{
	float2 clipPos= float2(inVert.screenPos.xy/inVert.screenPos.w);
	
	float2 texCoord = float2(clipPos.x,-clipPos.y)*0.5f+0.5f;
	texCoord += gHalfPixel;

	float3 normal = tex2D(normalGBufferSampler,texCoord).xyz;
	normal = (normal*2)-1;

	float depth = tex2D(depthGBufferSampler,texCoord).r;
	float4 fragPos = float4(clipPos.xy,depth,1.0f);
	
	float4 diffuseColor = tex2D(diffuseGBufferSampler,texCoord);

	float4 worldSpacePos = mul(fragPos,gInvViewProj);
	worldSpacePos /= worldSpacePos.w;

	float3 toLight = PointLight0.Position.xyz - worldSpacePos.xyz;
	
	float toLightLength = length(toLight);

	toLight /= toLightLength;

	float nDotL = saturate(dot(toLight,normal.xyz));

	float4 finalDiffuse = 0;

	if(gTime != 0.0f)
	{
		finalDiffuse = nDotL * PointLight0.DiffuseColor * diffuseColor;
		
	}
	else
	{
		float4 tempDiffuse = float4(1.0f,1.0f,1.0f,1.0f);
	
		tempDiffuse = lerp(PointLight0.DiffuseColor,float4(0.0f,0.0f,0.0f,1.0f),fTime);

		finalDiffuse = nDotL * tempDiffuse * diffuseColor;
	}

	float attenuation; 
	
	attenuation = saturate(1.0f / (PointLight0.Attenuation.x + PointLight0.Attenuation.y * 
		toLightLength + PointLight0.Attenuation.z * (toLightLength * toLightLength)));
	
	float alpha = toLightLength / PointLight0.Range;
	float damping = saturate( 1.0f - alpha * alpha);
	attenuation *= damping;
	
	float4 finalAmbient = PointLight0.AmbientColor * diffuseColor * diffuseColor.a;
	
	float specColor = tex2D(specularGBufferSampler,texCoord).r;
	float4 finalSpecular = float4(specColor,specColor,specColor,1.0f);
	
	float3 reflectionVector = reflect(toLight,normal);
	reflectionVector = normalize(reflectionVector);
	
	float3 directionToCamera = gCameraPos - worldSpacePos;
	directionToCamera = normalize(directionToCamera);
	
	float specMod = saturate(PointLight0.SpecularIntensity*pow(saturate(dot(reflectionVector, directionToCamera)),PointLight0.SpecularPower));
	
	finalSpecular *= specMod;

	return saturate(attenuation * (finalAmbient + finalDiffuse + finalSpecular));
}
technique RenderPointLight1 //inside
{
	pass P0
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 StencilFS();

		ZWriteEnable = FALSE;
		ZEnable = TRUE;
		ZFunc = GREATER;

		COLORWRITEENABLE = 0;
		AlphaBlendEnable = FALSE;

		CullMode = CW;
		StencilEnable = TRUE;
		StencilPass = REPLACE;
		StencilRef = 0x1;
	}
	pass P1
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 PointLightLaserFS();
        
		CullMode = CW;
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;

		COLORWRITEENABLE = 0x0000000F;

		StencilEnable = TRUE;
		StencilFunc = EQUAL;
		StencilPass = KEEP;
		StencilRef = 0x1;
    }
}
technique RenderPointLight2
{
	pass P0
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 StencilFS();

		ZWriteEnable = FALSE;
		ZEnable = TRUE;
		ZFunc = GREATER;

		COLORWRITEENABLE = 0;
		AlphaBlendEnable = FALSE;

		CullMode = CW;
		StencilEnable = TRUE;
		StencilPass = REPLACE;
		StencilRef = 0x1;
	}
	pass P1
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 StencilFS();

		ZWriteEnable = FALSE;
		ZEnable = TRUE;
		ZFunc = LESSEQUAL;
		COLORWRITEENABLE = 0;
		AlphaBlendEnable = FALSE;

		CullMode = CCW;
		StencilEnable = TRUE;
		StencilPass = INCR;
		StencilFunc = EQUAL;
		StencilRef = 0x1;
	}
	pass P2
	{
		vertexShader = compile vs_3_0 PointLightVS();
        pixelShader  = compile ps_3_0 PointLightLaserFS();
        
		CullMode = CCW;
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;

		COLORWRITEENABLE = 0x0000000F;

		StencilEnable = TRUE;
		StencilFunc = EQUAL;
		StencilPass = KEEP;
		StencilRef = 0x2;
    }
}
#endif