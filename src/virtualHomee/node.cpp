#include "node.hpp"

uint8_t node::GetNumberOfAttributes()
{
    return this->value.attributes.size();
}

nodeAttributes* node::GetAttribute(uint8_t n)
{
    if(n < this->GetNumberOfAttributes())
    {
        return value.attributes[n];
    }
    return nullptr;
}

node::node(uint32_t id, uint32_t profile, const String& name)
{
    this->value.id = id;
    this->value.name = name;
    this->value.profile = profile;
}

uint8_t node::calculateNextInstance(uint16_t _type)
{
    uint8_t occurrence = 0;
    for (int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        if(value.attributes[i]->getType() == _type)
        {
            occurrence++;
        }
    }
    return occurrence;
}

nodeAttributes* node::AddAttributes(nodeAttributes* attribute)
{
    attribute->setNodeId(this->value.id);
    if(attribute->getId() == 0)
    {
        attribute->setId(this->GetNumberOfAttributes() + (100 + 1) * this->value.id);
    }
    attribute->setInstance(this->calculateNextInstance(attribute->getType()));
    this->value.attributes.push_back(attribute);
    return attribute;
}

String node::getImage() 
{
    return this->value.image;
}

void node::setImage(String _image)
{
    this->value.image = _image;
}

node::~node()
{
    for(int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        delete value.attributes[i];
        value.attributes[i] = nullptr;
    }
}

void node::setState(uint8_t _state)
{
    this->value.state = _state;
    this->value.state_changed = getTimestamp();
}

uint32_t node::getTimestamp()
{
    time_t now = time(&now);
    return time(&now);
}
