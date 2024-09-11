#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <kiero.h>

#include "../../Logger/Logger.hpp"
#include "../../Memory/Memory.hpp"
#include "../../Events/Event.hpp"

struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};

extern inline ID3D11Device* d3d11Device = nullptr;
extern inline ID3D12Device* d3d12Device = nullptr;
extern inline ID3D11On12Device* d3d11on12Device = nullptr;
extern inline ID3D11Device* d3d11on12_11Device = nullptr;
extern inline ID3D11DeviceContext* d3d11on12_11DeviceContext = nullptr;

extern inline ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
extern inline ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
extern inline ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
extern inline ID3D12CommandQueue* d3d12CommandQueue = nullptr;
extern inline ID3D12CommandAllocator* allocator = nullptr;

extern inline uint64_t buffersCounts = 0;
extern inline std::vector<FrameContext> frameContexts = {};
extern inline ID3D12Device5* d3d12Device5 = nullptr;

extern inline ID3D12Fence* fence = nullptr;
extern inline HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

extern inline bool ImGUIintialized = false;
extern inline bool killed = true;
extern inline int fenceValue = 0;

class SetupImGUI {

	typedef void(__thiscall* CommandListdetour)(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists);
	static inline CommandListdetour CommandList_original = nullptr;

	typedef void(__thiscall* Presentdetour)(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags);
	static inline Presentdetour Present_original = nullptr;

	static void CommandList(ID3D12CommandQueue* queue, UINT numCommandLists, const ID3D12CommandList** ppCommandLists);

	static void Present(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags);

public:
	static void IntializeSetup();

	static void InitSwapchainDevice(IDXGISwapChain3* ppSwapChain);

	static void InitImGUI();

	static void RenderFrame();

	static void RenderDX11(IDXGISwapChain3* ppSwapChain);

	static void RenderDX12(IDXGISwapChain3* ppSwapChain);

};