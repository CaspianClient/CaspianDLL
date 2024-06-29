#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <kiero.h>
#include "../../Logger/Logger.hpp"
#include "../../Memory/Memory.hpp"

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

namespace D2D {
    inline IDXGISurface1* surface = nullptr;
};


ID3D11Device* d3d11Device;
ID3D12Device* d3d12Device;

ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers;AA
ID3D12GraphicsCommandList* d3d12CommandList;
ID3D12CommandQueue* d3d12CommandQueue;
ID3D12CommandAllocator* allocator;

uint64_t buffersCounts;
std::vector<FrameContext> frameContexts;
ID3D12Device5* d3d12Device5;



class SetupImGUI {
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
};

typedef void(__thiscall *CommandListdetour)(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists);
CommandListdetour CommandList_original = nullptr;

typedef void(__thiscall *Presentdetour)(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags);
Presentdetour Present_original = nullptr;

void CommandList(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists) {
	if(!d3d12CommandQueue) d3d12CommandQueue = queue;

	return CommandList_original(queue, numCommandLists, ppCommandLists);
}

void Present(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {
	return Present_original(ppSwapChain, syncInterval, flags);
}