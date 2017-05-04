#pragma once
//#include "Linker.h"
#include "includes.h"
#include "Camera.h"
#include "Player.h"

/*
#################################################################################################################################
#																																#
#							All Direct X components, mostly the core engine of the pipeline.									#
#							Allowing us to add key techniques as modules.														#
#																																#
#################################################################################################################################
*/

class DX
{
public:
	DX();
	~DX();

	void OfflineCreation(HMODULE hModule, HWND* wndHandle);
	void createMenu();
	void Clean();

	void CreateDirect3DContext(HWND* wndHandle);
	void SetViewport();

	void Update();
	void Render(bool isPlayer);
	void clearRender();

	void CreateShaders();
	void createCBuffer();
	void DepthBuffer();

	void updatePlayerConstantBuffer();
	void updateCameraConstantBuffer();
	void resetConstantBuffer();

	void flushGame();
	void menuControls();
	void startMenuLoop();
	void renderMenu();
	void renderInGameMenu();



	float degreeToRadians(float x) { return x*(XM_PI / 180); };
	void printMatrices(objMatrices mat);

private:
	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackBufferRTV = nullptr;


	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11Buffer* gMenuVertexArray = nullptr;

	ID3D11Buffer** gVertexBufferArray = nullptr; //DENNA!
	int gVertexBufferArray_size;

	//WaterBuffer
	ID3D11Buffer** WaterBufferArray = nullptr;
	int WaterBufferArray_Size;
	ID3D11Buffer* wConstbuff = nullptr;

	ID3D11GeometryShader* gGeometryShader = nullptr;

	ID3D11PixelShader* gFragmentShader = nullptr;

	ID3D11DepthStencilState* depthState = nullptr;
	ID3D11DepthStencilView* gDSV = nullptr;
	ID3D11Texture2D* gDepthStencil = nullptr;

	ID3D11Buffer* gCBuffer = nullptr;
	ID3D11Buffer* menuBuffer = nullptr;

	ID3D11Buffer* shaderBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11ShaderResourceView* gTextureRTV = nullptr;

	Camera* camera;
	Player* player;
	Collision col;
	InteractiveCollision interactiveCol;

	// New code
	int* vertexCountOBJ = nullptr;
	int gVertexBuffer2_size;
	float* objCoords;	//denna?

public:
	FBXImport FBX;
	//FBXImport WaterFBX;
	bool isStartMenu = true;

	objMatrices TEMP;

private:
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAT;
	DirectX::XMVECTOR upVec;
	DirectX::XMVECTOR mRight;


private:
	objMatrices menuMats;
	bool menuMsg = false;
	time_t tButtonPress;
	time_t lTimePress;
};