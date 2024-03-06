#include "Fence.h"
#include "d3d12utils.h"


namespace FRenderer
{
	Fence::Fence(Device* device)
		:device(device)
	{
		//创建同步围栏
		HRESULT fenceResult = device->Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(mFence.GetAddressOf()));
		FailedErrorBox(L"Failed to create Fence", fenceResult);
	}
	Fence::~Fence() = default;

	ID3D12Fence* Fence::Get() const noexcept
	{
		return mFence.Get();
	}

	Device* Fence::GetDevice() const noexcept
	{
		return device;
	}



	IFenceValueHandle::IFenceValueHandle(Fence* fence)
		:mFence(fence)
	{
		
	}

	Fence* IFenceValueHandle::GetFence() const noexcept
	{
		return mFence;
	}

}
