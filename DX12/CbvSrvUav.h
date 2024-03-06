#pragma once
#include <d3d12.h>
#include <intsafe.h>
#include <memory>
#include <wrl/client.h>

#include "RenderStructure.h"
#include "UploadBuffer.h"

namespace FRenderer
{

	struct DescriptorHeapIndices
	{
		enum
		{
			Font = 0,
			Reserve = 1,
			MaxCount = 1024
		};
	};

	class Device;
	class CbvSrvUav : public RenderStructure
	{
		//描述符堆
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
		//描述符大小
		UINT mDescriptorSize;
		//当前
		UINT mCurrentCount = DescriptorHeapIndices::Reserve;
		UINT mConstantBufferCount = 0;
	public:

		CbvSrvUav(const Device* device,const UINT bufferCount);
		~CbvSrvUav();

		//创建常量缓冲区视图
		template <typename T>
		std::unique_ptr<UploadBuffer<T>> CreateConstantBufferView(const Device* device,const UINT elementCount)
		{
			//偏移值
			UINT currentOffset = 0;
			//创建常量缓冲区（上传缓冲区）
			auto mUploadBuffer = std::make_unique<UploadBuffer<T>>(device->Get(),elementCount, true);
			for(int i=0;i< elementCount;i++)
			{
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
				cbvDesc.BufferLocation = currentOffset + mUploadBuffer->Resource()->GetGPUVirtualAddress();
				currentOffset += mUploadBuffer->ElementByteSize();
				cbvDesc.SizeInBytes = mUploadBuffer->ElementByteSize();

				auto descriptorHeapAddress = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				descriptorHeapAddress.ptr += mCurrentCount * mDescriptorSize;

				device->Get()->CreateConstantBufferView(
					&cbvDesc,
					descriptorHeapAddress);
				mCurrentCount++;
			}
			return mUploadBuffer;
		}

		ID3D12DescriptorHeap* Get() const noexcept;

		//强制偏移描述符堆
		void ForceOffset(UINT count = 1) noexcept;

		CD3DX12_GPU_DESCRIPTOR_HANDLE GetOffest(UINT index) const noexcept;

		NO_COPY_AND_MOVE(CbvSrvUav)
	};
}
