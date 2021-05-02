#pragma once

#include "ArduinoJson.h"
#include "node.hpp"

#define MAX_NUMBER_OF_NODES 99

class nodes
{
    private:
        node* n[MAX_NUMBER_OF_NODES];
        uint8_t numberOfNodes = 0;
    public:
        void AddNode(node* n);
        uint8_t GetNumberOfNodes();
        node* GetNode(uint8_t n);
        DynamicJsonDocument GetJSONArray();
        size_t size();
        nodes();
};
