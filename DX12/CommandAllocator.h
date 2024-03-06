#pragma once
#include <wrl/client.h>
#include <d3d12.h>
#include "RenderStructure.h"

namespace  FRenderer
{
	class Device;
	//抽象命令分配器
	class CommandAllocator : public RenderStructure
	{
	private:
		const Device* mDevice;
		//Direct命令分配器
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;

	public:

		CommandAllocator(const Device* device);
		~CommandAllocator();

		//禁用拷贝构造函数和赋值构造函数
		NO_COPY_AND_MOVE(CommandAllocator)

			//重置命令分配器
		void Reset() const;

		//获取指针
		ID3D12CommandAllocator* Get() const noexcept;

		//获取设备资源
		const Device* GetDevice() const noexcept;
	};

}
