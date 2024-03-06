#include "PipelineStateObject.h"

#include <d3dx12_core.h>

#include "d3d12utils.h"
#include "RootSignature.h"
#include "ShaderDesc.h"
namespace FRenderer
{
	PipelineStateObject::PipelineStateObject(Device* device, RootSignature* rootSignature, 
		ShaderDesc* shaders,
		D3D12_INPUT_ELEMENT_DESC* vertexInputLayout, 
		UINT inputLayoutNum,
		DXGI_FORMAT dsvFormat,
		DXGI_FORMAT rtvFormat)
	{
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
		//置0内存
		ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		//设置顶点输入布局
		psoDesc.InputLayout = { vertexInputLayout, inputLayoutNum };
		//绑定根签名
		psoDesc.pRootSignature = rootSignature->Get();

		//设置着色器
		if (shaders->mPixelShader == nullptr) 
			ErrorBox(L"Error", L"Vertex Shader cannot be nullptr.");
		psoDesc.VS =
		{
			static_cast<BYTE*>(shaders->mVertexShader->GetBufferPointer()),
			shaders->mVertexShader->GetBufferSize()
		};
		if (shaders->mHullShader != nullptr)
			psoDesc.HS =
		{
			static_cast<BYTE*>(shaders->mHullShader->GetBufferPointer()),
			shaders->mHullShader->GetBufferSize()
		};
		if (shaders->mDomainShader != nullptr)
			psoDesc.DS =
		{
					static_cast<BYTE*>(shaders->mDomainShader->GetBufferPointer()),
					shaders->mDomainShader->GetBufferSize()
		};
		if (shaders->mGeometryShader != nullptr)
			psoDesc.GS =
		{
			static_cast<BYTE*>(shaders->mGeometryShader->GetBufferPointer()),
			shaders->mGeometryShader->GetBufferSize()
		};
		if (shaders->mPixelShader == nullptr) 
			ErrorBox(L"Error", L"Pixel Shader cannot be nullptr.");
		psoDesc.PS =
		{
					static_cast<BYTE*>(shaders->mPixelShader->GetBufferPointer()),
					shaders->mPixelShader->GetBufferSize()
		};

		CD3DX12_RASTERIZER_DESC rasterizerDesc(D3D12_DEFAULT);
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE; // 禁用剔除

		psoDesc.RasterizerState = rasterizerDesc;
		//设置混合状态
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		//设置深度模板状态
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		//采样遮罩
		psoDesc.SampleMask = UINT_MAX;
		//图元拓扑类型
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//渲染目标数量
		psoDesc.NumRenderTargets = 1;
		//RTV格式
		psoDesc.RTVFormats[0] = rtvFormat;
		//DSV格式
		psoDesc.DSVFormat = dsvFormat;
		//MSAA禁用
		psoDesc.SampleDesc.Count = 1;
		psoDesc.SampleDesc.Quality = 0;

		//创建管线状态对象
		const HRESULT createResult = device->Get()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(mPSO.GetAddressOf()));
		FailedErrorBox(L"Failed to create graphics pipeline state.", createResult);
	}


	PipelineStateObject::~PipelineStateObject() = default;

	ID3D12PipelineState* PipelineStateObject::Get() const noexcept
	{
		return mPSO.Get();
	}



}

