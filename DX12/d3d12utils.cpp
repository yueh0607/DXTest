#pragma once

#include "d3d12utils.h"

#include "Vertex.h"
#include <cstdint>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>
#include <wrl.h>

#include "CommandList.h"
#include "Device.h"
#include "MeshGeometry.h"



using namespace DirectX;


using namespace Microsoft::WRL;


std::shared_ptr<MeshGeometry> BuildBoxGeometry(ID3D12Device* device, FRenderer::CommandList* cmdList , std::string name, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices)
{

	std::shared_ptr<MeshGeometry> mBoxGeo = std::make_shared<MeshGeometry>();
	mBoxGeo->Name = name;

	
	//计算逆时针法线
	CalNormal(vertices, indices);

	//缓冲区大小
	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::uint16_t);


	HRESULT vResult = D3DCreateBlob(vbByteSize, &mBoxGeo->VertexBufferCPU);
	FailedErrorBox(L"Failed to create vertex buffer.", vResult);
	CopyMemory(mBoxGeo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);
	HRESULT iResult = D3DCreateBlob(ibByteSize, &mBoxGeo->IndexBufferCPU);
	FailedErrorBox(L"Failed to create index buffer.", iResult);
	CopyMemory(mBoxGeo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	mBoxGeo->VertexBufferGPU = CreateDefaultBuffer(device,
		cmdList->Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = CreateDefaultBuffer(device,
		cmdList->Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;


	return mBoxGeo;
}


std::shared_ptr<MeshGeometry> BuildBox(ID3D12Device* device, FRenderer::CommandList* cmdList, std::string name, DirectX::XMFLOAT3 offestVertex)
{
	std::vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
	};
	OffestVertex(vertices, offestVertex);
	std::vector<uint16_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	return BuildBoxGeometry(device,cmdList, name, vertices, indices);
}


