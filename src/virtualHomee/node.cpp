#include "node.hpp"

uint8_t node::GetNumberOfAttributes()
{
    return this->value.numberOfAttributes;
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

nodeAttributes* node::AddAttributes(nodeAttributes* attributes)
{
    if(this->GetNumberOfAttributes() > MAX_NUMBER_OF_ATTRIBUTES)
        return nullptr;
    attributes->setNodeId(this->value.id);
    if(attributes->getId() == 0)
    {
        attributes->setId(this->GetNumberOfAttributes() + (MAX_NUMBER_OF_NODES + 1) * this->value.id);
    }
    attributes->setInstance(this->calculateNextInstance(attributes->getType()));
    this->value.attributes[this->value.numberOfAttributes++] = attributes;
    return attributes;
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
