#include<iostream>
#include"../Renderer/Renderer.h"
#include"../DX12/Model.h"
#include "../DX12/RenderItem.h"
#include "../DX12/MeshGeometry.h"
#include <memory>
#include <windowsx.h>

#include "../DX12/CommandList.h"
#include "../DX12/CommandQueue.h"
#include "../DX12/d3d12utils.h"
#include "../DX12/ImGuiUtils/ImGuiUtils.h"
#include "../Renderer/RendererInfo.h"
using namespace std;
using namespace FRenderer;
using namespace DirectX;

LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{

	
		//窗口实例销毁
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		//按Alt+Enter不会触发蜂鸣
	case WM_MENUCHAR:
		//ImGuiWantCatch
		return MAKELRESULT(0, MNC_CLOSE);

		//最小化
	case WM_GETMINMAXINFO:
		//ImGuiWantCatch
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

}


int main()
{
	RenderInitInfo rii{};
	rii.windowInfo.windowName = L"Test";
	rii.windowInfo.windowClassName= L"Test";
	rii.windowInfo.windowWidth = 800;
	rii.windowInfo.windowHeight = 600;
	rii.windowInfo.hInstance = GetModuleHandle(NULL);
	rii.windowInfo.wndProc = MsgProc;

	rii.d3d.mClearColor = DirectX::Colors::LightSteelBlue;

	Renderer* rd = new Renderer();
	rd->InitializeRenderer(rii);
	
	RenderItem *item = new RenderItem();
	rd->AddRenderItem(item);
	Model* model = new Model();
	item->model = model;
	auto rri = rd->GetRRI();
	rd->PreUpload();


	std::vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::White) }),
		Vertex({ XMFLOAT3(-1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Black) }),
		Vertex({ XMFLOAT3(+1.0f, 0.0f, +1.0f), XMFLOAT4(Colors::Red) }),
		Vertex({ XMFLOAT3(+1.0f, 0.0f, -1.0f), XMFLOAT4(Colors::Green) })
	};
	
	std::vector<uint16_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3
	};
	ReverseAllTriangle(indices);

	std::shared_ptr<MeshGeometry> mBoxGeo = std::make_shared<MeshGeometry>();
	mBoxGeo->Name = "MyBox";


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

	mBoxGeo->VertexBufferGPU = CreateDefaultBuffer(rri->d3d.device->Get(),
		rri->d3d.cmdList->Get(), vertices.data(), vbByteSize, mBoxGeo->VertexBufferUploader);

	mBoxGeo->IndexBufferGPU = CreateDefaultBuffer(rri->d3d.device->Get(),
		rri->d3d.cmdList->Get(), indices.data(), ibByteSize, mBoxGeo->IndexBufferUploader);

	mBoxGeo->VertexByteStride = sizeof(Vertex);
	mBoxGeo->VertexBufferByteSize = vbByteSize;
	mBoxGeo->IndexFormat = DXGI_FORMAT_R16_UINT;
	mBoxGeo->IndexBufferByteSize = ibByteSize;

	rd->EndUpload();
	model->meshes.push_back(mBoxGeo);


	mBoxGeo->DrawArgs["box"] = SubmeshGeometry{ mBoxGeo->IndexCount(),0,0 };

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//更新常量
		rd->OnUpdate();
		//添加命令
		rd->OnDraw();


		Sleep(33);
	}

	return 0;
}