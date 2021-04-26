#include "ArduinoJson.h"
#include "node.hpp"

#ifndef nodes_hpp
#define nodes_hpp

class nodes
{
    private:
        node* n[10];
        uint8_t numberOfNodes = 0;
    public:
        void AddNode(node* n);
        uint8_t GetNumberOfNodes();
        node* GetNode(uint8_t n);
        DynamicJsonDocument GetJSONArray();
        nodes();
};

#endif