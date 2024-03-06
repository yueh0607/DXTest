#pragma once
#include <wrl/client.h>

#include "RenderStructure.h"
#include "d3d12.h"
namespace FRenderer
{
	class Device;
	class SwapChain;
	class RenderTargetView : public RenderStructure
	{
		//设备
		const Device* mDevice;
		//描述符数量
		UINT mDescriptorCount;
		//描述符大小
		UINT mRtvDescriptorSize;
		//描述符堆
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvDescriptorHeap;
		//渲染目标缓冲区
		Microsoft::WRL::ComPtr<ID3D12Resource>* mRenderTargetBuffers;
		//缓冲区格式(在没有SetTargetBuffer之前都是未知状态)
		DXGI_FORMAT mBufferFormat;
		//重新创建View
		void ReCreateView() const noexcept;

	public:
		NO_COPY_AND_MOVE(RenderTargetView)

		RenderTargetView(const Device* device,const UINT descriptorCount);
		~RenderTargetView();

		//获取设备
		const Device* GetDevice() const noexcept;

		//获取描述符数量/缓冲区数量
		UINT GetDescriptorCount() const noexcept;

		//获取描述符堆
		ID3D12DescriptorHeap* GetDescriptorHeap() const noexcept;

		
		//使用交换链的缓冲区设置RTV的缓冲区
		void SetRenderTargetBuffers(const SwapChain* swapChain);
		//为RTV创建新的缓冲区并设置
		void SetRenderTargetBuffers(
			const UINT count,const DXGI_FORMAT format,const UINT width,const UINT height);

		//获取第index个缓冲区
		ID3D12Resource* GetBuffer(const UINT index) const noexcept;
		//获取第index个View
		D3D12_CPU_DESCRIPTOR_HANDLE Get(const UINT index) const noexcept;
		//获取缓冲区格式
		DXGI_FORMAT GetBufferFormat() const noexcept;




	};
}
