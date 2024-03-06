#include "CommandQueue.h"
#include "d3d12utils.h"
#include "Device.h"
#include "FrameResource.h"
#include "CommandList.h"
#include "Fence.h"

namespace FRenderer
{
    CommandQueue::CommandQueue(const Device* device)
        : device(device)
    {
        //创建命令队列
        D3D12_COMMAND_QUEUE_DESC queueDesc;
        queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.NodeMask = 0;
        queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

        HRESULT queueResult = device->Get()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(mCommandQueue.GetAddressOf()));
        FailedErrorBox(L"Failed to create CommandQueue", queueResult);
    }

    CommandQueue::~CommandQueue() = default;

    const Device* CommandQueue::GetDevice() const noexcept
    {
        return device;
    }

    ID3D12CommandQueue* CommandQueue::Get() const noexcept
    {
        return mCommandQueue.Get();
    }

    void CommandQueue::Flush(IFenceValueHandle* fence) const
    {
        fence->SetFenceValue(fence->GetFenceValue() + 1);

        const HRESULT signalResult = mCommandQueue->Signal(fence->GetFence()->Get(),fence->GetFenceValue());
        FailedErrorBox(L"Failed to signal CommandQueue", signalResult);

        if (fence->GetFence()->Get()->GetCompletedValue() < fence->GetFenceValue())
        {
            const HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
            assert(eventHandle);

            const HRESULT waitResult = fence->GetFence()->Get()->SetEventOnCompletion(fence->GetFenceValue(), eventHandle);
            FailedErrorBox(L"Failed to set event on completion", waitResult);

            WaitForSingleObject(eventHandle, INFINITE);
            CloseHandle(eventHandle);
        }
    }

    void CommandQueue::Execute(CommandList* cmdList) const
    {
        ID3D12CommandList* cmdLists[] = { cmdList->Get() };
        mCommandQueue->ExecuteCommandLists(1, cmdLists);
    }
}
