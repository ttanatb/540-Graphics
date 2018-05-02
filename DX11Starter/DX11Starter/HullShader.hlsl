// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: WORLD_POS;
	//matrix vp			: VIEW_PROJA;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: WORLD_POS;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	//matrix vp						: VIEW_PROJECTION;
	// TODO: change/add other stuff
};



#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	int tessalationFactor = 1;
	
	VS_CONTROL_POINT_OUTPUT vs = ip[0];
	float dotProduct = dot(vs.normal, float3(0, 1.0f, 0));
	bool mask = dotProduct >= 0.f;
	tessalationFactor += mask * 2;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.InsideTessFactor = tessalationFactor; // e.g. could calculate dynamic tessellation factors instead
	
	//Output.vp = vs.vp;

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[patchconstantfunc("CalcHSPatchConstants")]
[outputcontrolpoints(3)]
[maxtessfactor(7.0)]

HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.position = ip[i].position;
	//Output.vp = ip[i].vp;
	Output.normal = ip[i].normal;
	Output.uv = ip[i].uv;
	Output.worldPos = ip[i].worldPos;

	return Output;
}
