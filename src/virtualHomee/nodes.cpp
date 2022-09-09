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
    //n[this->numberOfNodes++] = n_1;
    n.push_back(n_1);
}

void nodes::GetJSONArray(JsonArray jsonDocument)
{
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        n[i]->AddJSONArrayElement(jsonDocument);
    }    
}

node* nodes::GetNode(uint8_t n)
{
    if (n < this->GetNumberOfNodes())
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
        if(this->GetNode(i)->getId() == nodeId)
        {
            return this->GetNode(i);
        }
    }
    return nullptr;
}

uint8_t nodes::GetNumberOfNodes() 
{
    return this->n.size();
}

nodes::nodes()
{

}

nodes::~nodes()
{
    
}

void nodes::RemoveNodeById(int32_t nodeId)
{
    node* n_1 = this->GetNodeById(nodeId);
    if(n_1)
    {
        size_t position = GetNodePositionInVector(n_1);
        if(position < 9999)
            n.erase(n.begin() + position);
        delete n_1;
    }
}

size_t nodes::GetNodePositionInVector(node* n)
{
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        if(this->GetNode(i) == n)
        {
            return i;
        }
    }
    return 9999;
}