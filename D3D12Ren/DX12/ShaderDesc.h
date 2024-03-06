#pragma once
#include <d3d12.h>
#include <wrl/client.h>

namespace FRenderer
{
	//着色描述
	struct ShaderDesc 
	{
		//顶点着色器(VS,Vertex Shader)
		Microsoft::WRL::ComPtr<ID3DBlob> mVertexShader;
		//外壳着色器(HS,Hull Shader)
		Microsoft::WRL::ComPtr<ID3DBlob> mHullShader;
		//域着色器(DS,Domain Shader)
		Microsoft::WRL::ComPtr<ID3DBlob> mDomainShader;
		//几何着色器(GS,Geometry Shader)
		Microsoft::WRL::ComPtr<ID3DBlob> mGeometryShader;
		//像素着色器(PS,Pixel Shader)
		Microsoft::WRL::ComPtr<ID3DBlob> mPixelShader;


	};
}
