#include "RootSignature.h"
#include "d3d12utils.h"


namespace FRenderer
{
	RootSignature::RootSignature(const Device* device,const CD3DX12_ROOT_PARAMETER* rootParameters)
	{
		//根签名由一组根参数组成。
		CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, rootParameters,
			0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		//创建根签名序列化对象。
		Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		const HRESULT serializeResult = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

		FailedErrorBox(L"Failed to serialize root signature.", serializeResult);
		//打印错误信息
		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}

		//创建根签名
		const HRESULT createResult = device->Get()->CreateRootSignature(
			0,
			serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature));

		FailedErrorBox(L"Failed to create root signature.", createResult);
	}

	RootSignature::~RootSignature() = default;

	ID3D12RootSignature* RootSignature::Get() const noexcept
	{
		return mRootSignature.Get();
	}

}

