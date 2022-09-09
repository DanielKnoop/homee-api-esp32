#pragma once

#include "ArduinoJson.h"
#include "node.hpp"
#include <vector>

class nodes
{
    private:
        std::vector<node*> n;
        size_t GetNodePositionInVector(node* n);
    public:
        void AddNode(node* n);
        uint8_t GetNumberOfNodes();
        node* GetNode(uint8_t n);
        node* GetNodeById(int32_t nodeId);
        void GetJSONArray(JsonArray jsonDocument);
        size_t size();
        nodes();
        ~nodes();
};
