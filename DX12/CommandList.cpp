#include "CommandList.h"

#include <d3dx12_barriers.h>

#include "CommandAllocator.h"
#include "d3d12utils.h"
#include "Device.h"
#include "PipelineStateObject.h"

namespace FRenderer
{
	CommandList::CommandList(const Device* device,const CommandAllocator* mCmdAllocator)
		: device(device), mCmdAllocator(const_cast<CommandAllocator*>(mCmdAllocator)), mIsOpening(false)
	{
		//创建命令列表
		const HRESULT listResult = device->Get()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			mCmdAllocator->Get(),
			nullptr,
			IID_PPV_ARGS(mCommandList.GetAddressOf()));
		FailedErrorBox(L"Failed to create CommandList", listResult);
		Close();
	}

	ID3D12GraphicsCommandList* CommandList::Get() const noexcept
	{
		return mCommandList.Get();
	}

	bool CommandList::IsOpen() const noexcept
	{
		return mIsOpening;
	}

	const Device* CommandList::GetDevice() const noexcept
	{
		return device;
	}

	const CommandAllocator* CommandList::GetCommandAllocator() const noexcept
	{
		return mCmdAllocator;
	}
	void CommandList::SetCommandAllocator(CommandAllocator* allocator) noexcept
	{
		mCmdAllocator = allocator;
	}


	void CommandList::Reset(PipelineStateObject* pso)
	{
		const HRESULT resetResult = mCommandList->Reset(mCmdAllocator->Get(), pso == nullptr ? nullptr : pso->Get());
		FailedErrorBox(L"Failed to reset CommandList", resetResult);
		mIsOpening = true;
	}

	void CommandList::Close()
	{
		const HRESULT closeResult2 = mCommandList->Close();
		FailedErrorBox(L"Failed to close command list.", closeResult2);
		mIsOpening = false;
	}

	void CommandList::ResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after) const
	{
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			pResource,
			before,
			after));
	}
}
