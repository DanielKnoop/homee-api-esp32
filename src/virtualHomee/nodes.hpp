#pragma once

#include "ArduinoJson.h"
#include "node.hpp"

class nodes
{
    private:
        node* n[MAX_NUMBER_OF_NODES];
        uint8_t numberOfNodes = 0;
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
