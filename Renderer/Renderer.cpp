#include "Renderer.h"
#include "../DX12/d3d12utils.h"
#if _DEBUG||DEBUG
#include "../DX12/DebugController.h"
#endif
#include "../DX12/Factory.h"
#include "../DX12/Adapter.h"
#include "../DX12/Device.h"
#include "../DX12/CommandQueue.h"
#include "../DX12/CommandList.h"
#include "../DX12/SwapChain.h"
#include "../DX12/DepthStencilView.h"
#include "../DX12/RenderTargetView.h"
#include "../DX12/CommandAllocator.h"
#include "../DX12/RenderItem.h"
#include "../DX12/FrameResource.h"
#include "../DX12/RootSignature.h"
#include "../DX12/PipelineStateObject.h"
#include "../DX12/ShaderDesc.h"
#include "../DX12/CbvSrvUav.h"
#include "../DX12/ImGuiUtils/ImGuiUtils.h"
#include "../DX12/Model.h"
namespace FRenderer
{
	void Renderer::OnResize(UINT width, UINT height)
	{
		//等待命令执行完成
		rri.d3d.cmdQueue->Flush(rri.d3d.currentFrame);
		//重置命令列表为OPEN
		rri.d3d.cmdList->Reset();

		//修改运行时信息
		rri.win.mWidth = width;
		rri.win.mHeight = height;

		//调整DSV大小
		rri.d3d.dsv->Resize(rri.d3d.cmdList, width, height);
		//调整交换链缓冲区大小
		rri.d3d.swapChain->ResizeBuffer(width, height);
		//调整RTV大小
		rri.d3d.rtv->SetRenderTargetBuffers(rri.d3d.swapChain);

		//闭合命令列表
		rri.d3d.cmdList->Close();
		//执行命令
		rri.d3d.cmdQueue->Execute(rri.d3d.cmdList);
		//等待命令执行完成
		rri.d3d.cmdQueue->Flush(rri.d3d.currentFrame);

	}

	void Renderer::InitializeWindow(const RenderInitWindowInfo& info)
	{
		//配置渲染器运行时信息
		rri.win.mWidth = static_cast<UINT>(info.windowWidth);
		rri.win.mHeight = static_cast<UINT>(info.windowHeight);

		//定义窗口类
		WNDCLASS wc;
		wc.style = info.styles.styleWindow;
		wc.lpfnWndProc = info.wndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = info.hInstance;
		wc.hIcon = LoadIcon(info.hInstance, info.styles.iconName);
		wc.hCursor = LoadCursor(info.hInstance, info.styles.cursorName);
		wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = info.windowClassName.c_str();

		//注册窗口类
		if (!RegisterClass(&wc))
			ErrorBox(L"Error", L"Failed to register win class.");

		//窗口矩形
		RECT windowRect;
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = info.windowWidth;
		windowRect.bottom = info.windowHeight;
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

		//创建窗口
		rri.win.mHWND = CreateWindow(wc.lpszClassName, info.windowName.c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			info.windowWidth, info.windowHeight, 0, 0, info.hInstance, 0);
		//检查句柄情况
		if (!rri.win.mHWND)
			ErrorBox(L"Error", L"Failed to create win");

		//在第一次时更新窗口及时响应
		ShowWindow(rri.win.mHWND, SW_SHOW);
		UpdateWindow(rri.win.mHWND);
	}
	void Renderer::InitializeDirect3D(const RenderInitDirect3DInfo& info)
	{
		//设置视口信息
		rri.d3d.mScreenViewport.TopLeftX = 0;
		rri.d3d.mScreenViewport.TopLeftY = 0;
		rri.d3d.mScreenViewport.Width = static_cast<float>(rri.win.mWidth);
		rri.d3d.mScreenViewport.Height = static_cast<float>(rri.win.mHeight);
		rri.d3d.mScreenViewport.MinDepth = 0.0f;
		rri.d3d.mScreenViewport.MaxDepth = 1.0f;
		//设置裁剪矩形
		rri.d3d.mScissorRect = { 0, 0,static_cast<LONG>(rri.win.mWidth), static_cast<LONG>(rri.win.mHeight) };

#if _DEBUG||DEBUG
		//开启调试层
		rri.d3d.debugController = new DebugController();
#endif
		//创建API工厂
		rri.d3d.factory = new Factory();
		//创建设备
		rri.d3d.device = new Device(rri.d3d.factory);
		//创建同步围栏
		rri.d3d.fence = new Fence(rri.d3d.device);
		//创建命令队列
		rri.d3d.cmdQueue = new CommandQueue(rri.d3d.device);
		//创建交换链
		rri.d3d.swapChain = new SwapChain(rri.d3d.device, rri.d3d.cmdQueue, rri.win.mHWND, rri.win.mWidth, rri.win.mHeight);
		//创建DSV
		rri.d3d.dsv = new DepthStencilView(rri.d3d.device, rri.win.mWidth, rri.win.mHeight);
		//创建RTV
		rri.d3d.rtv = new RenderTargetView(rri.d3d.device, rri.d3d.swapChain->GetBufferCount());
		//rri.d3d.rtv->SetRenderTargetBuffers(rri.d3d.swapChain->GetBufferCount(), rri.d3d.swapChain->GetBufferFormat(), rri.win.mWidth, rri.win.mHeight);
		//创建帧资源
		rri.d3d.currentFrame = new FrameResource(rri.d3d.device, rri.d3d.fence);

		//创建命令列表
		rri.d3d.cmdList = new CommandList(rri.d3d.device, rri.d3d.currentFrame->GetCmdListAlloc());



		//创建CbvSrvUav
		rri.d3d.cbvSrvUav = new CbvSrvUav(rri.d3d.device, 2);
		//为帧资源创建对象常量
		rri.d3d.currentFrame->mObjectConstantBuffer = rri.d3d.cbvSrvUav->CreateConstantBufferView<ObjectConstants>(rri.d3d.device, 1);


		//根参数
		CD3DX12_ROOT_PARAMETER parameters[1];

		CD3DX12_DESCRIPTOR_RANGE cbvTable;
		cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		parameters[0].InitAsDescriptorTable(1, &cbvTable);

		//创建根签名
		rri.d3d.rootSignature = new RootSignature(rri.d3d.device, parameters);



		//着色器资源
		ShaderDesc shaders;
		//编译顶点着色器
		shaders.mVertexShader = CompileShader(L"X:\\Project\\VC\\DX12R3D\\DX12R3D\\Shaders\\color.hlsl", "VS", "vs_5_0").shaderBlob;
		//编译像素着色器
		shaders.mPixelShader = CompileShader(L"X:\\Project\\VC\\DX12R3D\\DX12R3D\\Shaders\\color.hlsl", "PS", "ps_5_0").shaderBlob;

		//顶点输入布局
		D3D12_INPUT_ELEMENT_DESC mInputLayout[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(Vertex,Pos),D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,offsetof(Vertex,Color),D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,offsetof(Vertex,Normal),D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
		};

		//创建管线状态对象
		rri.d3d.pso = new PipelineStateObject(
			rri.d3d.device,
			rri.d3d.rootSignature,
			&shaders,
			mInputLayout,
			_countof(mInputLayout),
			rri.d3d.dsv->GetFormat(),
			rri.d3d.swapChain->GetBufferFormat());


		OnResize(rri.win.mWidth, rri.win.mHeight);

	}
	void Renderer::InitializeImGui(const RenderInitImGuiInfo& info)
	{
		//初始化IMGUI
		bool result = true;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		//取CPU地址
		const auto descriptorAddressCPU = rri.d3d.cbvSrvUav->Get()->GetCPUDescriptorHandleForHeapStart();
		//取GPU地址
		const auto descriptorAddressGPU = rri.d3d.cbvSrvUav->Get()->GetGPUDescriptorHandleForHeapStart();
		//地址偏移
		//descriptorAddressCPU.ptr += DescriptorHeapIndices::Font;
		//descriptorAddressGPU.ptr += DescriptorHeapIndices::Font;

		result &= ImGui_ImplWin32_Init(rri.win.mHWND);
		result &= ImGui_ImplDX12_Init(rri.d3d.device->Get(),
			static_cast<int>(rri.d3d.rtv->GetDescriptorCount()), rri.d3d.rtv->GetBufferFormat(), rri.d3d.cbvSrvUav->Get(),
			descriptorAddressCPU, descriptorAddressGPU);

		assert(result);
	}
	Renderer::Renderer()
	{

	}
	Renderer::~Renderer()
	{

		//结束ImGui
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void Renderer::OnDraw()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();


		//重置命令分配器
		rri.d3d.currentFrame->GetCmdListAlloc()->Reset();
		//重新绑定命令分配器
		rri.d3d.cmdList->SetCommandAllocator(rri.d3d.currentFrame->GetCmdListAlloc());
		//重置命令列表
		rri.d3d.cmdList->Reset(rri.d3d.pso);


		//设置视口和裁剪矩形
		rri.d3d.cmdList->Get()->RSSetViewports(1, &rri.d3d.mScreenViewport);
		rri.d3d.cmdList->Get()->RSSetScissorRects(1, &rri.d3d.mScissorRect);


		ID3D12Resource* renderTargetBackBuffer = rri.d3d.rtv->GetBuffer(rri.d3d.swapChain->GetCurrentBackBufferIndex());
		const D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = rri.d3d.rtv->Get(rri.d3d.swapChain->GetCurrentBackBufferIndex());

		//RT切换为渲染目标
		rri.d3d.cmdList->ResourceBarrier(
			renderTargetBackBuffer,
			D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

		//清除深度和模板信息
		rri.d3d.cmdList->Get()->ClearDepthStencilView(rri.d3d.dsv->Get(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		//清除后缓冲区
		rri.d3d.cmdList->Get()->ClearRenderTargetView(renderTargetView, rii.d3d.mClearColor, 0, nullptr);

		// 绑定DSV和RTV
		rri.d3d.cmdList->Get()->OMSetRenderTargets(1, &renderTargetView, true, &rri.d3d.dsv->Get());

		//绑定CBV描述符堆
		ID3D12DescriptorHeap* descriptorHeaps[] = { rri.d3d.cbvSrvUav->Get() };
		rri.d3d.cmdList->Get()->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
		//设置图元拓扑类型
		rri.d3d.cmdList->Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//绑定根签名
		rri.d3d.cmdList->Get()->SetGraphicsRootSignature(rri.d3d.rootSignature->Get());
		//绑定根描述符表
		rri.d3d.cmdList->Get()->SetGraphicsRootDescriptorTable(0,  rri.d3d.cbvSrvUav->GetOffest(1));

	

		for (auto& renderItem : renderItems)
		{

			auto& meshes = renderItem.second->model->meshes;
			for (auto& mesh : meshes)
			{
				//绑定顶点缓冲区视图
				rri.d3d.cmdList->Get()->IASetVertexBuffers(0, 1, &mesh->VertexBufferView());
				//绑定索引缓冲区视图
				rri.d3d.cmdList->Get()->IASetIndexBuffer(&mesh->IndexBufferView());

				//设置顶点/索引缓冲区
				for (auto& submesh : mesh->DrawArgs)
				{
					//绘制盒子
					rri.d3d.cmdList->Get()->DrawIndexedInstanced(
						submesh.second.IndexCount,
						1, submesh.second.StartIndexLocation, 0, 0);
				}
			}

		}
	/*	if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				if (ImGui::MenuItem("Open"))
				{

				}
				if (ImGui::MenuItem("Exit"))
				{
					PostQuitMessage(0);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}*/

		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), rri.d3d.cmdList->Get());

		//RT切换为呈现
		rri.d3d.cmdList->ResourceBarrier(
			renderTargetBackBuffer,
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		//关闭命令列表
		rri.d3d.cmdList->Close();
		//执行命令列表
		rri.d3d.cmdQueue->Execute(rri.d3d.cmdList);

		//呈现
		rri.d3d.swapChain->Present(false);

		//等待命令完成
		rri.d3d.cmdQueue->Flush(rri.d3d.currentFrame);

	}

	void Renderer::OnUpdate()
	{
		DirectX::XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
		DirectX::XMFLOAT4X4 mView = MathHelper::Identity4x4();
		DirectX::XMFLOAT4X4 mProj = MathHelper::Identity4x4();

		//确定一个测试数据
		float mRadius = 6;
		float mPhi = 1.2f * DirectX::XM_PI;
		float mTheta = DirectX::XM_PIDIV4;

		float x = mRadius * sinf(mPhi) * cosf(mTheta);
		float z = mRadius * sinf(mPhi) * sinf(mTheta);
		float y = mRadius * cosf(mPhi);


		DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
		DirectX::XMVECTOR target = DirectX::XMVectorZero();
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);


		DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);
		DirectX::XMStoreFloat4x4(&mView, view);


		DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&mWorld);
		DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&mProj);
		DirectX::XMMATRIX worldViewProj = world * view * proj;


		ObjectConstants objConstants;
		XMStoreFloat4x4(&objConstants.worldProj, XMMatrixTranspose(worldViewProj));
		rri.d3d.currentFrame->mObjectConstantBuffer->CopyData(0, objConstants);

	}

	void Renderer::PreUpload()
	{

		//重置命令分配器
		rri.d3d.currentFrame->GetCmdListAlloc()->Reset();
		//重新绑定命令分配器
		rri.d3d.cmdList->SetCommandAllocator(rri.d3d.currentFrame->GetCmdListAlloc());
		//重置命令列表
		rri.d3d.cmdList->Reset(rri.d3d.pso);
	}
	RenderRuntimeInfo* Renderer::GetRRI()
	{
		return &rri;
	}

	void Renderer::EndUpload()
	{
		//关闭命令列表
		rri.d3d.cmdList->Close();
		//执行命令列表
		rri.d3d.cmdQueue->Execute(rri.d3d.cmdList);

		//呈现
		rri.d3d.swapChain->Present(false);

		//等待命令完成
		rri.d3d.cmdQueue->Flush(rri.d3d.currentFrame);
	}

	void Renderer::AddRenderItem(RenderItem* renderItem)
	{
		renderItems.insert({ renderItem->GetGUID(), renderItem });
	}

	void Renderer::RemoveRenderItem(RenderItem* renderItem)
	{
		renderItems.erase(renderItem->GetGUID());
	}

	RenderItem* Renderer::GetRenderItem(long guid)
	{
		return renderItems[guid];
	}

}

