#include "Material.h"

Material::Material(SimpleVertexShader * vShader, SimplePixelShader * pShader)
{
	vertexShader = vShader;
	pixelShader = pShader;
}

void Material::PrepareMaterial(mat4* viewMat, mat4* projMat, mat4* worldMat)
{
	vertexShader->SetMatrix4x4("view", *viewMat); // camera->GetViewMatTransposed());
	vertexShader->SetMatrix4x4("projection", *projMat); //camera->GetProjMatTransposed());
	vertexShader->SetMatrix4x4("world", *worldMat);
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
}
