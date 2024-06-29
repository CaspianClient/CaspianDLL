#pragma once
#include "VisualTree.hpp"

class ScreenView
{
public:
    BUILD_ACCESS(this, class VisualTree*, VisualTree, 0x48);

    std::string TopLayer() {
        return this->VisualTree->root->LayerName;
    }
};

