#include "Mesh.h"

Mesh::Mesh(Vertex * vertices, int vertexCount, int * indices, int indexCount, ID3D11Device * device)
{
	this->indexCount = indexCount;

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indexCount;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}

Mesh::~Mesh()
{
	Release();
}

ID3D11Buffer * Mesh::GetVertexBuffer() { return vertexBuffer; }
ID3D11Buffer * Mesh::GetIndexBuffer() {	return indexBuffer; }
int Mesh::GetIndexCount() { return indexCount; }

Mesh * Mesh::CreateCube(ID3D11Device* device, float size, Color c)
{
	Vertex vertices[] =
	{
		{ vec3(-size, -size, -size), c.GetVec4() }, //0
		{ vec3(-size, -size, +size), c.GetVec4() }, //1
		{ vec3(-size, +size, -size), c.GetVec4() }, //2
		{ vec3(-size, +size, +size), c.GetVec4() }, //3
		{ vec3(+size, -size, -size), c.GetVec4() }, //4
		{ vec3(+size, -size, +size), c.GetVec4() }, //5
		{ vec3(+size, +size, -size), c.GetVec4() }, //6
		{ vec3(+size, +size, +size), c.GetVec4() }, //7
	};

	int indices[] = 
	{
		1, 5, 3,	//front
		3, 5, 7,
		4, 0, 6,	//back
		6, 0, 2,
		0, 1, 2,	//left
		2, 1, 3,
		5, 4, 7,	//right
		7, 4, 6,
		3, 7, 2,	//up
		2, 7, 6,
		0, 4, 1,	//down
		1, 4, 5,
	};

	return new Mesh(vertices, 8, indices, 36, device);
}

void Mesh::Release()
{
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }
}
