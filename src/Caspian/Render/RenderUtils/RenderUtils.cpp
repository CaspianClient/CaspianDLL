#include "RenderUtils.hpp"

#include "../../Resources/ResourceManager.hpp"
#include "../../Resources/Resources/Flag.h"
#include "../../Client/Client.hpp"
#include "../Setup/Setup.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../ImGUI/stb.h"

bool RenderUtils::LoadTextureFromFileDX12(std::string resourceName, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	Resource img = GET_RESOURCE(resourceName);
	unsigned char* image_data = stbi_load_from_memory(img.data, img.size, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture resource
	D3D12_HEAP_PROPERTIES props;
	memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
	props.Type = D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = image_width;
	desc.Height = image_height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* pTexture = NULL;
	d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

	// Create a temporary upload resource to move the data in
	UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
	UINT uploadSize = image_height * uploadPitch;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = uploadSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	ID3D12Resource* uploadBuffer = NULL;
	HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
	IM_ASSERT(SUCCEEDED(hr));

	// Write pixels into the upload resource
	void* mapped = NULL;
	D3D12_RANGE range = { 0, uploadSize };
	hr = uploadBuffer->Map(0, &range, &mapped);
	IM_ASSERT(SUCCEEDED(hr));
	for (int y = 0; y < image_height; y++)
		memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
	uploadBuffer->Unmap(0, &range);

	// Copy the upload resource content into the real resource
	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = uploadBuffer;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srcLocation.PlacedFootprint.Footprint.Width = image_width;
	srcLocation.PlacedFootprint.Footprint.Height = image_height;
	srcLocation.PlacedFootprint.Footprint.Depth = 1;
	srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.pResource = pTexture;
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.SubresourceIndex = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pTexture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// Create a temporary command queue to do the copy with
	ID3D12Fence* fence = NULL;
	hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	IM_ASSERT(SUCCEEDED(hr));

	HANDLE event = CreateEvent(0, 0, 0, 0);
	IM_ASSERT(event != NULL);

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 1;

	ID3D12CommandQueue* cmdQueue = NULL;
	hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12CommandAllocator* cmdAlloc = NULL;
	hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* cmdList = NULL;
	hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
	IM_ASSERT(SUCCEEDED(hr));

	cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
	cmdList->ResourceBarrier(1, &barrier);

	hr = cmdList->Close();
	IM_ASSERT(SUCCEEDED(hr));

	// Execute the copy
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
	hr = cmdQueue->Signal(fence, 1);
	IM_ASSERT(SUCCEEDED(hr));

	// Wait for everything to complete
	fence->SetEventOnCompletion(1, event);
	WaitForSingleObject(event, INFINITE);

	// Tear down our temporary command queue and release the upload resource
	cmdList->Release();
	cmdAlloc->Release();
	cmdQueue->Release();
	CloseHandle(event);
	fence->Release();
	uploadBuffer->Release();

	// Create a shader resource view for the texture
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	d3d_device->CreateShaderResourceView(pTexture, &srvDesc, srv_cpu_handle);

	// Return results
	*out_tex_resource = pTexture;
	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

bool RenderUtils::LoadTextureFromFile(std::string resourceName, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	Resource img = GET_RESOURCE(resourceName);
	int width, height, channels;
	unsigned char* image_data = stbi_load_from_memory(img.data, img.size, &image_width, &image_height, &channels, 4);
	if (image_data == NULL) {
		std::cout << "no img" << std::endl;
		return false;
	}

	else {
		printf("ong image data\n");
	}

	std::cout << image_width << std::endl;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	HRESULT r = d3d11Device->CreateTexture2D(&desc, &subResource, &pTexture);

	if (FAILED(r)) printf("worked");

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	r = d3d11Device->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	if (FAILED(r)) printf("worked too");
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;

	return true;
}
	
// ImGui Config Settings
void RenderUtils::SetCurrentDrawList(DrawListType DrawList) {
	CurrentDrawList = DrawList;
}

ImDrawList* RenderUtils::getDrawList() {
	switch (CurrentDrawList)
	{
	case ForeGround:
		return ImGui::GetForegroundDrawList();
	case BackGround:
		return ImGui::GetBackgroundDrawList();
	default:
		return nullptr;
	}
}

void RenderUtils::SetupFonts() {
	Resource mcFont = GET_RESOURCE("MinecraftFont");
	FontList["MinecraftFont"] = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)mcFont.data, mcFont.size, 100);
	FontSetup = true;
}

//Utilities
void RenderUtils::PushClipRect(Vec2 Position, Vec2 Size) {
	getDrawList()->PushClipRect(Position, Position + Size);
}

void RenderUtils::PopClipRect() {
	getDrawList()->PopClipRect();
}

//Rendering Shapes
void RenderUtils::RoundedRectFilled(Vec2 Position, Vec2 Size, ImColor Colour, float Rounding, int Flags) {
	getDrawList()->AddRectFilled(Position, Position + Size, Colour, Rounding, Flags);
}

void RenderUtils::RoundedRectBorder(Vec2 Position, Vec2 Size, ImColor Colour, float Thickness, float Rounding, int Flags) {
	getDrawList()->AddRect(Position, Position + Size, Colour, Rounding, Flags, Thickness);
}

void RenderUtils::RectMultiColor(Vec2 Position, Vec2 Size, ImColor topLeft, ImColor topRight, ImColor bottomLeft, ImColor bottomRight) {
	getDrawList()->AddRectFilledMultiColor(Position, Position + Size, topLeft, topRight, bottomRight, bottomLeft);
}

//Rendering Other Things
void RenderUtils::Text(Vec2 Position, Vec2 PaddingSize, ImColor TextColor, std::string Text, float FontSize, int Alignment, std::string Font) {

	if (!FontSetup) {
		SetupFonts();
	}

	ImGui::PushFont(FontList[Font]);
	float fSize = FontSize * Client::WindowSize.y / 1080;

	ImGui::SetWindowFontScale(fSize);

	switch (Alignment) {
		case 1: 
			break;

		case 2: {
			Position.x += (PaddingSize.x / 2) - (ImGui::CalcTextSize(Text.c_str()).x / 2);
			break;
		}

		case 3: {
			Position.x += (PaddingSize.x - ImGui::CalcTextSize(Text.c_str()).x);
			break;
		}
	}

	Position.y += (PaddingSize.y / 2) - (ImGui::CalcTextSize(Text.c_str()).y / 2);
	getDrawList()->AddText(Position, TextColor, Text.c_str());
	ImGui::PopFont();
}

void RenderUtils::RenderImage(Vec2 Pos, Vec2 size, std::string image, ImColor imgColour) {
	if (d3d11Device != nullptr) {
		if (images[image] == nullptr) {
			int my_image_width = 0;
			int my_image_height = 0;
			LoadTextureFromFile(image, &images[image], (int*)&my_image_width, (int*)&my_image_height);
		}
		else if (images[image] != nullptr) {
			getDrawList()->AddImage(images[image], Pos, Pos + size, ImVec2(0, 0), ImVec2(1, 1), imgColour);
		}
	}
	else {
		if (imagesDX12[image] == nullptr) {
			static_assert(sizeof(ImTextureID) >= sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), "D3D12_CPU_DESCRIPTOR_HANDLE is too large to fit in an ImTextureID");

			// We presume here that we have our D3D device pointer in g_pd3dDevice

			int my_image_width = 0;
			int my_image_height = 0;
			ID3D12Resource* my_texture = NULL;

			// Get CPU/GPU handles for the shader resource view
			// Normally your engine will have some sort of allocator for these - here we assume that there's an SRV descriptor heap in
			// g_pd3dSrvDescHeap with at least two descriptors allocated, and descriptor 1 is unused
			UINT handle_increment = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			int descriptor_index = imagesDX12.size() + 1; // The descriptor table index to use (not normally a hard-coded constant, but in this case we'll assume we have slot 1 reserved for us)
			D3D12_CPU_DESCRIPTOR_HANDLE cpu = d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart();
			cpu.ptr += (handle_increment * descriptor_index);
			D3D12_GPU_DESCRIPTOR_HANDLE gpu = d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart();
			gpu.ptr += (handle_increment * descriptor_index);

			bool ret = LoadTextureFromFileDX12(image, d3d12Device, cpu, &my_texture, &my_image_width, &my_image_height);
			if (!ret)
				return;
			imagesDX12[image] = (ImTextureID)gpu.ptr;

		}
		else {
			getDrawList()->AddImage(imagesDX12[image], Pos, Pos + size, ImVec2(0, 0), ImVec2(1, 1), imgColour);
		}
	}
}

inline float GetColorComponent(ImU32 color, int shift) {
    return ((color >> shift) & 0xFF) / 255.0f;
}

void RenderUtils::RoundedShadows(Vec2 pos, Vec2 size, ImU32 color, float rounding, int shadowSize)
{
	ImColor shadowColor = color;
	shadowColor.Value.w *= 0.2;

	for (int i = 0; i < shadowSize; i++) {
		float progress = static_cast<float>(i) / shadowSize;
		float alphaFactor = (1.0f - progress) * (1.0f - progress);

		ImColor fadedShadowColor = ImColor(
			shadowColor.Value.x,
			shadowColor.Value.y,
			shadowColor.Value.z,
			shadowColor.Value.w * alphaFactor
		);

		Vec2 offset(progress * shadowSize, progress * shadowSize);

		RoundedRectBorder(
			pos - offset,
			size + offset + offset,
			fadedShadowColor,
			2.0f,
			rounding + progress * shadowSize,
			240
		);
	}
}

void RenderUtils::CircleFilled(Vec2 pos, float Radius, ImU32 color) {
	getDrawList()->AddCircleFilled(pos, Radius, color);
}

void RenderUtils::TriangleFilled(Vec2 Vertex1, Vec2 Vertex2, Vec2 Vertex3, ImU32 color) {
	getDrawList()->AddTriangleFilled(Vertex1, Vertex2, Vertex3, color);
}

ImColor RenderUtils::VecToImcolor(std::vector<float> color) {
	return ImColor(color[0], color[1], color[2], color[3]);
}

std::vector<float> RenderUtils::ImcolorToVec(ImColor color) {
	std::vector<float> col = { color.Value.x, color.Value.y, color.Value.z, color.Value.w };
	return col;
}