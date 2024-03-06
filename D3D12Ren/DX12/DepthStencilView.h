#pragma once
#include <d3d12.h>
#include <dxgiformat.h>
#include <wrl/client.h>

#include "Device.h"

class Device;

namespace FRenderer
{
    class CommandList;
    class DepthStencilView: public RenderStructure
    {
        //设备
        Device* device;
        //深度模板格式
        const DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D32_FLOAT;
        //深度/模板缓冲区
        Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;
        //DSV描述符堆
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvDescriptorHeap;
        //缓冲区大小（和RTV相同即可）
        UINT mWidth,mHeight;
        //描述符大小
        UINT mDsvDescriptorSize;
        
    public:

        DepthStencilView(Device* device,UINT width,UINT height);
        ~DepthStencilView();

        //获取DSV缓冲区资源
        ID3D12Resource * GetBufferResource() const noexcept;
        //获取设备
        Device* GetDevice() const noexcept;
        //获取DSV
        D3D12_CPU_DESCRIPTOR_HANDLE Get() const noexcept;
        //获取FSV描述符大小
        UINT GetDescriptorSize() const noexcept;
        //获取描述符堆
        ID3D12DescriptorHeap* GetDescriptorHeap() const noexcept;

        //调整大小(创建之后首次使用需要调整大小)
        void Resize(const CommandList* cmdList,UINT width, UINT height);

        DXGI_FORMAT GetFormat() const noexcept;

        NO_COPY_AND_MOVE(DepthStencilView)


    };
}
