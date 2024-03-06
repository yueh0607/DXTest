#include "CommandAllocator.h"
#include "d3d12utils.h"
#include "Device.h"


namespace FRenderer
{
	CommandAllocator::CommandAllocator(const Device* device)
		:mDevice(device)
	{
		const HRESULT hr = device->Get()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mDirectCmdListAlloc));
		FailedErrorBox(L"Failed to create command allocator", hr);
	}

	CommandAllocator::~CommandAllocator() = default;


	void CommandAllocator::Reset() const
	{
		const HRESULT resetResult = mDirectCmdListAlloc->Reset();
		FailedErrorBox(L"Failed to reset command allocator", resetResult);
	}

	
	ID3D12CommandAllocator* CommandAllocator::Get() const noexcept
	{
		return mDirectCmdListAlloc.Get();
	}

	const Device* CommandAllocator::GetDevice() const noexcept
	{
		return mDevice;
	}


}
