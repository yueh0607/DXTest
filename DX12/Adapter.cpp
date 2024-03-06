#include "Adapter.h"

namespace FRenderer
{
	Adapter::Adapter()
	{
		mAdapter = nullptr;
	}
	Adapter::~Adapter() = default;
	IDXGIAdapter* Adapter::Get() const noexcept
	{
		return mAdapter.Get();
	}

	IDXGIAdapter*const* Adapter::GetAddressOf() const noexcept
	{
		return mAdapter.GetAddressOf();
	}

}
