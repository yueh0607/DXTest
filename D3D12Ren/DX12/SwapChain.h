#pragma once
#include <dxgi.h>

namespace FRenderer
{
	class CommandQueue;
	class Device;


	class SwapChain : public RenderStructure
	{
		//缓冲区大小（可变）
		UINT mWidth, mHeight;
		//设备
		const Device* mDevice;
		//命令队列
		const CommandQueue* mCommandQueue;
		//句柄
		const HWND mHWND;
		//缓冲区格式
		const DXGI_FORMAT mBufferFormat;
		//缓冲区数量
		const UINT mBufferCount;
		//交换链
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
		//当前缓存区索引值
		UINT mCurrentBackBufferIndex;
	public:
		NO_COPY_AND_MOVE(SwapChain)

			SwapChain(const Device* device,const CommandQueue* cmdQueue, const HWND hWnd,
				const UINT width, const UINT height,
				const DXGI_FORMAT bufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM, const UINT bufferCount = 2
			);
		~SwapChain();

		//获取交换链
		IDXGISwapChain* Get() const noexcept;
		//获取交换链的缓冲区数量
		UINT GetBufferCount()const noexcept;
		//获取当前的缓冲区索引值
		UINT GetCurrentBackBufferIndex() const noexcept;

		//呈现
		void Present(bool vSync);

		//调整缓冲区大小
		void ResizeBuffer(const UINT width, const UINT height);


		DXGI_FORMAT GetBufferFormat() const noexcept;
	};


}
