#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity()
{
	position = vec3(0.0f, 0.0f, 0.0f);
	rotation = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	scale = vec3(1.0f, 1.0f, 1.0f);

	XMMATRIX mat = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, mat);
	meshPtr = nullptr;
}

GameEntity::GameEntity(Mesh * mesh, vec3 position, vec4 rotation, vec3 scale)
{
	meshPtr = mesh;

	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	XMVECTOR quaternion = XMLoadFloat4(&rotation);

	XMMATRIX mat = XMMatrixMultiply((XMMatrixMultiply(XMMatrixScaling(scale.x, scale.y, scale.z),
		XMMatrixRotationQuaternion(quaternion))),
		XMMatrixTranslation(position.x, position.y, position.z));
	XMStoreFloat4x4(&worldMatrix, mat);
}

GameEntity::GameEntity(Mesh * mesh, vec3 position, vec3 rotation, vec3 scale)
{
	meshPtr = mesh;

	this->position = position;
	this->scale = scale;

	XMVECTOR quaternion = XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMStoreFloat4(&(this->rotation), quaternion);

	//scale x rotation x translation
	XMMATRIX mat = XMMatrixMultiply((XMMatrixMultiply(XMMatrixScaling(scale.x, scale.y, scale.z),
													  XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z))), 
									 XMMatrixTranslation(position.x, position.y, position.z));
	XMStoreFloat4x4(&worldMatrix, mat);
}

void GameEntity::Update()
{
	XMMATRIX mat = XMLoadFloat4x4(&worldMatrix);
	XMVECTOR quaternion = XMLoadFloat4(&rotation);

	mat = XMMatrixMultiply((XMMatrixMultiply(XMMatrixScaling(scale.x, scale.y, scale.z),
		XMMatrixRotationQuaternion(quaternion))),
		XMMatrixTranslation(position.x, position.y, position.z));
	XMStoreFloat4x4(&worldMatTransposed, DirectX::XMMatrixTranspose(mat));
}

DirectX::XMFLOAT4X4* GameEntity::GetWorldMat()
{
	return &worldMatTransposed;
}

vec3 GameEntity::GetPosition() { return position; }
vec4 GameEntity::GetRotation() { return rotation; }
vec3 GameEntity::GetRotEuler()
{
	return vec3();
}

vec3 GameEntity::GetScale() { return scale; }



Mesh * GameEntity::GetMesh()
{
	return meshPtr;
}

void GameEntity::SetMesh(Mesh* mesh)
{
	meshPtr = mesh;
}

Material * GameEntity::GetMat()
{
	return matPtr;
}

void GameEntity::SetMat(Material * newMat)
{
	matPtr = newMat;
}

void GameEntity::TranslateBy(float x, float y, float z)
{
	TranslateBy(vec3(x, y, z));
}

void GameEntity::TranslateBy(vec3 translation)
{
	XMVECTOR newPos = XMVectorAdd(XMLoadFloat3(&translation), XMLoadFloat3(&position));
	XMStoreFloat3(&position, newPos);
}

void GameEntity::RotateOnAxis(float x, float y, float z, float angle)
{
	RotateOnAxis(vec3(x, y, z), angle);
}

void GameEntity::RotateOnAxis(vec3 axis, float angle)
{
	XMVECTOR quaternion = XMQuaternionMultiply(XMLoadFloat4(&rotation), 
		XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle));
	XMStoreFloat4(&rotation, quaternion);
}


void GameEntity::ScaleBy(float x, float y, float z)
{
	ScaleBy(vec3(x, y, z));
}

void GameEntity::ScaleBy(vec3 toScale)
{
	XMVECTOR newScale = XMVectorAdd(XMLoadFloat3(&scale), XMLoadFloat3(&toScale));
	XMStoreFloat3(&scale, newScale);
}
