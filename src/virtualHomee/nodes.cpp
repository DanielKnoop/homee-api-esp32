#include "nodes.hpp"

size_t nodes::size()
{
    size_t s = 0;
    for(uint8_t i = 0; i < this->GetNumberOfNodes(); i++)
    {
        s += this->GetNode(i)->size();
    }
    return s + JSON_ARRAY_SIZE(this->GetNumberOfNodes()) + 16;
}

void nodes::AddNode(node* n_1)
{
    if(this->numberOfNodes > MAX_NUMBER_OF_NODES)
        return;
    n[this->numberOfNodes++] = n_1;
}

void nodes::GetJSONArray(JsonArray jsonDocument)
{
    for(int i = 0; i < this->numberOfNodes; i++)
    {
        n[i]->AddJSONArrayElement(jsonDocument);
    }    
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

node* nodes::GetNodeById(int32_t nodeId)
{
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        if(this->n[i]->getId() == nodeId)
        {
            return n[i];
        }
    }
    return nullptr;
}

uint8_t nodes::GetNumberOfNodes() 
{
    return this->numberOfNodes;
}

nodes::nodes()
{

}

nodes::~nodes()
{
    
}