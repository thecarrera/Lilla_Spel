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
	void Render(int pass, bool isPlayer);
	void clearRender();

	void CreateShaders();
	void createCBuffer();
	void DepthBuffer();

	void updatePlayerConstantBuffer();
	void updateCameraConstantBuffer();
	void resetConstantBuffer();

	void createLightCaster();
	
	void flushGame();
	void menuControls();
	void startMenuLoop();
	void renderMenu();
	void renderInGameMenu();
	void Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView** &RTV);

	float degreeToRadians(float x) { return x*(XM_PI / 180); };
	void printMatrices(objMatrices& mat);

	void updateLevelPos();

private:
	ID3D11Device* gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	IDXGISwapChain* gSwapChain = nullptr;
	ID3D11RenderTargetView* gBackBufferRTV = nullptr;

	ID3D11InputLayout* gVertexLayout = nullptr;
	ID3D11InputLayout* gShadowLayout = nullptr;

	ID3D11VertexShader* gVertexShader = nullptr;
	ID3D11VertexShader* gShadowVertexShader = nullptr;
	ID3D11VertexShader* gBoneVertexShader = nullptr;
	ID3D11VertexShader* gBoneShadowVertexShader = nullptr;

	ID3D11Buffer* gMenuVertexArray = nullptr;
	
	ID3D11Buffer** gVertexBufferArray = nullptr; //DENNA!
	int gVertexBufferArray_size;

	ID3D11GeometryShader* gGeometryShader = nullptr;

	ID3D11ComputeShader* gBlurShader = nullptr;
	ID3D11UnorderedAccessView* gBlurUAV = nullptr;

	ID3D11PixelShader* gFragmentShader = nullptr;
	ID3D11PixelShader* gMenuFragmentShader = nullptr;

	ID3D11DepthStencilState* depthState = nullptr;
	ID3D11DepthStencilView* gDSV = nullptr;
	ID3D11Texture2D* gDepthStencil = nullptr;
	
	//shadowmap
	ShadowMap						*shadowMap = nullptr;
	ID3D11Texture2D					*ShadowMask = nullptr;
	ID3D11Texture2D					*ShadowmapTex = nullptr;
	ID3D11DepthStencilView			*ShadowDepthStencilView = nullptr;
	ID3D11ShaderResourceView		*ShadowShaderRecourceView = nullptr;
	ID3D11ShaderResourceView		*ShadowMaskResourceView = nullptr;
	ID3D11ShaderResourceView		*GroundMaskRV = nullptr;
	ID3D11SamplerState				*ShadowSampler = nullptr;
	ID3D11SamplerState				*sMaskSamplerState = nullptr;

	ID3D11Buffer* gCBuffer = nullptr;
	ID3D11Buffer* menuBuffer = nullptr;

	//ID3D11Buffer* shaderBuffer = nullptr;
	ID3D11SamplerState* txSamplerState = nullptr;

	ID3D11ShaderResourceView** gTextureRTV = nullptr;
	int textureCount = 0;

	Camera* camera = nullptr;
	Player* player = nullptr;
	Collision col;
	InteractiveCollision interactiveCol;
	Skeleton skeletons;
	chrono::high_resolution_clock::time_point lastFrameTime;
	float deltaTime = 0;

	// New code
	int* vertexCountOBJ = nullptr;
	int gVertexBuffer2_size;
	float* objCoords;	//denna?


	int currentLevel = -100;
	int nextLevel = -200;

public:
	FBXImport FBX;
	bool isStartMenu = true;

	SoundManager SM;

	objMatrices TEMP;
	objMatrices originalLightMatrix;

	objMatrices test;

private:
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR lookAT;
	DirectX::XMVECTOR upVec;
	DirectX::XMVECTOR mRight;

	objMatrices lMatrix;
	ID3D11Buffer* lcBuffer = nullptr;
	ID3D11Buffer* boneBuffer = nullptr;
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };

private:
	objMatrices menuMats;
	bool menuMsg = false;
	time_t tButtonPress;
	time_t lTimePress;
};
