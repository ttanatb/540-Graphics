#include "Debug.h"
using namespace DirectX;
Debug* Debug::instance = nullptr;

Debug::Debug()
{
	lines = std::vector<DebugLine>();
	cuboids = std::vector<mat4>();
	spheres = std::vector<mat4>();
}

Debug * Debug::GetInstance()
{
	if (instance == nullptr)
		instance = new  Debug();

	return instance;
}

void Debug::ReleaseInstance()
{
	if (instance != nullptr)
		delete(instance);
}

void Debug::SetShaders()
{
	//do some init stuff?
}

void Debug::Release()
{
	//??
}

void Debug::Draw(mat4* viewMat, mat4* projMat)
{


	//clear all vectors
	lines.clear();
	spheres.clear();
	cuboids.clear();
}

void Debug::DrawLine(vec3 pos1, vec3 pos2)
{
	if (instance != nullptr)
		instance->lines.push_back({ pos1, pos2 });
}

void Debug::DrawCuboid(vec3 pos, vec3 scale)
{
	if (instance == nullptr) return;

	mat4 worldMat;
	XMMATRIX matrix = XMMatrixMultiply(XMMatrixTranslation(pos.x, pos.y, pos.z), XMMatrixScaling(scale.x, scale.y, scale.z));
	XMStoreFloat4x4(&worldMat, matrix);

	instance->cuboids.push_back(worldMat);
}

void Debug::DrawSphere(vec3 pos, float radius)
{
	if (instance == nullptr) return;

	mat4 worldMat;
	XMMATRIX matrix = XMMatrixMultiply(XMMatrixTranslation(pos.x, pos.y, pos.z), XMMatrixScaling(radius, radius, radius));
	XMStoreFloat4x4(&worldMat, matrix);

	instance->cuboids.push_back(worldMat);
}
