#pragma once
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <intsafe.h>
#include <string>


namespace FRenderer
{


	//渲染器初始化窗口信息
	struct RenderInitWindowInfo
	{
		//窗口名
		std::wstring windowName;
		//窗口类名
		std::wstring windowClassName;
		//窗口宽度
		LONG windowWidth;
		//窗口高度
		LONG windowHeight;
		//进程实例句柄
		HINSTANCE hInstance;
		//窗口过程函数
		WNDPROC wndProc;

		struct WindowStyle
		{
			//样式
			UINT styleWindow = CS_HREDRAW | CS_VREDRAW;
			//缩略图
			LPCWSTR iconName =IDI_APPLICATION;
			//指针
			LPCWSTR cursorName = IDC_ARROW;
		} styles;

	};
	//渲染器初始化Direct3D信息
	struct RenderInitDirect3DInfo
	{
		DirectX::XMVECTORF32 mClearColor = DirectX::Colors::LightSteelBlue;
	};
	//渲染器初始化ImGui信息
	struct RenderInitImGuiInfo
	{

	};
	//渲染器初始化配置信息
	struct RenderInitInfo
	{
	public:
		RenderInitWindowInfo windowInfo;
		RenderInitDirect3DInfo d3d;
		RenderInitImGuiInfo guiInfo;
	};


	struct RenderWindowRuntimeInfo
	{
		//窗口句柄
		HWND mHWND;
		//窗口宽度
		UINT mWidth;
		//窗口高度
		UINT mHeight;
	};

	

	class Device;
	class Factory;
	class CommandQueue;
	class CommandList;
	class CommandAllocator;
	class SwapChain;
	class DepthStencilView;
	class RenderTargetView;
	class FrameResource;
	class Fence;
	class RootSignature;
	class PipelineStateObject;
	class CbvSrvUav;
#if _DEBUG||DEBUG
	class DebugController;
	
#endif
	struct RenderDirect3DRuntimeInfo
	{
#if _DEBUG||DEBUG

		DebugController* debugController;
#endif


		//API工厂
		Factory* factory;
		//设备
		Device* device;
		//围栏
		Fence* fence;
		//命令队列
		CommandQueue* cmdQueue;
		//命令列表
		CommandList* cmdList;
		//交换链
		SwapChain* swapChain;
		//DSV
		DepthStencilView* dsv;
		//RTV
		RenderTargetView * rtv;
		//当前帧资源
		FrameResource* currentFrame;
		//根签名
		RootSignature * rootSignature;
		//管线状态对象
		PipelineStateObject* pso;
		//CbvSrvUav
		CbvSrvUav* cbvSrvUav;

		//视口矩形
		D3D12_VIEWPORT mScreenViewport;
		//裁剪矩形
		D3D12_RECT mScissorRect;

	};

	struct RenderImGuiRuntimeInfo
	{
		
	};

	struct RenderRuntimeInfo
	{
		RenderWindowRuntimeInfo win;
		RenderDirect3DRuntimeInfo d3d;
		RenderImGuiRuntimeInfo gui;
	};

}
