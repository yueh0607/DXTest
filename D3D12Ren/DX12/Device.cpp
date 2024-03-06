#include"Device.h"
#include "d3d12utils.h"
#include "FrameResource.h"
#include "Factory.h"
#include "Adapter.h"
#include "iostream"
namespace FRenderer
{

	Device::Device(const Factory* factory)
		:mFactory(factory)
	{
		//创建适配器
		mAdapter = new Adapter();

		//创建Device
		const HRESULT deviceResult = D3D12CreateDevice(
			nullptr,
			D3D_FEATURE_LEVEL_12_0,
			IID_PPV_ARGS(mDevice.GetAddressOf()));
		

		//如果DX12设备创建失败，则回退到WARP适配器（WARP与硬件无关）
		if (FAILED(deviceResult))
		{
			std::cout << "默认设备创建失败";
			//获取WARP适配器
			factory->EnumWarpAdapter(const_cast<Adapter*>(mAdapter));
			//再次尝试创建设备
			const HRESULT warpResult = D3D12CreateDevice(
				mAdapter->Get(),
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(mDevice.GetAddressOf()));
			
			FailedErrorBox(L"Failed to create DX12 device and rollback WARP device", warpResult);
		}
		
	}
	Device::~Device() = default;

	const Factory* Device::GetFactory() const noexcept
	{
		return mFactory;
	}

	const Adapter* Device::GetAdapter() const noexcept
	{
		return mAdapter;
	}

	ID3D12Device* Device::Get() const noexcept
	{
		return mDevice.Get();
	}


	
	
}
