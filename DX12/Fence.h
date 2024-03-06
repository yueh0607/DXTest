#pragma once

#include <wrl/client.h>
#include <d3d12.h>

#include "RenderStructure.h"

namespace FRenderer
{
	class Device;

	class Fence : public RenderStructure
	{
		//设备
		Device* device;
		//围栏
		Microsoft::WRL::ComPtr<ID3D12Fence> mFence;

	public:

		//禁用拷贝构造函数和赋值构造函数	
		NO_COPY_AND_MOVE(Fence)

			Fence(Device* device);

		~Fence();

		ID3D12Fence* Get() const noexcept;

		Device* GetDevice() const noexcept;

	};

	//用于设置围栏值的接口
	class IFenceValueHandle
	{
		Fence* mFence;
	public:
		IFenceValueHandle(Fence* fence);
		virtual ~IFenceValueHandle() = default;
		NO_COPY_AND_MOVE(IFenceValueHandle)


		Fence* GetFence() const noexcept;
		//设置围栏值
		virtual void SetFenceValue(UINT value) noexcept = 0;
		//获取围栏值
		virtual UINT GetFenceValue() const noexcept = 0;
	};
}
