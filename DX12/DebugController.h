#pragma once


#include <d3d12sdklayers.h>
#include <wrl/client.h>

#include "RenderStructure.h"


namespace FRenderer
{
	class DebugController : public RenderStructure
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> mDebugController;
	public:

		NO_COPY_AND_MOVE(DebugController)

			DebugController();
		~DebugController();

		ID3D12Debug* Get() const noexcept;

	};

}
