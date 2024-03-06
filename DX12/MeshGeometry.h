#pragma once
#include<string>
#include<d3d12.h>
#include<d3dx12.h>
#include<wrl/client.h>
#include<unordered_map>
#include"SubMeshGeometry.h"

struct MeshGeometry
{

private:
	D3D12_INDEX_BUFFER_VIEW* indexBufferView;
	D3D12_VERTEX_BUFFER_VIEW* vertexBufferView;
public:
	// 可以按名字查
	std::string Name;

	//系统内存副本 使用Blob，因为顶点/索引格式可以是通用的。
	//取决于客户是否进行适当的处理。
	Microsoft::WRL::ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;

	// 关于缓冲区的数据
	UINT VertexByteStride = 0;
	UINT VertexBufferByteSize = 0;
	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
	UINT IndexBufferByteSize = 0;

	//MeshGeometry可以在一个顶点/索引缓冲区中存储多个几何体。
	//使用此容器定义子网格几何图形，以便我们可以绘制
	//单独的子网格。
	std::unordered_map<std::string, SubmeshGeometry> DrawArgs;


	MeshGeometry()
	{
		indexBufferView = new D3D12_INDEX_BUFFER_VIEW();
		vertexBufferView = new D3D12_VERTEX_BUFFER_VIEW();
	}
	~MeshGeometry()
	{
		delete indexBufferView;
		delete vertexBufferView;
	}

	/// <summary>
	/// 获取顶点缓冲区视图
	/// </summary>
	/// <returns></returns>
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes = VertexByteStride;
		vbv.SizeInBytes = VertexBufferByteSize;

		return vbv;
	}

	/// <summary>
	/// 获取索引缓冲区视图
	/// </summary>
	/// <returns></returns>
	D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
	{
		D3D12_INDEX_BUFFER_VIEW ibv;
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format = IndexFormat;
		ibv.SizeInBytes = IndexBufferByteSize;

		return ibv;
	}

	/// <summary>
	/// 顶点总数
	/// </summary>
	/// <returns></returns>
	UINT VertexCount()const
	{
		return VertexBufferByteSize / VertexByteStride;
	}
	/// <summary>
	/// 索引总数
	/// </summary>
	/// <returns></returns>
	UINT IndexCount() const
	{
		return IndexBufferByteSize / (IndexFormat == DXGI_FORMAT_R16_UINT ? sizeof(UINT16) : sizeof(UINT32));
	}

	//我们可以在上传到GPU后释放这些内存。
	void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader = nullptr;
	}



};