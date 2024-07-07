#pragma once
#include <iostream>
#include <map>
#include "resource.hpp"

#define GET_RESOURCE(name) resourceManager.getResource(name);

class ResourceManager {
	std::map<std::string, Resource> Resources;

public:
	void LoadResources();
	Resource getResource(std::string name);
};

extern ResourceManager resourceManager;