struct DirectionalLight
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

cbuffer lightData : register(b0)
{
	DirectionalLight directionalLight;
	DirectionalLight directionalLight2;
};
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float3 oppositeDir = normalize(directionalLight.direction);
	float3 oppositeDir2 = normalize(directionalLight2.direction);
	return directionalLight.ambientColor + directionalLight2.ambientColor +
		directionalLight.diffuseColor * saturate(dot(oppositeDir, input.normal)) +
		directionalLight2.diffuseColor * saturate(dot(oppositeDir2, input.normal));
	//return float4(1.0, 0.0, 0.0, 1.0);
}