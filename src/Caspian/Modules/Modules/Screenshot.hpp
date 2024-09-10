#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <iostream>
#include <functional>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "../../Render/Setup/Setup.hpp"

class Screenshot : public Module
{
public:
    bool TakeSS = false;

    Screenshot() : Module("Screenshot", "takes a Screenshot")
    {
        EventDispatcher.listen<KeyboardEvent>(KeyEvent);
        EventDispatcher.listen<RenderEvent>(renderEvent);

        this->set("enabled", false, false);
        this->set("keybind", 45, false);
    }

    void RenderSettings() override {
        AddKeybindPicker("keybind", "Keybind");
    }

    std::function<void(KeyboardEvent &)> KeyEvent = [&](KeyboardEvent &event)
    {
        if (event.key == get<int>("keybind"))
        {
            if (event.state) TakeSS = true;
        }
    };

    std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
        if (!get<bool>("enabled") or !TakeSS)
            return;

        if (event.deviceType == DeviceType::DX11) {
            ID3D11Texture2D* pBackBuffer = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
            if (FAILED(hr)) {
                std::cout << "Failed to get the back buffer. HRESULT: " << std::hex << hr << std::endl;
                return;
            }

            D3D11_TEXTURE2D_DESC desc;
            pBackBuffer->GetDesc(&desc);

            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            desc.MiscFlags = 0;

            ID3D11Texture2D* pStagingTexture = nullptr;
            hr = event.Dx11Device->CreateTexture2D(&desc, nullptr, &pStagingTexture);
            if (FAILED(hr)) {
                std::cout << "Failed to create the staging texture. HRESULT: " << std::hex << hr << std::endl;
                pBackBuffer->Release();
                return;
            }

            ID3D11DeviceContext* pContext = nullptr;
            event.Dx11Device->GetImmediateContext(&pContext);

            pContext->CopyResource(pStagingTexture, pBackBuffer);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            hr = pContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
            if (FAILED(hr)) {
                std::cout << "Failed to map the staging texture. HRESULT: " << std::hex << hr << std::endl;
                pContext->Release();
                pBackBuffer->Release();
                pStagingTexture->Release();
                return;
            }

            unsigned char* imageData = static_cast<unsigned char*>(mappedResource.pData);
            int width = desc.Width;
            int height = desc.Height;
            int rowPitch = mappedResource.RowPitch;

            stbi_write_png((Utils::getClientFolder() + "\\image.png").c_str(), width, height, 4, imageData, rowPitch);

            pContext->Unmap(pStagingTexture, 0);

            pContext->Release();
            pBackBuffer->Release();
            pStagingTexture->Release();
        }
        else if (event.deviceType == DeviceType::DX12) {
            ID3D12Resource* renderTarget = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTarget));  // Get the actual back buffer
            if (FAILED(hr)) {
                std::cout << "Failed to get the back buffer. HRESULT: " << std::hex << hr << std::endl;
                return;
            }

            TakeScreenshot(event.Dx12Device, event.CommandQueue, renderTarget,event.CommandList,event.allocator, (Utils::getClientFolder() + "\\image.png").c_str());

            renderTarget->Release();
        }

        TakeSS = false;
    };
 private:
     void TakeScreenshot(ID3D12Device* dx12Device, ID3D12CommandQueue* dx12CommandQueue, ID3D12Resource* renderTarget, ID3D12GraphicsCommandList* commandList,ID3D12CommandAllocator* commandAllocator, const char* fileName) {
         HRESULT hr;

         // Initialize the DX11on12 device
         ID3D11Device* dx11Device = nullptr;
         ID3D11DeviceContext* dx11Context = nullptr;
         ID3D11On12Device* dx11on12Device = nullptr;
         hr = D3D11On12CreateDevice(dx12Device, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
             reinterpret_cast<IUnknown**>(&dx12CommandQueue), 1, 0, &dx11Device, &dx11Context, nullptr);
         if (FAILED(hr)) {
             std::cout << "Failed to create DX11On12 device. HRESULT: " << std::hex << hr << std::endl;
             return;
         }

         hr = dx11Device->QueryInterface(IID_PPV_ARGS(&dx11on12Device));
         if (FAILED(hr)) {
             std::cout << "Failed to query DX11On12 interface. HRESULT: " << std::hex << hr << std::endl;
             dx11Device->Release();
             return;
         }


         // Transition the render target to the copy source state
         D3D12_RESOURCE_BARRIER barrier = {};
         barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
         barrier.Transition.pResource = renderTarget;
         barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
         barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
         barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
         commandList->ResourceBarrier(1, &barrier);

         hr = commandList->Close();
         if (FAILED(hr)) {
             std::cout << "Failed to close command list. HRESULT: " << std::hex << hr << std::endl;
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         ID3D12CommandList* commandLists[] = { commandList };
         dx12CommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

         // Create a fence for synchronization
         ID3D12Fence* fence = nullptr;
         UINT64 fenceValue = 1;
         HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
         hr = dx12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
         if (FAILED(hr)) {
             std::cout << "Failed to create fence. HRESULT: " << std::hex << hr << std::endl;
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         hr = dx12CommandQueue->Signal(fence, fenceValue);
         if (FAILED(hr)) {
             std::cout << "Failed to signal the fence. HRESULT: " << std::hex << hr << std::endl;
             fence->Release();
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         if (fence->GetCompletedValue() < fenceValue) {
             hr = fence->SetEventOnCompletion(fenceValue, fenceEvent);
             if (FAILED(hr)) {
                 std::cout << "Failed to set fence event on completion. HRESULT: " << std::hex << hr << std::endl;
                 fence->Release();
                 
                 commandAllocator->Release();
                 dx11on12Device->Release();
                 dx11Device->Release();
                 return;
             }
             WaitForSingleObject(fenceEvent, INFINITE);
         }

         // Now that the resource is ready, we can wrap it with DX11On12
         ID3D11Resource* wrappedResource = nullptr;
         D3D11_RESOURCE_FLAGS resourceFlags = { D3D11_BIND_SHADER_RESOURCE };
         hr = dx11on12Device->CreateWrappedResource(renderTarget, &resourceFlags, D3D12_RESOURCE_STATE_COPY_SOURCE,
             D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(&wrappedResource));
         if (FAILED(hr) || !wrappedResource) {
             std::cout << "Failed to create wrapped resource. HRESULT: " << std::hex << hr << std::endl;
             fence->Release();
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         // Acquire the wrapped resources
         dx11on12Device->AcquireWrappedResources(&wrappedResource, 1);

         // Create a staging texture for CPU read access
         ID3D11Texture2D* texture2D = nullptr;
         hr = wrappedResource->QueryInterface(IID_PPV_ARGS(&texture2D));
         if (FAILED(hr) || !texture2D) {
             std::cout << "Failed to query texture2D interface from wrapped resource. HRESULT: " << std::hex << hr << std::endl;
             wrappedResource->Release();
             fence->Release();
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         D3D11_TEXTURE2D_DESC desc;
         texture2D->GetDesc(&desc);
         desc.Usage = D3D11_USAGE_STAGING;
         desc.BindFlags = 0;
         desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
         desc.MiscFlags = 0;

         ID3D11Texture2D* stagingTexture = nullptr;
         hr = dx11Device->CreateTexture2D(&desc, nullptr, &stagingTexture);
         if (FAILED(hr) || !stagingTexture) {
             std::cout << "Failed to create staging texture. HRESULT: " << std::hex << hr << std::endl;
             texture2D->Release();
             wrappedResource->Release();
             fence->Release();
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         // Copy the data from GPU to the CPU accessible staging texture
         dx11Context->CopyResource(stagingTexture, texture2D);

         // Map the staging texture to access the pixel data
         D3D11_MAPPED_SUBRESOURCE mappedResource;
         hr = dx11Context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
         if (FAILED(hr)) {
             std::cout << "Failed to map the staging texture. HRESULT: " << std::hex << hr << std::endl;
             stagingTexture->Release();
             texture2D->Release();
             wrappedResource->Release();
             fence->Release();
             
             commandAllocator->Release();
             dx11on12Device->Release();
             dx11Device->Release();
             return;
         }

         // Use stb_image_write to save the image
         int width = desc.Width;
         int height = desc.Height;
         int channels = 4; // RGBA

         // Save the image as a PNG
         stbi_write_png(fileName, width, height, channels, mappedResource.pData, mappedResource.RowPitch);

         // Unmap the staging texture
         dx11Context->Unmap(stagingTexture, 0);

         // Cleanup
         stagingTexture->Release();
         texture2D->Release();
         wrappedResource->Release();
         fence->Release();
         
         commandAllocator->Release();
         dx11on12Device->Release();
         dx11Context->Release();
         dx11Device->Release();
     }

};