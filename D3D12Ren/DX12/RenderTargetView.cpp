#include "RenderTargetView.h"

#include "d3d12utils.h"
#include "SwapChain.h"
namespace FRenderer
{
	RenderTargetView::RenderTargetView(const Device* device, const UINT descriptorCount)
		:mDevice(device), mDescriptorCount(descriptorCount)
	{
		//创建RTV描述符堆
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;
		rtvHeapDesc.NumDescriptors = descriptorCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		const HRESULT rtvResult = device->Get()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(mRtvDescriptorHeap.GetAddressOf()));
		FailedErrorBox(L"Failed to create RTV descriptor heap", rtvResult);


		//描述符大小
		mRtvDescriptorSize = mDevice->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	RenderTargetView::~RenderTargetView() = default;
	const Device* RenderTargetView::GetDevice() const noexcept
	{
		return mDevice;
	}
	UINT RenderTargetView::GetDescriptorCount() const noexcept
	{
		return mDescriptorCount;
	}
	ID3D12DescriptorHeap* RenderTargetView::GetDescriptorHeap() const noexcept
	{
		return mRtvDescriptorHeap.Get();
	}


	void RenderTargetView::ReCreateView() const noexcept
	{


		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		const int mDescriptorCountInt = static_cast<int>(mDescriptorCount);
		for (int i = 0; i < mDescriptorCountInt; i++)
		{
			mDevice->Get()->CreateRenderTargetView(mRenderTargetBuffers[i].Get(), nullptr, rtvHeapHandle.Offset(i, mRtvDescriptorSize));
			
		}
	}

	void RenderTargetView::SetRenderTargetBuffers(const SwapChain* swapChain)
	{
		mBufferFormat = swapChain->GetBufferFormat();
		if (mRenderTargetBuffers != nullptr)
			//“可能”释放缓冲区资源
			for (int i = 0; i < mDescriptorCount; i++)
				mRenderTargetBuffers[i].Reset();
		//释放指针内存
		delete mRenderTargetBuffers;

		//描述符数量和交换链的缓冲区数一致（View就是描述符，叫法不同其实是DX12和以前版本的遗留问题）
		mDescriptorCount = swapChain->GetBufferCount();
		//缓冲区数组
		mRenderTargetBuffers = new Microsoft::WRL::ComPtr<ID3D12Resource>[mDescriptorCount];

		for (UINT i = 0; i < mDescriptorCount; i++)
		{
			//将交换链的缓冲区指针放入数组缓存
			const HRESULT getBufferResult = swapChain->Get()->GetBuffer(i, IID_PPV_ARGS(mRenderTargetBuffers[i].GetAddressOf()));
			FailedErrorBox(L"Failed to get SwapChain Buffer", getBufferResult);
		}
		//重新创建视图
		ReCreateView();
	}


	void RenderTargetView::SetRenderTargetBuffers(
		const UINT count, const DXGI_FORMAT format, const UINT width, const UINT height)
	{
		mBufferFormat = format;

		if (mRenderTargetBuffers != nullptr)
			//“可能”释放缓冲区资源
			for (int i = 0; i < mDescriptorCount; i++)
				mRenderTargetBuffers[i].Reset();
		//释放指针内存
		delete mRenderTargetBuffers;
		//确定新的缓冲区数量
		mDescriptorCount = count;
		//创建缓冲区数组
		mRenderTargetBuffers = new Microsoft::WRL::ComPtr<ID3D12Resource>[mDescriptorCount];
		//创建资源
		for (UINT i = 0; i < mDescriptorCount; i++)
		{
			const HRESULT getBufferResult = mDevice->Get()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(format, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET),
				D3D12_RESOURCE_STATE_PRESENT,
				nullptr,
				IID_PPV_ARGS(mRenderTargetBuffers[i].GetAddressOf()));
			FailedErrorBox(L"Failed to get SwapChain Buffer", getBufferResult);
		}
		//重新创建视图
		ReCreateView();
	}
	ID3D12Resource* RenderTargetView::GetBuffer(const UINT index) const noexcept
	{
		return mRenderTargetBuffers[index].Get();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE RenderTargetView::Get(const UINT index) const noexcept
	{
		int mIndexInt = static_cast<int>(index);
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			mIndexInt,
			mRtvDescriptorSize);
		
	}

	DXGI_FORMAT RenderTargetView::GetBufferFormat() const noexcept
	{
		return mBufferFormat;
	}

}

