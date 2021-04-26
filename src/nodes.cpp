#include "nodes.hpp"

void nodes::AddNode(node* n_1)
{
    n[this->numberOfNodes++] = n_1;
}

DynamicJsonDocument nodes::GetJSONArray()
{
    DynamicJsonDocument ja(this->numberOfNodes * 1024);
    JsonArray nestedArray = ja.createNestedArray("nodes");
    for(int i = 0; i < this->numberOfNodes; i++)
    {
        nestedArray.add(n[i]->GetJSONObject());
    }
    return ja;
}

node* nodes::GetNode(uint8_t n)
{
    if (n < this->numberOfNodes)
    {
        return this->n[n];
    }
    else
    {
        return nullptr;
    }
}

uint8_t nodes::GetNumberOfNodes() 
{
    return this->numberOfNodes;
}

nodes::nodes()
{

}