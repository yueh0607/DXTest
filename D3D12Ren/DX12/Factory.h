#pragma once

#include <dxgi1_4.h>
#include <wrl/client.h>

#include "RenderStructure.h"

namespace FRenderer
{
	class Adapter;
	class Factory
	{
		Microsoft::WRL::ComPtr<IDXGIFactory4> mFactory;

	public :
		NO_COPY_AND_MOVE(Factory)


		Factory();
		~Factory();

		//获取API工厂
		IDXGIFactory1* Get() const noexcept;

		//枚举WARP适配器
		void EnumWarpAdapter(Adapter* adapter) const;
	};
}
