#pragma once
#include "Color.h"
#include "SimpleShader.h"
#include "WICTextureLoader.h"

class Material {
private:
	//texture
	SimpleVertexShader* vertexShader = nullptr;
	SimplePixelShader* pixelShader = nullptr;
	ID3D11ShaderResourceView* srvPtr = nullptr;
	ID3D11SamplerState* samplerPtr = nullptr;
public:
	Material();
	Material(SimpleVertexShader * vShader,
		SimplePixelShader* pShader);
	Material(SimpleVertexShader * vShader,
		SimplePixelShader* pShader,
		ID3D11Device* device,
		ID3D11DeviceContext* context,
		const wchar_t* fileName);
	~Material();

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

	void SetVertexShader(SimpleVertexShader* newVertexShader);
	void SetPixelShader(SimplePixelShader* newPixelShader);

	void PrepMatTexture(mat4* worldMat);
	void PrepMat(mat4* worldMat);
};