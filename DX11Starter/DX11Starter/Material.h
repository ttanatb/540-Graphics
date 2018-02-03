#pragma once
#include "Color.h"
#include "SimpleShader.h"

class Material {
private:
	//texture
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

public:
	Material();
	Material(SimpleVertexShader * vShader, SimplePixelShader* pShader);

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	void SetVertexShader(SimpleVertexShader* newVertexShader);
	void SetPixelShader(SimplePixelShader* newPixelShader);

	void PrepareMaterial(mat4* viewMat, mat4* projMat, mat4* worldMat);
};