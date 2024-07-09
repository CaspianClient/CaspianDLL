#include "ResourceManager.hpp"
#include "Resources/PoppinsFont.h"
#include "Resources/MinecraftFont.h"

void ResourceManager::LoadResources() {
	Resources["PoppinsFont"] = Resource(PoppinsFont, PoppinsFont_size);
	Resources["MinecraftFont"] = Resource(MinecraftFont, MinecraftFont_size);
}

Resource ResourceManager::getResource(std::string name) {
	return Resources[name];
}

ResourceManager resourceManager = ResourceManager();