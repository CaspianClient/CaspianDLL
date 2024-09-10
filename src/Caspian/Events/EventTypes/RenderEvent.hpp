#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d12.h>

enum DeviceType{
    DX11,
    DX12
};

class RenderEvent {
public:
    IDXGISwapChain* swapChain;
    DeviceType deviceType;
    ID3D11Device* Dx11Device;
    ID3D12Device* Dx12Device;
    ID3D12GraphicsCommandList* CommandList;
    ID3D12CommandQueue* CommandQueue;
    ID3D12CommandAllocator* allocator;
};