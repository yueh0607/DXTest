#pragma once
#include "d3d12utils.h"

template<typename T>
class UploadBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
	//映射的指针
	BYTE* mMappedData = nullptr;
	//元素大小
	UINT mElementByteSize;
	//是否是常量缓冲区
	bool mIsConstantBuffer = false;

public:

	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer)
	{
		mIsConstantBuffer = isConstantBuffer;
		mElementByteSize = isConstantBuffer ? CalcConstantBufferByteSize(sizeof(T)) : sizeof(T);

		const HRESULT createResult = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // Upload heap
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(mUploadBuffer.GetAddressOf()));

		FailedErrorBox(L"Failed to create commited resouce.", createResult);

		const HRESULT mapResult = mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
		FailedErrorBox(L"Failed to map upload buffer.", mapResult);
	}

	UploadBuffer(const UploadBuffer& rhs) = delete;
	UploadBuffer& operator=(const UploadBuffer& rhs) = delete;


	~UploadBuffer()
	{
		if (mUploadBuffer != nullptr)
			mUploadBuffer->Unmap(0, nullptr);

		mMappedData = nullptr;
	}

	ID3D12Resource* Resource()const
	{
		return mUploadBuffer.Get();
	}

	//获取元素字节数
	UINT ElementByteSize() const noexcept
	{
		return mElementByteSize;
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
	}

};