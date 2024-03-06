#pragma once
#include <d3d12.h>
#include <d3dx12_root_signature.h>
#include <wrl/client.h>

#include "RenderStructure.h"



namespace FRenderer
{
	class Device;
	class RootSignature : public RenderStructure
	{
		//根签名
		Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootSignature;


	public:
		RootSignature(const Device* device,const CD3DX12_ROOT_PARAMETER* rootParameters);
		~RootSignature();

		//获取根签名
		ID3D12RootSignature* Get() const noexcept;

		NO_COPY_AND_MOVE(RootSignature)

	};
}
