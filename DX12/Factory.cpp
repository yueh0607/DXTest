#include "Factory.h"
#include "d3d12utils.h"
#include "Adapter.h"

namespace FRenderer
{
	Factory::Factory()
	{
#if _DEBUG||DEBUG
		const HRESULT factoryResult = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(mFactory.GetAddressOf()));
#else
		const HRESULT factoryResult = CreateDXGIFactory2(0, IID_PPV_ARGS(mFactory.GetAddressOf()));
#endif

		FailedErrorBox(L"Failed to create DXGI Factory1", factoryResult);
	}
	Factory::~Factory() = default;
	IDXGIFactory1* Factory::Get() const noexcept
	{
		return mFactory.Get();
	}
	void Factory::EnumWarpAdapter(Adapter* adapter) const
	{
		mFactory->EnumWarpAdapter(
			IID_PPV_ARGS(
				const_cast<IDXGIAdapter**>(adapter->GetAddressOf())));
	}

}


