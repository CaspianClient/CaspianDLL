#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "../Module.hpp"
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
        if (event.key == get<int>("keybind") and get<bool>("enabled"))
        {
            if (event.state) TakeSS = true;
        }
    };

    std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
        RndrUtils.RoundedRectFilled(Vec2(), PositionComponent(1, 1), currentScreenColor);
        currentScreenColor = Utils::LerpImColor(currentScreenColor, endScreenColor, Client::Delta * 0.05);
        if (!get<bool>("enabled") || !TakeSS)
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

            stbi_write_png(getFormattedFilename().c_str(), width, height, 4, imageData, rowPitch);

            pContext->Unmap(pStagingTexture, 0);

            pContext->Release();
            pBackBuffer->Release();
            pStagingTexture->Release();
        }
        else if (event.deviceType == DeviceType::DX12) {
            ID3D11Resource* pWrappedBackBuffer11 = nullptr;
            ID3D11Texture2D* pStagingTexture = nullptr;

            ID3D12Resource* pBackBuffer12 = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer12));
            if (FAILED(hr)) {
                std::cout << "Failed to get the D3D12 back buffer. HRESULT: " << std::hex << hr << std::endl;
                return;
            }

            D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };
            hr = event.d3d11on12Device->CreateWrappedResource(
                pBackBuffer12,
                &d3d11Flags,
                D3D12_RESOURCE_STATE_COPY_SOURCE,
                D3D12_RESOURCE_STATE_PRESENT,
                IID_PPV_ARGS(&pWrappedBackBuffer11));

            if (FAILED(hr)) {
                std::cout << "Failed to wrap the D3D12 back buffer. HRESULT: " << std::hex << hr << std::endl;
                pBackBuffer12->Release();
                return;
            }

            D3D12_RESOURCE_DESC backBufferDesc = pBackBuffer12->GetDesc();
            D3D11_TEXTURE2D_DESC desc = {};
            desc.Width = static_cast<UINT>(backBufferDesc.Width);
            desc.Height = static_cast<UINT>(backBufferDesc.Height);
            desc.MipLevels = 1;
            desc.ArraySize = 1;
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.SampleDesc.Count = 1;
            desc.Usage = D3D11_USAGE_STAGING;
            desc.BindFlags = 0;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
            desc.MiscFlags = 0;

            hr = event.d3d11on12_11Device->CreateTexture2D(&desc, nullptr, &pStagingTexture);
            if (FAILED(hr)) {
                std::cout << "Failed to create staging texture. HRESULT: " << std::hex << hr << std::endl;
                pWrappedBackBuffer11->Release();
                pBackBuffer12->Release();
                return;
            }

            event.d3d11on12_11DeviceContext->CopyResource(pStagingTexture, pWrappedBackBuffer11);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            hr = event.d3d11on12_11DeviceContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
            if (FAILED(hr)) {
                std::cout << "Failed to map the staging texture. HRESULT: " << std::hex << hr << std::endl;
                pStagingTexture->Release();
                pWrappedBackBuffer11->Release();
                pBackBuffer12->Release();
                return;
            }

            unsigned char* imageData = static_cast<unsigned char*>(mappedResource.pData);
            int width = desc.Width;
            int height = desc.Height;
            int rowPitch = mappedResource.RowPitch;

            stbi_write_png(getFormattedFilename().c_str(), width, height, 4, imageData, rowPitch);

            event.d3d11on12_11DeviceContext->Unmap(pStagingTexture, 0);

            pStagingTexture->Release();
            pWrappedBackBuffer11->Release();
            pBackBuffer12->Release();
        }

        TakeSS = false;
        currentScreenColor = ImColor(255, 255, 255, 255);
    };

private:
    std::string getFormattedFilename() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm localTime;
        localtime_s(&localTime, &time);

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %H-%M-%S") << ".png";
        return Utils::getClientFolder() + "\\Screenshots\\" + oss.str();
    }

    ImColor currentScreenColor = ImColor(255, 255, 255, 0);
    ImColor endScreenColor = ImColor(255, 255, 255, 0);
};
