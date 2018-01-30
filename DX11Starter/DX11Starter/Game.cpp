#include "Game.h"

//remove later
#include <iostream>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	mesh1 = nullptr;
	mesh2 = nullptr;
	mesh3 = nullptr;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	for (size_t i = 0; i < gameEntities.size(); ++i) {
		delete gameEntities[i];
	}

	Mesh::ReleasePrimitives();
	
	if (mesh1 != nullptr) delete mesh1;
	if (mesh3 != nullptr) delete mesh3;

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	Color red = Color::Red();
	Color green = Color::Green();
	Color blue = Color::Blue();

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ vec3(-2.0f, -1.0f, +0.0f), green.GetVec4() },
		{ vec3(-0.0f, +1.5f, +0.0f), green.GetVec4() },
		{ vec3(+2.0f, -1.0f, +0.0f), green.GetVec4() },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	int indices[] = { 0, 1, 2, 2, 1, 0 };

	mesh1 = new Mesh(vertices, 3, indices, 6, device);
	Mesh::CreateCube(device);
	mesh2 = Mesh::GetCubeMeshPtr();

	Vertex vertices3[] =
	{
		{ vec3(+2.0f, +1.5f, +0.0f), blue.GetVec4() },
		{ vec3(+3.5f, +1.5f, +0.0f), blue.GetVec4() },
		{ vec3(+2.0f, -1.0f, +0.0f), blue.GetVec4() },
	};

	mesh3 = new Mesh(vertices3, 3, indices, 3, device);

	gameEntities.push_back(new GameEntity(mesh2, vec3(-2.0f, 1.5f, 0.0f), vec3(0.0f, 45.0f, 0.0f), vec3(2.0f, 0.5f, -1.0f)));
	gameEntities.push_back(new GameEntity(mesh2, vec3(-2.5f, -1.0f, 2.0f), vec3(45.0f, 0.0f, 45.0f), vec3(1.0f, -1.0f, 1.0f)));
	gameEntities.push_back(new GameEntity(mesh2, vec3(0.5f, -0.5f, 0.0f), vec3(45.0f, 0.0f, 0.0f), vec3(-2.0f, -2.0f, 1.0f)));
	gameEntities.push_back(new GameEntity(mesh1, vec3(3.0f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f)));
	gameEntities.push_back(new GameEntity(mesh3, vec3(-4.0f, -0.5f, 0.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f)));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	for (size_t i = 0; i < gameEntities.size(); ++i) {
		gameEntities[i]->Update();
	}
	gameEntities[0]->TranslateBy(sin(totalTime) / 200.0f, 0.0f, 0.0f);
	gameEntities[1]->RotateOnAxis(sin(totalTime), 0.0f, cos(totalTime), deltaTime * 2.0f);
	gameEntities[2]->ScaleBy(sin(totalTime) / 1000.0f, 0.0f, sin(totalTime) / 1000.0f);
	gameEntities[3]->TranslateBy(0.0f, cos(totalTime * 20.0f) / 500.0f, 0.0f);
	gameEntities[3]->RotateOnAxis(0.0f, 1.0f, 0.0f, deltaTime);
	gameEntities[4]->TranslateBy(sin(totalTime * 20.0f)/ 500.0f, cos(totalTime * 20.0f) / 500.0f, 0.0f);
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	//camera stuff
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Set buffers in the input assembler

	//ID3D11Buffer * vertexBuffer = mesh1->GetVertexBuffer();
	//context->IASetVertexBuffers(0, 1, &(vertexBuffer), &stride, &offset);
	//context->IASetIndexBuffer(mesh1->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	for (size_t i = 0; i < gameEntities.size(); ++i) {
		Mesh* meshPtr = gameEntities[i]->GetMesh();
		if (meshPtr == nullptr) continue;

		vertexShader->SetMatrix4x4("world", *(gameEntities[i]->GetWorldMat()));
		vertexShader->CopyAllBufferData();

		vertexShader->SetShader();
		pixelShader->SetShader();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer * vertexBuffer = meshPtr->GetVertexBuffer();
		context->IASetVertexBuffers(0, 1, &(vertexBuffer), &stride, &offset);
		context->IASetIndexBuffer(meshPtr->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(meshPtr->GetIndexCount(), 0, 0);
	}

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	//context->DrawIndexed(
	//	mesh1->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
	//	0,     // Offset to the first index we want to use
	//	0);    // Offset to add to each index when looking up vertices

	//vertexBuffer = mesh2->GetVertexBuffer();
	//context->IASetVertexBuffers(0, 1, &(vertexBuffer), &stride, &offset);
	//context->IASetIndexBuffer(mesh2->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	//context->DrawIndexed(mesh2->GetIndexCount(), 0, 0);

	//vertexBuffer = mesh3->GetVertexBuffer();
	//context->IASetVertexBuffers(0, 1, &(vertexBuffer), &stride, &offset);
	//context->IASetIndexBuffer(mesh3->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	//context->DrawIndexed(mesh3->GetIndexCount(), 0, 0);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion