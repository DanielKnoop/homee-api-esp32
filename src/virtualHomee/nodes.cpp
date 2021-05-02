#include "nodes.hpp"

size_t nodes::size()
{
    size_t s = 0;
    for(uint8_t i = 0; i < this->GetNumberOfNodes(); i++)
    {
        s += this->GetNode(i)->size();
    }
    return s;
}

void nodes::AddNode(node* n_1)
{
    if(this->numberOfNodes > MAX_NUMBER_OF_NODES)
        return;
    n[this->numberOfNodes++] = n_1;
}

DynamicJsonDocument nodes::GetJSONArray()
{
    DynamicJsonDocument ja(this->size());
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