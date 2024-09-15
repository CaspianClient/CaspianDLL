#include "../Module.hpp"
#include "../../SDK/SDK.hpp"
#include <math.h>

class MotionBlur : public Module {
public:
    MotionBlur() : Module("MotionBlur", "Shows your Frames Per Second") {
        this->set("enabled", false, false);
        this->set("intensity", 0.5, false);
        this->set("diff", 0.5, false);

        EventDispatcher.listen<RenderEvent>(renderEvent);
    }

    std::vector<ID3D11ShaderResourceView*> previousFrames;

    std::function<void(RenderEvent&)> renderEvent = [&](RenderEvent& event) {
        int maxFrames = Client::currentFrameRate * (get<float>("intensity") * 0.1) * 0.8;

        if (event.deviceType == DX11 and get<bool>("enabled")) {
            if (previousFrames.size() >= static_cast<int>(maxFrames)) {
                int framesToRemove = (int)previousFrames.size() - static_cast<int>(maxFrames);
                for (int i = maxFrames; i < previousFrames.size(); ++i) {
                    if (previousFrames[i]) previousFrames[i]->Release();
                }
                previousFrames.erase(previousFrames.end() - framesToRemove, previousFrames.end());
            }

            ID3D11ShaderResourceView* buffer = getSRV(event);
            if (buffer) previousFrames.insert(previousFrames.begin(), buffer);
            else std::cout << "Couldn't save buffer for Motion Blur.";

            float diff = get<float>("diff") * 3 * Client::Delta;

            float alpha = 0.3f * (pow(diff, previousFrames.size()));

            for (ID3D11ShaderResourceView* frame : previousFrames) {

                ImageWithOpacity(frame, PositionComponent(1, 1), alpha);
                alpha /= diff;
            }


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
        draw_list->AddImage(srv, pos, ImVec2(Width, Height), ImVec2(0, 0), ImVec2(1, 1), col);
    }

	void RenderSettings() override {
        AddSlider("intensity", "Intensity", 0, 1);
        AddSlider("diff", "Frame Diffusion", 0, 1);
	}
private:
    int Width = 0;
    int Height = 0;

    ID3D11Texture2D* pStagingTexture = nullptr;
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

            if (pStagingTexture == nullptr) {
                hr = event.Dx11Device->CreateTexture2D(&desc, nullptr, &pStagingTexture);
                if (FAILED(hr)) {
                    std::cout << "Failed to create the staging texture. HRESULT: " << std::hex << hr << std::endl;
                    pBackBuffer->Release();
                    return nullptr;
                }
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

            Width = desc.Width;
            Height = desc.Height;

            D3D11BackBuffer->Release();

            return outSRV;
        }
    }
};