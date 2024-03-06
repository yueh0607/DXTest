#pragma once


#include"../ImGui/lib/imgui.h";
#include"../ImGui/lib/imgui_impl_win32.h";
#include"../ImGui/lib/imgui_impl_dx12.h";
#include <wtypes.h>
#include<wrl/client.h>
#include<d3dx12.h>





static bool InitializeImGui(HWND hwnd, ID3D12Device* device, int mSwapChainBufferCount, 
	DXGI_FORMAT mBackBufferFormat,Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap)
{
	bool result = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	result &= ImGui_ImplWin32_Init(hwnd);
	result &= ImGui_ImplDX12_Init(device, mSwapChainBufferCount, mBackBufferFormat,mSrvHeap.Get()
		,mSrvHeap->GetCPUDescriptorHandleForHeapStart(),mSrvHeap->GetGPUDescriptorHandleForHeapStart());

	return result;
}


static void DestroyImGui()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}


static void DrawImGui()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Render();
}

static void EndDrawImGui(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList,Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap)
{
	mCommandList->SetDescriptorHeaps(1, mSrvHeap.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), mCommandList.Get());
}

#define ImGuiWantCatch if(imio.WantCaptureMouse || imio.WantCaptureKeyboard)break;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define MsgProcContent(hwnd,msg,wParam,lParam) if(ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))return true;const ImGuiIO& imio = ImGui::GetIO();