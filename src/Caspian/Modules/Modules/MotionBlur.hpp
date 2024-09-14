#include "../Module.hpp"
#include "../../SDK/SDK.hpp"

class MotionBlur : public Module {
public:
    MotionBlur() : Module("MotionBlur", "Shows your Frames Per Second") {
        this->set("enabled", false, false);

        EventDispatcher.listen<RenderEvent>(renderEvent);
    }

    std::vector<ID3D11ShaderResourceView*> previousFrames;

    std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
        int maxFrames = 10;

        if (true) {
            if (previousFrames.size() >= static_cast<int>(maxFrames)) {
                // Remove excess frames if maxFrames is reduced
                int framesToRemove = (int)previousFrames.size() - static_cast<int>(maxFrames);
                for (int i = 0; i < framesToRemove; ++i) {
                    if (previousFrames[i]) previousFrames[i]->Release();
                }
                previousFrames.erase(previousFrames.begin(), previousFrames.begin() + framesToRemove);
            }

            ID3D11ShaderResourceView* buffer = getSRV(event);
            if (buffer) previousFrames.push_back(buffer);
            else std::cout << "Couldn't save buffer for Motion Blur.";


            float alpha = 0.3f;

            for (ID3D11ShaderResourceView* frame : previousFrames) {

                ImageWithOpacity(frame, PositionComponent(1, 1), alpha);
                alpha *= 0.8;
            }


        }
        else {

            for (ID3D11ShaderResourceView* bitmap : previousFrames) bitmap->Release();
            previousFrames.clear();
        }
    };

    void ImageWithOpacity(ID3D11ShaderResourceView* srv, ImVec2 size, float opacity)
    {
        if (opacity <= 0.0f) {
            //std::cout << "alpha: " + std::to_string(opacity) << std::endl;
            return;
        }

        opacity = opacity > 1.0f ? 1.0f : opacity < 0.0f ? 0.0f : opacity;
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImVec2 pos = {0, 0};
        ImU32 col = IM_COL32(255, 255, 255, static_cast<int>(opacity * 255));
        draw_list->AddImage(srv, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 0), ImVec2(1, 1), col);
        ImGui::SetCursorScreenPos(ImVec2(pos.x + size.x, pos.y));
    }

	void RenderSettings() override {
	}
private:
    ID3D11ShaderResourceView* getSRV(RenderEvent& event) {
        if (event.deviceType == DeviceType::DX11) {
            ID3D11Texture2D* D3D11BackBuffer = nullptr;
            ID3D11Texture2D* pBackBuffer = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
            if (FAILED(hr)) {
                std::cout << "Failed to get the back buffer. HRESULT: " << std::hex << hr << std::endl;
                return nullptr;
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
                return nullptr;
            }

            ID3D11DeviceContext* pContext = nullptr;
            event.Dx11Device->GetImmediateContext(&pContext);

            pContext->CopyResource(pStagingTexture, pBackBuffer);

            D3D11_TEXTURE2D_DESC defaultDesc = desc;
            defaultDesc.Usage = D3D11_USAGE_DEFAULT;
            defaultDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            defaultDesc.CPUAccessFlags = 0;

            hr = event.Dx11Device->CreateTexture2D(&defaultDesc, nullptr, &D3D11BackBuffer);
            if (FAILED(hr)) {
                std::cout << "Failed to create def texture: " << std::hex << hr << std::endl;
            }

            pContext->CopyResource(D3D11BackBuffer, pStagingTexture);

            pContext->Release();
            pBackBuffer->Release();
            pStagingTexture->Release();


            D3D11_TEXTURE2D_DESC d;
            D3D11BackBuffer->GetDesc(&d);
            ID3D11ShaderResourceView* outSRV;
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = d.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = d.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = 0;

            if (FAILED(hr = event.Dx11Device->CreateShaderResourceView(D3D11BackBuffer, &srvDesc, &outSRV)))
            {
                std::cout << "Failed to create shader resource view: " << std::hex << hr << std::endl;
            }

            return outSRV;
        }
        else {
            ID3D11Texture2D* D3D11BackBuffer = nullptr;
            ID3D11Resource* pWrappedBackBuffer11 = nullptr;
            ID3D11Texture2D* pStagingTexture = nullptr;

            ID3D12Resource* pBackBuffer12 = nullptr;
            HRESULT hr = event.swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer12));
            if (FAILED(hr)) {
                std::cout << "Failed to get the D3D12 back buffer. HRESULT: " << std::hex << hr << std::endl;
                return nullptr;
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
                return nullptr;
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
                return nullptr;
            }

            event.d3d11on12_11DeviceContext->CopyResource(pStagingTexture, pWrappedBackBuffer11);

            D3D11_TEXTURE2D_DESC defaultDesc = desc;
            defaultDesc.Usage = D3D11_USAGE_DEFAULT;
            defaultDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            defaultDesc.CPUAccessFlags = 0;

            hr = event.d3d11on12_11Device->CreateTexture2D(&defaultDesc, nullptr, &D3D11BackBuffer);
            if (FAILED(hr)) {
                std::cout << "Failed to create def texture: " << std::hex << hr << std::endl;
            }

            event.d3d11on12_11DeviceContext->CopyResource(D3D11BackBuffer, pStagingTexture);

            pStagingTexture->Release();
            pWrappedBackBuffer11->Release();
            pBackBuffer12->Release();


            D3D11_TEXTURE2D_DESC d;
            D3D11BackBuffer->GetDesc(&d);
            ID3D11ShaderResourceView* outSRV;
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = d.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = d.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = 0;

            if (FAILED(hr = event.d3d11on12_11Device->CreateShaderResourceView(D3D11BackBuffer, &srvDesc, &outSRV)))
            {
                std::cout << "Failed to create shader resource view: " << std::hex << hr << std::endl;
            }

            return outSRV;
        }
    }
};