struct DS_OUTPUT
{
	float4 vPosition	: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: WORLD_POS;
	float3 color		: COLOR;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 vPosition	: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: WORLD_POS;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	//matrix vp			: VIEW_PROJECTION;
	// TODO: change/add other stuff
};

cbuffer perFrameData : register(b0)
{
	matrix view;
	matrix projection;
};

cbuffer perObjData : register(b1)
{
	matrix world;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	Output.vPosition = 
			patch[0].vPosition * domain.x + 
			patch[1].vPosition * domain.y + 
			patch[2].vPosition * domain.z;

	Output.normal = 
		patch[0].normal * domain.x + 
		patch[1].normal * domain.y + 
		patch[2].normal * domain.z;

	Output.worldPos = 
		patch[0].worldPos * domain.x + 
		patch[1].worldPos * domain.y + 
		patch[2].worldPos * domain.z;

	float dotProduct = dot(Output.normal, float3(0, 1.0f, 0));
	bool mask = dotProduct >= 0.3f;
	dotProduct = dotProduct - 0.3f;

	Output.worldPos += mask * Output.normal * 1.5f * dotProduct; // float4(0, 1, 0, 0);
	Output.vPosition = mul(float4(Output.worldPos, 1.0f), mul(view, projection));
	Output.color = float3(0, 0, 0);
	Output.color += mask * float3(1, 1, 1) * dotProduct / 0.1f;

	Output.uv = 
		patch[0].uv * domain.x + 
		patch[1].uv * domain.y + 
		patch[2].uv * domain.z;

	return Output;
}
