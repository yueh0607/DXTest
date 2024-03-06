#pragma once

#include <wrl/client.h>
#include <d3d12.h>
#include "RenderStructure.h"


namespace FRenderer
{
	class Factory;
	class Adapter;

	/// <summary>
	/// 绘图设备
	/// </summary>
	class Device : public RenderStructure
	{
		/// <summary>
		/// 绘图设备
		/// </summary>
		Microsoft::WRL::ComPtr<ID3D12Device> mDevice;
		//API工厂
		const Factory* mFactory;
		//适配器
		const Adapter* mAdapter;


	public:
		//禁用拷贝构造函数和赋值构造函数	
		NO_COPY(Device)


		Device(const Factory* factory);
		~Device();


		const Factory* GetFactory() const noexcept;

		const Adapter* GetAdapter() const noexcept;

		ID3D12Device* Get() const noexcept;

	};
}
