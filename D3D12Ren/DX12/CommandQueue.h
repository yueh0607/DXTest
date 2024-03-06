#pragma once
#include <wrl/client.h>
#include <d3d12.h>

#include "RenderStructure.h"




namespace FRenderer
{
    class Device;
    class FrameResource;
    class CommandList;
    class IFenceValueHandle;


    class CommandQueue : public RenderStructure
    {
        //设备
        const Device* device;
        //队列
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
    public:
       
        
        //禁用拷贝构造函数和赋值构造函数	
        NO_COPY_AND_MOVE(CommandQueue)



        CommandQueue(const Device* device);
        ~CommandQueue();

        const Device* GetDevice() const noexcept;

  
        ID3D12CommandQueue* Get() const noexcept;

        void Flush(IFenceValueHandle* handle) const;

        void Execute(CommandList* cmdList) const;
    };
}
