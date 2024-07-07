#include "ResourceManager.hpp"
#include "Resources/PoppinsFont.h"

void ResourceManager::LoadResources() {
	Resources["PoppinsFont"] = Resource(PoppinsFont, PoppinsFont_size);
}

Resource ResourceManager::getResource(std::string name) {
	return Resources[name];
}

ResourceManager resourceManager = ResourceManager();