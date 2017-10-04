float4x4 gMVP : WORLDVIEWPROJECTION;

struct VS_INPUT
{
    float3 untransformed_pos	: POSITION0;
};

struct VS_OUTPUT
{
    float4 transformed_pos	: POSITION0;
    float4 color_rgba 		: COLOR0;
};

VS_OUTPUT myVertexShader(VS_INPUT input)
{
	VS_OUTPUT output;
	output.transformed_pos = mul(float4(input.untransformed_pos, 1.0f), gMVP);
	output.color_rgba = float4(0.0f,0.0f,1.0f,1.0f);
	return output; 
}

float4 myPixelShader(float4 color : COLOR0) : COLOR
{
	return color;
}

technique myTechnique
{

    pass FirstPass
    {

        vertexShader = compile vs_3_0 myVertexShader();
        pixelShader  = compile ps_3_0 myPixelShader();

		FillMode = WIREFRAME;
        CullMode = NONE;
    }
}