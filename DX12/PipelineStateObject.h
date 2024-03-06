#pragma once
#include <d3d12.h>
#include <wrl/client.h>

#include "RenderStructure.h"


namespace FRenderer
{
	class Device;
	class RootSignature;
	struct ShaderDesc;
	class PipelineStateObject : public RenderStructure
	{
		Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO;


		public:
	
		PipelineStateObject(Device* device, RootSignature* rootSignature, ShaderDesc* shaders, D3D12_INPUT_ELEMENT_DESC* vertexInputLayout, UINT inputLayoutNum, DXGI_FORMAT dsvFormat, DXGI_FORMAT rtvFormat);
		~PipelineStateObject();

		ID3D12PipelineState* Get() const noexcept;
		//禁用移动和拷贝
		NO_COPY_AND_MOVE(PipelineStateObject)
	};
}
