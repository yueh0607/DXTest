#pragma once
#include <dxgi.h>
#include <wrl/client.h>
#include "RenderStructure.h"

namespace FRenderer
{
	class Adapter: public RenderStructure
	{

		Microsoft::WRL::ComPtr<IDXGIAdapter> mAdapter;
	public:

		NO_COPY_AND_MOVE(Adapter)

		Adapter();
		~Adapter();

		IDXGIAdapter* Get() const noexcept;

		IDXGIAdapter*const* GetAddressOf() const noexcept;
	};
}
