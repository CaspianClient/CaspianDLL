#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <kiero.h>
#include "../../Logger/Logger.hpp"
#include "../../Memory/Memory.hpp"
#include "../ImGUI/imgui_impl_dx11.h"
#include "../ImGUI/imgui_impl_dx12.h"
#include "../ImGUI/imgui_impl_win32.h"

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

ID3D11Device* d3d11Device;
ID3D12Device* d3d12Device;

ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers;
ID3D12GraphicsCommandList* d3d12CommandList;
ID3D12CommandQueue* d3d12CommandQueue;
ID3D12CommandAllocator* allocator;

uint64_t buffersCounts;
std::vector<FrameContext> frameContexts;
ID3D12Device5* d3d12Device5;

bool ImGUIintialized = false;
bool killed = false;

auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");

class SetupImGUI {

	typedef void(__thiscall *CommandListdetour)(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists);
	static inline CommandListdetour CommandList_original = nullptr;

	typedef void(__thiscall *Presentdetour)(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags);
	static inline Presentdetour Present_original = nullptr;

	static void CommandList(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists) {
		if(!d3d12CommandQueue) d3d12CommandQueue = queue;

		return CommandList_original(queue, numCommandLists, ppCommandLists);
	}

	static void Present(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {

		if (!killed) {
			if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5)))) {
				d3d12Device5->RemoveDevice();
				killed = true;

				return Present_original(ppSwapChain, syncInterval, flags);
			}
		}

		InitSwapchainDevice(ppSwapChain);

		if (d3d11Device) {
			InitImGUI();
			RenderDX11(ppSwapChain);
		}

		return Present_original(ppSwapChain, syncInterval, flags);
	}

public:
	static void IntializeSetup() {
		if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success) {
			Logger::info("DX12 Device Found.");

			Memory::hookFunc((void*)kiero::getMethodsTable()[54], CommandList, (void**)&CommandList_original, "Command List");
			Memory::hookFunc((void*)kiero::getMethodsTable()[140], Present, (void**)&Present_original, "Present");
		}
		else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
			Logger::info("DX11 Device Found.");

			Memory::hookFunc((void*)kiero::getMethodsTable()[8], Present, (void**)&Present_original, "Present");
		}
	}

	static void InitSwapchainDevice(IDXGISwapChain3* ppSwapChain) {
		if (d3d11Device == nullptr && d3d12Device == nullptr) {

			if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {

				Logger::debug("SwapChain initializing for D3D12");

			}
			else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {

				Logger::debug("SwapChain initializing for D3D11");

			};

		};
	}

	static void InitImGUI() {
		if (!ImGUIintialized) {
			
			ImGui::CreateContext();

			ID3D11DeviceContext* ppContext = nullptr;
			d3d11Device->GetImmediateContext(&ppContext);

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);

			ppContext->Release();

			ImGUIintialized = true;
		}
	}

	static void RenderDX11(IDXGISwapChain3* ppSwapChain) {
		ID3D11RenderTargetView* mainRenderTargetView = nullptr;
		ID3D11DeviceContext* ppContext = nullptr;
		ID3D11Texture2D* pBackBuffer = nullptr;

		d3d11Device->GetImmediateContext(&ppContext);

		if (ppContext) {

			if (SUCCEEDED(ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {

				if (SUCCEEDED(d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView))) {
					ImGui_ImplDX11_NewFrame();
					ImGui_ImplWin32_NewFrame();
					ImGui::NewFrame();

					ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(), ImVec2(100, 100), IM_COL32_WHITE, 0, 240);

					ImGui::EndFrame();
					ImGui::Render();

					ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
					ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				};

			};

		};

		if (pBackBuffer)
			pBackBuffer->Release();
					
		if(mainRenderTargetView)
			mainRenderTargetView->Release();

		if (ppContext)
			ppContext->Release();
	}
};