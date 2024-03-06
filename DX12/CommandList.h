#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include "RenderStructure.h"	

namespace FRenderer
{
	class Device;
	class PipelineStateObject;
	class CommandAllocator;

	class CommandList : public RenderStructure
	{
		//设备
		const Device* device;
		//分配器
		CommandAllocator* mCmdAllocator;
		//命令列表
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;
		//是否是Open状态
		bool mIsOpening ;
	public:

		//禁用拷贝构造函数和赋值构造函数	
		NO_COPY_AND_MOVE(CommandList)


		CommandList(const Device* device,const CommandAllocator* allocator);

		~CommandList() = default;

		const CommandAllocator* GetCommandAllocator() const noexcept;

		void SetCommandAllocator(CommandAllocator* allocator) noexcept;

		//获取命令列表
		ID3D12GraphicsCommandList* Get() const noexcept;


		//获取设备
		const Device* GetDevice() const noexcept;

		//是否处于打开状态
		bool IsOpen() const noexcept;

		//重置命令列表
		void Reset(PipelineStateObject* pso=nullptr);

		//关闭命令列表
		void Close() ;

		//资源屏障：转换
		void ResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const;
	};
}
