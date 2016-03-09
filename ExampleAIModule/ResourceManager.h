#pragma once

#include <BWAPI.h>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	ResourceManager& ResourceManager::getInstance();
};

