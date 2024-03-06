#pragma once

#include <d3d12.h>
#include <d3dcommon.h>
#include <D3Dcompiler.h>
#include <d3dx12_barriers.h>
#include <d3dx12_core.h>
#include <d3dx12_resource_helpers.h>
#include <DirectXMath.h>
#include <memory>
#include <string>
#include <vector>

#include "DepthStencilView.h"
#include "MathHelper.h"
#include "Vertex.h"
#include "wrl/client.h"


#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;
struct Vertex;
class MeshGeometry;
class FrameResource;
class FrameResourceManager;


//合并n个LPCWSTR为wstring
inline std::wstring MergeLPCWSTR(int n, LPCWSTR strings[])
{
	std::wstring result;

	for (int i = 0; i < n; ++i)
	{
		result += strings[i];
	}

	return result;
}


inline void ErrorBox(LPCWSTR title, LPCWSTR info)
{
	MessageBox(nullptr, info, title, MB_OK | MB_ICONERROR);
	exit(1);
}
inline void ErrorBox(LPCWSTR title, LPCWSTR info, HRESULT result)
{
	LPWSTR errorText = nullptr;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&errorText,
		0,
		NULL
	);
	LPCWSTR strs[3] = { info, L"\n", errorText };
	ErrorBox(title, MergeLPCWSTR(3, strs).c_str());
}
inline void ErrorBox(LPCWSTR info, HRESULT result)
{
	ErrorBox(L"Error", info, result);
}

inline void FailedErrorBox(LPCWSTR info, HRESULT result)
{
	if (FAILED(result))
		ErrorBox(info, result);
}

inline void NullAssertBox(void* obj)
{
	if (obj == nullptr)
		ErrorBox(L"Error", L"NullReference");
}



/// <summary>
/// 计算常量缓冲区大小
/// </summary>
/// <param name="byteSize"></param>
/// <returns></returns>
inline UINT CalcConstantBufferByteSize(UINT byteSize)
{
	// Constant buffers must be a multiple of the minimum hardware
	// allocation size (usually 256 bytes).  So round up to nearest
	// multiple of 256.  We do this by adding 255 and then masking off
	// the lower 2 bytes which store all bits < 256.
	// Example: Suppose byteSize = 300.
	// (300 + 255) & ~255
	// 555 & ~255
	// 0x022B & ~0x00ff
	// 0x022B & 0xff00
	// 0x0200
	// 512
	return (byteSize + 255) & ~255;
}


/// <summary>
/// 编译信息
/// </summary>
struct CompileInfo
{
	bool succeed;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
};





/// <summary>
/// 编译信息Blob翻译为wstring
/// </summary>
/// <param name="errorBlob"></param>
/// <returns></returns>
inline std::wstring ParseErrorBlobInfo(const Microsoft::WRL::ComPtr<ID3DBlob>& errorBlob)
{
	if (errorBlob)
	{
		const char* ansiErrors = reinterpret_cast<const char*>(errorBlob->GetBufferPointer());

		// 获取错误信息的长度
		int length = MultiByteToWideChar(CP_ACP, 0, ansiErrors, -1, nullptr, 0);

		// 创建 std::wstring 来存储宽字符错误信息
		std::wstring compileErrors(length, L'\0');

		// 将 ANSI 编码的错误信息转换为宽字符
		MultiByteToWideChar(CP_ACP, 0, ansiErrors, -1, &compileErrors[0], length);

		return compileErrors;
	}
	else
	{
		// 如果没有错误信息，返回一个空字符串
		return L"";
	}
}

/// <summary>
/// 编译着色器
/// </summary>
/// <param name="path"></param>
/// <param name="entryFuncName"></param>
/// <param name="target"></param>
/// <param name="include"></param>
/// <returns></returns>
inline CompileInfo CompileShader(LPCWSTR path, LPCSTR entryFuncName, LPCSTR target, ID3DInclude* includes = nullptr)
{
	CompileInfo info;

	HRESULT shaderResult = D3DCompileFromFile(
		path,
		nullptr,
		includes,
		entryFuncName,
		target,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		info.shaderBlob.GetAddressOf(),
		info.errorBlob.GetAddressOf());

	info.succeed = SUCCEEDED(shaderResult);
	if (!info.succeed)
	{
		std::wstring errorInfo = ParseErrorBlobInfo(info.errorBlob);
		ErrorBox(L"Shader Compile Error", errorInfo.c_str());
	}
	return info;
}


/// <summary>
/// 创建默认缓冲区
/// </summary>
/// <param name="mDevice"></param>
/// <param name="cmdList"></param>
/// <param name="initData"></param>
/// <param name="byteSize"></param>
/// <param name="uploadBuffer"></param>
/// <returns></returns>
inline Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* cmdList,
	const void* initData,
	UINT64 byteSize,
	Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
	ComPtr<ID3D12Resource> defaultBuffer;

	// Create the actual default buffer resource.
	HRESULT createResult = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(defaultBuffer.GetAddressOf()));
	FailedErrorBox(L"Failed to create commited resouce.", createResult);

	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap. 
	HRESULT createResult2 = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(uploadBuffer.GetAddressOf()));
	FailedErrorBox(L"Failed to create commited resouce.", createResult2);


	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(cmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.


	return defaultBuffer;
}



/// <summary>
/// 计算逆时针法线
/// </summary>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <returns></returns>
inline DirectX::XMVECTOR ComputeNormal(DirectX::FXMVECTOR p0, DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2)
{
	using namespace DirectX;
	XMVECTOR u = p1 - p0;
	XMVECTOR v = p2 - p0;

	return DirectX::XMVector3Normalize(DirectX::XMVector3Cross(u, v));

}

/// <summary>
/// 批量计算逆时针法线
/// </summary>
/// <param name="vertices"></param>
/// <param name="indices"></param>
inline void CalNormal(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices)
{
	//计算顶点法线
	UINT triangleCount = (UINT)indices.size() / 3;
	for (UINT i = 0; i < triangleCount; i++)
	{
		//取出三个顶点
		UINT i0 = indices[i * 3 + 0];
		UINT i1 = indices[i * 3 + 1];
		UINT i2 = indices[i * 3 + 2];


		//计算法向量
		DirectX::XMVECTOR faceNormal = ComputeNormal(
			DirectX::XMLoadFloat3(&vertices[i0].Pos),
			DirectX::XMLoadFloat3(&vertices[i1].Pos),
			DirectX::XMLoadFloat3(&vertices[i2].Pos)
		);

		//转FLOAT3
		DirectX::XMFLOAT3 normal;
		XMStoreFloat3(&normal, faceNormal);
		//累加到顶点上即为法线方向
		MathHelper::AddEqual(vertices[i0].Normal, normal);
		MathHelper::AddEqual(vertices[i1].Normal, normal);
		MathHelper::AddEqual(vertices[i2].Normal, normal);

	}


	//法向量归一化
	for (UINT i = 0; i < vertices.size(); i++)
	{
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(XMLoadFloat3(&vertices[i].Normal));
		XMStoreFloat3(&vertices[i].Normal, n);
	}

}


/// <summary>
/// 对顶点进行批量偏移
/// </summary>
/// <param name="vertices"></param>
/// <param name="offestVertex"></param>
inline void OffestVertex(std::vector<Vertex>& vertices, DirectX::XMFLOAT3 offestVertex)
{
	//偏移
	for (Vertex& v : vertices)
	{
		DirectX::XMStoreFloat3(&v.Pos, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&v.Pos), DirectX::XMLoadFloat3(&offestVertex)));
	}
}


/// <summary>
/// 反转全部三角形
/// </summary>
/// <param name="indices"></param>
inline void ReverseAllTriangle(std::vector<uint16_t> indices)
{
	if (indices.size() % 3 != 0) ErrorBox(L"Error", L"indices.size()/3!=0");
	uint16_t temp;
	for (int i = 0; i < indices.size(); i += 3)
	{
		temp = indices[i + 0];
		indices[i + 0] = indices[i + 2];
		indices[i + 2] = temp;
	}
}




std::shared_ptr<MeshGeometry> BuildBoxGeometry(ID3D12Device* device, FRenderer::CommandList* cmdList, std::string name, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);


std::shared_ptr<MeshGeometry> BuildBox(ID3D12Device* device, FRenderer::CommandList* cmdList, std::string name, DirectX::XMFLOAT3 offestVertex);

