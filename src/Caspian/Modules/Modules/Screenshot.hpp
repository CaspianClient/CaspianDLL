#include <d3d12.h>
#include <dxgi1_6.h>
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
            ID3D12Resource* pBackBuffer = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
            if (FAILED(hr)) {
                std::cout << "Failed to get the back buffer. HRESULT: " << std::hex << hr << std::endl;
                return;
            }

            D3D12_RESOURCE_DESC backBufferDesc = pBackBuffer->GetDesc();

            // Create the readback buffer
            D3D12_RESOURCE_DESC readbackBufferDesc = {};
            readbackBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            readbackBufferDesc.Width = backBufferDesc.Width * backBufferDesc.Height * 4; // RGBA format (4 bytes per pixel)
            readbackBufferDesc.Height = 1;
            readbackBufferDesc.DepthOrArraySize = 1;
            readbackBufferDesc.MipLevels = 1;
            readbackBufferDesc.Format = DXGI_FORMAT_UNKNOWN; // Format not used for buffers
            readbackBufferDesc.SampleDesc.Count = 1;
            readbackBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            D3D12_HEAP_PROPERTIES heapProps = {};
            heapProps.Type = D3D12_HEAP_TYPE_READBACK;

            ID3D12Resource* pReadbackBuffer = nullptr;
            hr = event.Dx12Device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &readbackBufferDesc,
                D3D12_RESOURCE_STATE_COPY_DEST,  // Correct state for readback
                nullptr,
                IID_PPV_ARGS(&pReadbackBuffer)
            );

            if (FAILED(hr)) {
                std::cout << "Failed to create the readback buffer. HRESULT: " << std::hex << hr << std::endl;
                pBackBuffer->Release();
                return;
            }

            // Transition the back buffer to COPY_SOURCE state
            D3D12_RESOURCE_BARRIER barrier = {};
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = pBackBuffer;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

            event.CommandList->ResourceBarrier(1, &barrier);

            // Copy back buffer to readback buffer
            D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
            srcLocation.pResource = pBackBuffer;
            srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
            srcLocation.SubresourceIndex = 0;

            D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
            dstLocation.pResource = pReadbackBuffer;
            dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
            event.Dx12Device->GetCopyableFootprints(&backBufferDesc, 0, 1, 0, &dstLocation.PlacedFootprint, nullptr, nullptr, nullptr);

            event.CommandList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, nullptr);

            // Transition back to present state
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
            event.CommandList->ResourceBarrier(1, &barrier);

            // Execute the command list
            event.CommandList->Close();
            ID3D12CommandList* ppCommandLists[] = { event.CommandList };
            event.CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

            // Wait for the GPU to finish processing
            SetupImGUI::waitFor();

            // Map the readback buffer
            void* pData;
            D3D12_RANGE readRange = { 0, backBufferDesc.Width * backBufferDesc.Height * 4 };  // Read the entire buffer
            hr = pReadbackBuffer->Map(0, &readRange, &pData);
            if (FAILED(hr)) {
                std::cout << "Failed to map the readback buffer. HRESULT: " << std::hex << hr << std::endl;
                pReadbackBuffer->Release();
                pBackBuffer->Release();
                return;
            }

            // Write the image to a PNG file
            int width = static_cast<int>(backBufferDesc.Width);
            int height = static_cast<int>(backBufferDesc.Height);
            if (!stbi_write_png((Utils::getClientFolder() + "\\image.png").c_str(), width, height, 4, pData, width * 4)) {
                std::cout << "Failed to write image to PNG." << std::endl;
            }

            // Unmap and release resources
            pReadbackBuffer->Unmap(0, nullptr);
            pReadbackBuffer->Release();
            pBackBuffer->Release();
            CloseHandle(fenceEvent);

        }

        TakeSS = false;
    };
};


