#include "Setup.hpp"

#include "../../Resources/ResourceManager.hpp"
#include "../RenderUtils/RenderUtils.hpp"
#include "../../Client/Client.hpp"

#include "../ImGUI/imgui_impl_dx11.h"
#include "../ImGUI/imgui_impl_dx12.h"
#include "../ImGUI/imgui_impl_win32.h"

auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");

void SetupImGUI::CommandList(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists) {
		if (!d3d12CommandQueue) d3d12CommandQueue = queue;

		return CommandList_original(queue, numCommandLists, ppCommandLists);
}

void SetupImGUI::Present(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {

	if (!killed) {
		HRESULT hr = ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5));
		if (FAILED(hr)) {
			printf("Failed to remove D3D12Device.\n");
			killed = true;
			return Present_original(ppSwapChain, syncInterval, flags);
		}
		d3d12Device5->RemoveDevice();
		killed = true;
		return Present_original(ppSwapChain, syncInterval, flags);
	}

	InitSwapchainDevice(ppSwapChain);

	if (d3d11Device) {
		InitImGUI();
		RenderDX11(ppSwapChain);
	}
	else if (d3d12Device) {
		RenderDX12(ppSwapChain);
	}

	return Present_original(ppSwapChain, syncInterval, flags);
}

void SetupImGUI::IntializeSetup() {
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

void SetupImGUI::InitSwapchainDevice(IDXGISwapChain3* ppSwapChain) {
	if (d3d11Device == nullptr && d3d12Device == nullptr) {

		if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {

			Logger::debug("SwapChain initializing for D3D12");

		}
		else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {

			Logger::debug("SwapChain initializing for D3D11");

		};

	};
}

void SetupImGUI::InitImGUI() {
	if (!ImGUIintialized) {

		ImGui::CreateContext();

		if (d3d11Device) {
			ID3D11DeviceContext* ppContext = nullptr;
			d3d11Device->GetImmediateContext(&ppContext);
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(d3d11Device, ppContext);
			ppContext->Release();
		}
		else if (d3d12Device) {

			D3D11On12CreateDevice(d3d12Device, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
				reinterpret_cast<IUnknown**>(&d3d12CommandQueue), 1, 0, &d3d11on12_11Device, &d3d11on12_11DeviceContext, nullptr);

			d3d11on12_11Device->QueryInterface(IID_PPV_ARGS(&d3d11on12Device));

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
				DXGI_FORMAT_R8G8B8A8_UNORM, d3d12DescriptorHeapImGuiRender,
				d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
		}

		RndrUtils.SetupFonts();

		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		Client::WindowSize = Vec2(desktop.right, desktop.bottom);

		ImGUIintialized = true;
	}
}

void SetupImGUI::RenderDX11(IDXGISwapChain3* ppSwapChain) {
	ID3D11RenderTargetView* mainRenderTargetView = nullptr;
	ID3D11DeviceContext* ppContext = nullptr;
	ID3D11Texture2D* pBackBuffer = nullptr;


	d3d11Device->GetImmediateContext(&ppContext);

	if (!ppContext)
		return;

	if (FAILED(ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return;

	if (FAILED(d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView)))
		return;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    nes::event_holder<RenderEvent> event;
    event->swapChain = ppSwapChain;
    event->deviceType = DeviceType::DX11;
    event->Dx11Device = d3d11Device;
    EventDispatcher.trigger(event);

	ImGui::EndFrame();
	ImGui::Render();

	ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (pBackBuffer) pBackBuffer->Release();

	if (mainRenderTargetView) mainRenderTargetView->Release();

	if (ppContext) ppContext->Release();
}

void SetupImGUI::RenderDX12(IDXGISwapChain3* ppSwapChain) {
	if (!d3d12CommandQueue)
		return;

	DXGI_SWAP_CHAIN_DESC sdesc;
	ppSwapChain->GetDesc(&sdesc);
	sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sdesc.OutputWindow = window;
	sdesc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	buffersCounts = sdesc.BufferCount;
	frameContexts.resize(buffersCounts);

	D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
	descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptorImGuiRender.NumDescriptors = buffersCounts;
	descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	if (!d3d12DescriptorHeapImGuiRender)
		if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender))))
			return;

	if (FAILED(d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator))))
		return;

	for (size_t i = 0; i < buffersCounts; i++) {
		frameContexts[i].commandAllocator = allocator;
	};

	if (FAILED(d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator, NULL, IID_PPV_ARGS(&d3d12CommandList))))
		return;

	D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
	descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorBackBuffers.NumDescriptors = buffersCounts;
	descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorBackBuffers.NodeMask = 1;

	if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers))))
		return;

	const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

	for (size_t i = 0; i < buffersCounts; i++) {
		ID3D12Resource* pBackBuffer = nullptr;

		frameContexts[i].main_render_target_descriptor = rtvHandle;
		ppSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
		d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
		frameContexts[i].main_render_target_resource = pBackBuffer;
		rtvHandle.ptr += rtvDescriptorSize;

		pBackBuffer->Release();
	};

	InitImGUI();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

    nes::event_holder<RenderEvent> event;
    event->swapChain = ppSwapChain;
    event->deviceType = DeviceType::DX12;
    event->Dx12Device = d3d12Device;
    event->CommandList = d3d12CommandList;
    event->CommandQueue = d3d12CommandQueue;
    event->allocator = allocator;
	event->d3d11on12Device = d3d11on12Device;
	event->d3d11on12_11Device = d3d11on12_11Device;
	event->d3d11on12_11DeviceContext = d3d11on12_11DeviceContext;
    EventDispatcher.trigger(event);

	frameContexts[ppSwapChain->GetCurrentBackBufferIndex()].commandAllocator->Reset();;

	D3D12_RESOURCE_BARRIER barrier;
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = frameContexts[ppSwapChain->GetCurrentBackBufferIndex()].main_render_target_resource;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	d3d12CommandList->Reset(frameContexts[ppSwapChain->GetCurrentBackBufferIndex()].commandAllocator, nullptr);
	d3d12CommandList->ResourceBarrier(1, &barrier);
	d3d12CommandList->OMSetRenderTargets(1, &frameContexts[ppSwapChain->GetCurrentBackBufferIndex()].main_render_target_descriptor, FALSE, nullptr);
	d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	d3d12CommandList->ResourceBarrier(1, &barrier);
	d3d12CommandList->Close();

	d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));

	if (allocator) {
		allocator->Release();
		allocator = nullptr;
	}

	if (d3d12CommandList) {
		d3d12CommandList->Release();
		d3d12CommandList = nullptr;
	}

	if (d3d12DescriptorHeapBackBuffers) {
		d3d12DescriptorHeapBackBuffers->Release();
		d3d12DescriptorHeapBackBuffers = nullptr;
	}

	if (!frameContexts.empty() && frameContexts.front().commandAllocator != nullptr) {
		frameContexts.front().commandAllocator = nullptr;
	}

	if (!frameContexts.empty() && frameContexts.front().main_render_target_resource != nullptr) {
		frameContexts.front().main_render_target_resource->Release();
		frameContexts.front().main_render_target_resource = nullptr;
	}

	frameContexts.resize(0);
}

void SetupImGUI::RenderFrame() {
	nes::event_holder<RenderEvent> event;
	EventDispatcher.trigger(event);
}

void SetupImGUI::Disable() {
	Memory::Release(d3d11Device);
	Memory::Release(d3d12Device);
	Memory::Release(d3d11on12Device);
	Memory::Release(d3d11on12_11Device);
	Memory::Release(d3d11on12_11DeviceContext);
	Memory::Release(d3d12DescriptorHeapImGuiRender);
	Memory::Release(d3d12DescriptorHeapBackBuffers);
	Memory::Release(d3d12CommandList);
	//Memory::Release(d3d12CommandQueue);

	for (auto x : frameContexts) {
		Memory::Release(x.commandAllocator);
		Memory::Release(x.main_render_target_resource);
	}
}