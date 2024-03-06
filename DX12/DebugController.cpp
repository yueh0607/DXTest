#include "DebugController.h"

#include "d3d12utils.h"
#include "iostream"
namespace FRenderer
{
	DebugController::DebugController()
	{
		//开启调试层，此行为建议仅在DEBUG模式使用，信息详见VisualStudio/调试/输出
		const HRESULT debugResult = D3D12GetDebugInterface(IID_PPV_ARGS(mDebugController.GetAddressOf()));
		mDebugController->EnableDebugLayer();
		FailedErrorBox(L"Failed to enable debug layer", debugResult);
		std::cout << "Debug layer enabled" << std::endl;
	}

	DebugController::~DebugController() = default;

	ID3D12Debug* DebugController::Get() const noexcept
	{
		return mDebugController.Get();
	}



}
