#ifndef GBUFFERS_FX_
#define GBUFFERS_FX_
#include "Globals.fx"

//	Vertex Shaders
GBUFFERS_OUTPUT gbuffersVertexShader(GENERAL_INPUT input)
{
	GBUFFERS_OUTPUT buffer;
	buffer.position = mul(float4(input.pos,1.0f),gMVP);
	
	buffer.normal = mul(float4(input.norm,0.0f),gWorld);
	buffer.normal = normalize(buffer.normal);
	
	buffer.texCoord = input.uv;
	buffer.depth = buffer.position.zw;
	
	buffer.tangent = mul(input.tangent,gWorld);
	buffer.tangent = normalize(buffer.tangent);

	buffer.binormal = mul(input.binormal,gWorld);
	buffer.binormal = normalize(buffer.binormal);

	return buffer;
}

GBUFFERS_OUTPUT SungbuffersVertexShader(GENERAL_INPUT input)
{
	GBUFFERS_OUTPUT buffer;
	buffer.position = mul(float4(input.pos, 1.0f),gMVP);
	
	buffer.normal = mul(float4(input.norm,0.0f),gWorld);
	buffer.normal = normalize(buffer.normal);
	
	buffer.texCoord = float2(input.uv.x,input.uv.y);
	buffer.depth = buffer.position.zw;
	
	buffer.tangent = mul(input.tangent,gWorld);
	buffer.tangent = normalize(buffer.tangent);

	buffer.binormal = mul(input.binormal,gWorld);
	buffer.binormal = normalize(buffer.binormal);

	return buffer;
}
//	Pixel Shaders

COLOR_OUTPUT gbuffersPixelShader(GBUFFERS_OUTPUT input)
{
	COLOR_OUTPUT output;

	output.depth = input.depth.x / input.depth.y;
	
	float4 normalMap = tex2D(normalSampler,input.texCoord);
	normalMap = ((normalMap)*2)-1;
	
	float4 normal = float4(input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal,0.0f);

	output.normal = ((normalize(normal)+1)/2);
	
	output.diffuse = tex2D(diffuseSampler,input.texCoord);

	if(output.diffuse.a <= 0.2f)
		discard;

	output.specular = 0;
	output.specular.r = tex2D(specularSampler,input.texCoord).r;
	output.specular.g = gEdgeColor;
	return output;
}

COLOR_OUTPUT generatorgbuffersPixelShader(GBUFFERS_OUTPUT input)
{
	COLOR_OUTPUT output;

	output.depth = input.depth.x / input.depth.y;
	
	float4 normalMap = tex2D(normalSampler,input.texCoord);
	normalMap = ((normalMap)*2)-1;
	
	float4 normal = float4(input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal,0.0f);
	
	output.normal = ((normalize(normal)+1)/2);
	
	output.diffuse = tex2D(diffuseSampler,input.texCoord);	
	if(output.diffuse.a <= 0.2f)
		discard;
		
	float4 FadeToRed = float4(output.diffuse.r / 1000.0f ,output.diffuse.g / 1000.0f ,output.diffuse.b / 1000.0f,output.diffuse.a);

	output.diffuse = saturate(lerp(FadeToRed,output.diffuse,gPercent));

	output.specular = 0;
	output.specular.r = tex2D(specularSampler,input.texCoord).r;
	output.specular.g = gEdgeColor;

	return output;
}

COLOR_OUTPUT gbuffersPixelShaderCollisionSphere(GBUFFERS_OUTPUT input)
{
	COLOR_OUTPUT output;
	output.depth = input.depth.x / input.depth.y;
	output.normal = gColor;
	
	output.diffuse = gColor;	
	
	output.specular = gColor;
	return output;
}

COLOR_OUTPUT playergbuffersPixelShader(GBUFFERS_OUTPUT input)
{
	COLOR_OUTPUT output;
	output.depth= input.depth.x / input.depth.y;
	
	float4 normalMap = tex2D(normalSampler,input.texCoord);
	normalMap = ((normalMap)*2)-1;
	float4 normal = float4(input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal,0.0f);

	output.normal = ((normalize(normal)+1)/2);

	output.diffuse = tex2D(diffuseSampler,input.texCoord);

	if( output.diffuse.r >= 0.639f && gTransitionValue <= 1.1f && gTransitionValue > 0.0f)
	{
		output.diffuse = lerp(float4(1.0f,1.0f,1.0f,1.0f),float4(0.0f,0.0f,1.0f,1.0f),gTransitionValue);
	}
	else if(output.diffuse.r >= 0.639f)
	{
		output.diffuse = lerp(float4(1.0f,1.0f,1.0f,1.0f),float4(1.0f,0.0f,0.0f,1.0f),-gTransitionValue);
	}

	output.specular = float4(tex2D(specularSampler,input.texCoord).r,gEdgeColor,0,0);
	
	return output;
}
COLOR_OUTPUT SungbuffersPixelShader(GBUFFERS_OUTPUT input)
{
	COLOR_OUTPUT output;
	output.depth= input.depth.x / input.depth.y;
	
	float4 normalMap = tex2D(normalSampler,input.texCoord);
	normalMap = ((normalMap)*2)-1;

	float4 normal = float4(input.normal + normalMap.x * input.tangent + normalMap.y * input.binormal,0.0f);

	output.normal = ((normalize(normal)+1)/2);

	output.diffuse = tex2D(diffuseSampler,input.texCoord);

	float fSin = sin( gTime * 2 ) * 0.285f + 0.15f;
	if(output.diffuse.g >= 0.339f )
	{
		output.diffuse = lerp(output.diffuse,float4(1.0f,1.0f,0.0f,1.0f),fSin);
	}
	else if( output.diffuse.r >= 0.339f)
	{
		output.diffuse = lerp(output.diffuse,float4(1.0f,0.270f,0.0f,1.0f),fSin);
	}

	output.specular = float4(tex2D(specularSampler,input.texCoord).r,gEdgeColor,0,0);
	
	return output;
}
//	Techniques
technique gbuffersTechnique
{
	pass FirstPass
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 gbuffersPixelShader();
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		cullMode = CCW;
	}
}

technique WireframegbuffersTechnique
{
	pass FirstPass
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 gbuffersPixelShader();
		ZEnable = TRUE;
		ZWriteEnable = TRUE;
		cullMode = CCW;
		FillMode = WIREFRAME;
	}
}

technique generatorgbuffersTechnique
{
	pass FirstPass
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 generatorgbuffersPixelShader();
		cullMode = CCW;
	}
}

technique gbuffersTechniqueCollisionSphere
{
	pass FirstPass
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 gbuffersPixelShaderCollisionSphere();
		cullMode = CCW;
	}
}

technique gbuffersTechniqueCollisionSphereWF
{
	pass FirstPass
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 gbuffersPixelShaderCollisionSphere();
		cullMode = CCW;
		FillMode = WIREFRAME;
	}
}

technique playergbuffersTechnique
{
	pass P0
	{
		vertexShader = compile vs_3_0 gbuffersVertexShader();
		pixelShader = compile ps_3_0 playergbuffersPixelShader();
		cullMode = CCW;
	}

}	
technique SungbuffersTechnique
{
	pass P0
	{
		vertexShader = compile vs_3_0 SungbuffersVertexShader();
		pixelShader = compile ps_3_0 SungbuffersPixelShader();
		cullMode = CCW;
	}

}
#endif