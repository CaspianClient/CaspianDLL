#include "ResourceManager.hpp"
#include "Resources/PoppinsFont.h"
#include "Resources/MinecraftFont.h"
#include "Resources/Flag.h"
#include "Resources/monkey.h"
#include "Resources/Settings.h"
#include "Resources/Transparent.h"
#include "Resources/TransparentRect.hpp"

void ResourceManager::LoadResources() {
	Resources["PoppinsFont"] = Resource(PoppinsFont, PoppinsFont_size);
	Resources["MinecraftFont"] = Resource(MinecraftFont, MinecraftFont_size);
	Resources["Flag"] = Resource(Flag, Flag_size);
	Resources["monke"] = Resource(monke, monke_size);
	Resources["Settings"] = Resource(settings, settings_size);
	Resources["Transparent"] = Resource(Transparent, Transparent_size);
	Resources["TransparentRect"] = Resource(TransparentRect, TransparentRect_size);
}

Resource ResourceManager::getResource(std::string name) {
	return Resources[name];
}

ResourceManager resourceManager = ResourceManager();