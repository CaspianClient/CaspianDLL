#include "ResourceManager.hpp"
#include "Resources/PoppinsFont.h"
#include "Resources/MinecraftFont.h"
#include "Resources/Flag.h"
#include "Resources/monkey.h"

void ResourceManager::LoadResources() {
	Resources["PoppinsFont"] = Resource(PoppinsFont, PoppinsFont_size);
	Resources["MinecraftFont"] = Resource(MinecraftFont, MinecraftFont_size);
	Resources["Flag"] = Resource(Flag, Flag_size);
	Resources["monke"] = Resource(monke, monke_size);
}

Resource ResourceManager::getResource(std::string name) {
	return Resources[name];
}

ResourceManager resourceManager = ResourceManager();