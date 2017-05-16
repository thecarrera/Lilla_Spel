#pragma once
#include "Includes.h"
class ShadowMap
{
public:

	void ShadowMapping(int WIDTH, int HEIGHT, ID3D11Texture2D*& shadTex, ID3D11Texture2D*& shadMask, ID3D11DepthStencilView*& DepthStencView, ID3D11ShaderResourceView*& ShaderStencView, ID3D11Device*& gDevice, ID3D11DeviceContext *&gDeviceContext, ID3D11ShaderResourceView *&MaskResourceView, ID3D11ShaderResourceView *&groundMaskView)
	{
		//Shadowmap Description

		D3D11_TEXTURE2D_DESC shadTextDesc;
		//Width and height want's to be the same resolution as the Render Target view! RTV is (640 x 800) and this is 2000x2000 causing conflicts!
		shadTextDesc.Width = 800;
		shadTextDesc.Height = 640;
		shadTextDesc.MipLevels = 4;
		shadTextDesc.ArraySize = 1;
		shadTextDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		// multi sample has to be the same the macro PIXELSAMPLE, it's the univerisal pixel sample for all textures! Same with multi sample Quality!
		shadTextDesc.SampleDesc.Count = 1;
		shadTextDesc.SampleDesc.Quality = 0;
		shadTextDesc.Usage = D3D11_USAGE_DEFAULT;
		shadTextDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		shadTextDesc.CPUAccessFlags = 0;
		shadTextDesc.MiscFlags = 0;

		//Depth Stencil Description
		D3D11_DEPTH_STENCIL_VIEW_DESC shadDSV;
		shadDSV.Format = DXGI_FORMAT_D32_FLOAT;
		shadDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		shadDSV.Texture2D.MipSlice = 0;
		shadDSV.Flags = 0;

		//Shader Resource desc 
		D3D11_SHADER_RESOURCE_VIEW_DESC shadSRV;
		shadSRV.Format = DXGI_FORMAT_R32_FLOAT;
		shadSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shadSRV.Texture2D.MipLevels = shadTextDesc.MipLevels;
		shadSRV.Texture2D.MostDetailedMip = 0;
		//Crosshatching TextureMask
		D3D11_TEXTURE2D_DESC shadMaskDesc;
		shadMaskDesc.Width = 8000;
		shadMaskDesc.Height = 8000;
		shadMaskDesc.MipLevels = 1;
		shadMaskDesc.ArraySize = 1;
		shadMaskDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		// multi sample has to be the same the macro PIXELSAMPLE, it's the univerisal pixel sample for all textures!
		shadMaskDesc.SampleDesc.Count = 1;
		shadMaskDesc.SampleDesc.Quality = 0;
		shadMaskDesc.Usage = D3D11_USAGE_DEFAULT;
		shadMaskDesc.CPUAccessFlags = 0;
		shadMaskDesc.MiscFlags = 0;

		D3D11_TEXTURE2D_DESC groundMaskDesc;
		groundMaskDesc.Width = 8000;
		groundMaskDesc.Height = 8000;
		groundMaskDesc.MipLevels = 1;
		groundMaskDesc.ArraySize = 1;
		groundMaskDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		// multi sample has to be the same the macro PIXELSAMPLE, it's the univerisal pixel sample for all textures!
		groundMaskDesc.SampleDesc.Count = 1;
		groundMaskDesc.SampleDesc.Quality = 0;
		groundMaskDesc.Usage = D3D11_USAGE_DEFAULT;
		groundMaskDesc.CPUAccessFlags = 0;
		groundMaskDesc.MiscFlags = 0;

		//Create
		HRESULT hr;
		hr = gDevice->CreateTexture2D(&shadTextDesc, NULL, &shadTex);
		hr = gDevice->CreateDepthStencilView(shadTex, &shadDSV, &DepthStencView);
		hr = gDevice->CreateShaderResourceView(shadTex, &shadSRV, &ShaderStencView);

		wchar_t fileName[256];

		char temp1[256] = "ShadowMask.png";
		for (int i = 0; i < sizeof(temp1); i++)
		{
			fileName[i] = temp1[i];
		}
		hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &MaskResourceView, NULL);

		char temp2[256] = "Ground_Mask.png";
		for (int i = 0; i < sizeof(temp2); i++)
		{
			fileName[i] = temp2[i];
		}

		hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &groundMaskView, NULL);

		if (FAILED(hr)) return;

	}



	ShadowMap::ShadowMap()
	{
	}

	ShadowMap::~ShadowMap()
	{
	}

};