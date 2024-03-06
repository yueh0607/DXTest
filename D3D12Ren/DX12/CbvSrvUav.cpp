#include "CbvSrvUav.h"

#include "d3d12utils.h"
#include "UploadBuffer.h"

namespace FRenderer
{
	CbvSrvUav::CbvSrvUav(const Device* device, const UINT bufferCount)
	{
		D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
		cbvHeapDesc.NumDescriptors = bufferCount;
		cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		cbvHeapDesc.NodeMask = 0;
		const HRESULT createResult = device->Get()->CreateDescriptorHeap(&cbvHeapDesc,
			IID_PPV_ARGS(&mDescriptorHeap));
		FailedErrorBox(L"Failed to create CBV_SRV_UAV descriptor heap.", createResult);
		//取得CbvSrvUav描述符大小
		mDescriptorSize = device->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	CbvSrvUav::~CbvSrvUav() = default;

	ID3D12DescriptorHeap* CbvSrvUav::Get() const noexcept
	{
		return mDescriptorHeap.Get();
	}

	void CbvSrvUav::ForceOffset(UINT count) noexcept
	{
		mConstantBufferCount += count;
	}

	CD3DX12_GPU_DESCRIPTOR_HANDLE CbvSrvUav::GetOffest(UINT index) const noexcept
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(mDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),index,mDescriptorSize);
	}


}

