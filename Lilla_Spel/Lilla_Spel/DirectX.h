#pragma once
//#include "Linker.h"
#include "includes.h"
#include "Player.h"
#include "Camera.h"

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
	void Clean();

	void CreateDirect3DContext(HWND* wndHandle);
	void SetViewport();

	void Update();
	void Render(bool isPlayer);
	void RenderParticles();

	void CreateShaders();
	void createGCBuffer();
	void DepthBuffer();

	void updatePlayerConstantBuffer();
	void updateCameraConstantBuffer();
	void resetConstantBuffer();

private:
	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackBufferRTV = nullptr;

	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11Buffer* gVertexBuffer = nullptr;
	ID3D11VertexShader* gVertexShader = nullptr;

	ID3D11GeometryShader* gGeometryShader = nullptr;

	ID3D11PixelShader* gFragmentShader = nullptr;

	ID3D11DepthStencilState* depthState = nullptr;
	ID3D11DepthStencilView* gDSV = nullptr;
	ID3D11Texture2D* gDepthStencil = nullptr;

	ID3D11Buffer* gCBuffer = nullptr;

	ID3D11Buffer* shaderBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11ShaderResourceView* gTextureRTV = nullptr;

	Camera* camera;
	Player* player;

	//Particles
	ID3D11VertexShader* gVSParticle = nullptr;
	ID3D11GeometryShader* gGSParticle = nullptr;
	ID3D11PixelShader* gPSParticle = nullptr;

	// New code
	int* vertexCountOBJ = nullptr;
	int gVertexBuffer2_size;
	float* objCoords;	//denna?
	ID3D11Buffer** gVertexBufferArray = nullptr; //DENNA!

private:
	//Linker linker;
private:
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAT;
	DirectX::XMVECTOR upVec;
	DirectX::XMVECTOR mRight;
};