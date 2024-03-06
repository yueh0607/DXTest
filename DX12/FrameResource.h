#pragma once
#include <vector>

#include "MathHelper.h"
#include "UploadBuffer.h"

#include "RenderStructure.h"
#include "Fence.h"

namespace FRenderer
{
	class CommandAllocator;

	//对象常量
	struct ObjectConstants
	{
		DirectX::XMFLOAT4X4 worldProj = MathHelper::Identity4x4();
	};

	//帧资源
	class FrameResource : public RenderStructure, public IFenceValueHandle
	{
		//当前帧的fence
		UINT64 mFenceValue = 0;
		//命令列表分配器
		CommandAllocator* mCmdListAlloc;
		
	public:

		FrameResource(const Device* device,Fence* fence);

		~FrameResource();


		//对象常量缓冲区
		std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectConstantBuffer = nullptr;

		

		CommandAllocator* GetCmdListAlloc() const noexcept;
		//设置围栏值
		void SetFenceValue(UINT value) noexcept override;
		//获取围栏值
		UINT GetFenceValue() const noexcept override;

		NO_COPY_AND_MOVE(FrameResource)
	};


}
