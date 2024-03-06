#pragma once
#include <d3d12.h>
#include "../DX12/RenderStructure.h"
#include "RendererInfo.h"
#include <map>

namespace FRenderer
{
	class RenderItem;

	
	class Renderer : RenderStructure
	{
		
	private:

		//渲染器初始化信息（渲染器不会变更这些信息）
		RenderInitInfo rii;
		//渲染器运行时信息(渲染器读写其中信息)
		RenderRuntimeInfo rri;
		//渲染项列表
		std::map<long,RenderItem*> renderItems;

		//重置大小时调用
		void OnResize(UINT width,UINT height);

		//初始化窗口
		void InitializeWindow(const RenderInitWindowInfo& info);
		//初始化Direct3D
		void InitializeDirect3D(const RenderInitDirect3DInfo& info);
		//初始化ImGui
		void InitializeImGui(const RenderInitImGuiInfo& info);
	public:
		Renderer();
		~Renderer();
		
		//初始化渲染器
		void InitializeRenderer(const RenderInitInfo info)
		{
			rii = info;
			InitializeWindow(info.windowInfo);
			InitializeDirect3D(info.d3d);
			InitializeImGui(info.guiInfo);
		}


		//渲染帧调用
		void OnDraw();
		void OnUpdate();

		void PreUpload();

		RenderRuntimeInfo* GetRRI();

		void EndUpload();


		//添加渲染项
		void AddRenderItem(RenderItem* renderItem);
		//移除渲染项
		void RemoveRenderItem(RenderItem* renderItem);
		//获取渲染项
		RenderItem* GetRenderItem(long guid);

		//禁用拷贝构造函数和赋值构造函数
		NO_COPY_AND_MOVE(Renderer)

	};
}
