#include "Device.h"
#include "CommandList.h"
#include "FrameResource.h"
#include "d3d12utils.h"
#include "CommandAllocator.h"
namespace FRenderer
{

    FrameResource::FrameResource(const Device* device,Fence* fence)
	    :IFenceValueHandle(fence)
    {
        mCmdListAlloc= new CommandAllocator(device);
    }


    FrameResource::~FrameResource() = default;

    CommandAllocator* FrameResource::GetCmdListAlloc() const noexcept
    {
        return mCmdListAlloc;
    }

    void FrameResource::SetFenceValue(UINT value) noexcept
    {
        mFenceValue = value;
    }

    UINT FrameResource::GetFenceValue() const noexcept
    {
        return mFenceValue;
    }

}
