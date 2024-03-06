#include "DepthStencilView.h"
#include "Device.h"
#include <d3dx12.h>
#include "d3d12utils.h"
#include "CommandList.h"

namespace FRenderer
{
    DepthStencilView::DepthStencilView(Device* device,UINT width,UINT height)
        : device(device),mWidth(width),mHeight(height)
    {
        //创建DSV描述符堆
        D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
        dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsvHeapDesc.NodeMask = 0;
        dsvHeapDesc.NumDescriptors = 1;
        dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        HRESULT dsvResult = device->Get()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(mDsvDescriptorHeap.GetAddressOf()));
        FailedErrorBox(L"Failed to create DSV descriptor heap", dsvResult);

        //描述符大小
        mDsvDescriptorSize =device->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }


    DepthStencilView::~DepthStencilView() = default;


    ID3D12Resource* DepthStencilView::GetBufferResource() const noexcept
    {
        return mDepthStencilBuffer.Get();
    }

    Device* DepthStencilView::GetDevice() const noexcept
    {
        return device;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView::Get() const noexcept
    {
        return mDsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();;
    }
    UINT DepthStencilView::GetDescriptorSize() const noexcept
    {
        return mDsvDescriptorSize;
    }

    ID3D12DescriptorHeap* DepthStencilView::GetDescriptorHeap() const noexcept
    {
        return mDsvDescriptorHeap.Get();
    }

    DXGI_FORMAT DepthStencilView::GetFormat() const noexcept
    {
        return mDepthStencilFormat;
    }

    void DepthStencilView::Resize(const CommandList* cmdList,UINT width, UINT height)
    {
        mWidth= width;
        mHeight = height;

        mDepthStencilBuffer.Reset();

        //资源描述符
        D3D12_RESOURCE_DESC depthStencilDesc;
        depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthStencilDesc.Alignment = 0;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.Width = mWidth;
        depthStencilDesc.Height = mHeight;
        depthStencilDesc.DepthOrArraySize = 1;
        depthStencilDesc.Format = mDepthStencilFormat;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        //清除值信息
        D3D12_CLEAR_VALUE optClear;
        optClear.Format = mDepthStencilFormat;
        optClear.DepthStencil.Depth = 1.0f;
        optClear.DepthStencil.Stencil = 0;

        //创建资源
        HRESULT dsBufferResult = device->Get()->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &depthStencilDesc,
            D3D12_RESOURCE_STATE_COMMON,
            &optClear,
            IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())
        );
        FailedErrorBox(L"Failed to create DepthStencilBuffer", dsBufferResult);

        //创建视图
        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Format = mDepthStencilFormat;
        dsvDesc.Texture2D.MipSlice = 0;

        device->Get()->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, Get());

        //使得缓冲区进入可写入的状态
        cmdList->ResourceBarrier(
            mDepthStencilBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_DEPTH_WRITE);
    }


}
