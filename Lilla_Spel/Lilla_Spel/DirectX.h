#pragma once
#include "Includes.h"
#include "Linker.h"
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
	void Clean();

	void CreateDirect3DContext(HWND* wndHandle);
	void SetViewport();

	void Update();
	void Render();
	void characterMove();
	void cameraMov();

	void CreateShaders();
	void ConstantBuffer();
	void DepthBuffer();

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
	struct objMatrices
	{
		DirectX::XMMATRIX worldM;
		DirectX::XMMATRIX viewM;
		DirectX::XMMATRIX projM;
	};
	objMatrices cameraMatrices;

	ID3D11Buffer* shaderBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11ShaderResourceView* gTextureRTV = nullptr;

	objMatrices characterMatrices;
	Player player;


private:
	Linker linker;
private:
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAT;
	DirectX::XMVECTOR upVec;
	DirectX::XMVECTOR mRight;
};