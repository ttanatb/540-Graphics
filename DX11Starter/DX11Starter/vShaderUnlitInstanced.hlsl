cbuffer perFrameData : register(b0)
{
	matrix view;
	matrix projection;
};

cbuffer perObjData : register(b1)
{
	matrix world;
};

struct VertexShaderInput
{
	float3 position		: POSITION;     // XYZ position
	float4 color		: COLOR;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;
};


VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;
	output.position = mul(float4(input.position, 1.0f), mul(mul(world, view), projection));
	output.color = input.color;
	return output;
}