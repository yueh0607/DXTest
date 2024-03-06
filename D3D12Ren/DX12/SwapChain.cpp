#pragma once
#include "d3d12utils.h"
#include "Device.h"
#include"SwapChain.h"

#include <dxgi.h>

#include"CommandQueue.h"
#include "Factory.h"
namespace FRenderer
{


	SwapChain::SwapChain(const Device* device,const CommandQueue* cmdQueue,const HWND hWnd,
		const UINT width,const UINT height,const DXGI_FORMAT bufferFormat,const UINT bufferCount)
		:mWidth(width),mHeight(height),mDevice(device),
		mCommandQueue(cmdQueue),mHWND(hWnd),mBufferFormat(bufferFormat),mBufferCount(bufferCount),
		mCurrentBackBufferIndex(0)
	{
		
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc.Width = mWidth;
		swapChainDesc.BufferDesc.Height = mHeight;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.Format = mBufferFormat;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = mHWND;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = 0;
		swapChainDesc.BufferCount = mBufferCount;

		//创建交换链
		const HRESULT swapChainResult = mDevice->GetFactory()->Get()->CreateSwapChain(
			mCommandQueue->Get(),
			&swapChainDesc,
			mSwapChain.GetAddressOf());
		
		FailedErrorBox(L"Failed to create SwapChain", swapChainResult);
	}

	SwapChain::~SwapChain() = default;

	IDXGISwapChain* SwapChain::Get() const noexcept
	{
		return mSwapChain.Get();
	}

	UINT SwapChain::GetBufferCount() const noexcept
	{
		return mBufferCount;
	}

	UINT SwapChain::GetCurrentBackBufferIndex() const noexcept
	{
		return mCurrentBackBufferIndex;
	}

	/// <summary>
	/// 呈现
	/// </summary>
	/// <param name="vSync"></param>
	void SwapChain::Present(bool vSync)
	{
		const HRESULT presentResult = mSwapChain->Present(vSync, 0);
		FailedErrorBox(L"Failed to present swap chain.", presentResult);
		mCurrentBackBufferIndex = (mCurrentBackBufferIndex+1) % mBufferCount;
	}

	void SwapChain::ResizeBuffer(const UINT width, const UINT height)
	{
		mWidth= width;
		mHeight = height;

		const HRESULT resizeBufferResult = mSwapChain->ResizeBuffers(
			mBufferCount,
			mWidth, mHeight,
			mBufferFormat,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		mCurrentBackBufferIndex = 0;
		FailedErrorBox(L"Failed to resize SwapChain Buffer", resizeBufferResult);
	}

	DXGI_FORMAT SwapChain::GetBufferFormat() const noexcept
	{
		return mBufferFormat;
	}

}