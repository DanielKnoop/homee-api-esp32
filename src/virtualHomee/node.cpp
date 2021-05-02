#include "node.hpp"

size_t node::size()
{
    size_t s = 350;
    for(uint8_t i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        s += this->GetAttribute(i)->size();
    }
    return s;
}

uint8_t node::GetNumberOfAttributes()
{
    return this->numberOfAttributes;
}

nodeAttributes* node::GetAttribute(uint8_t n)
{
    if(n < this->numberOfAttributes)
    {
        return attributes[n];
    }
    return nullptr;
}

node::node(uint32_t id, uint32_t profile, String name)
{
    this->id = id;
    this->name = name;
    this->profile = profile;
    this->image = "default";
    this->favorite = 0;
    this->order = 1;
    this->protocol = 3;
    this->routing = 0;
    this->state = 1;
    this->state_changed = 1618853497;
    this->added = 1618853497;
    this->history = 0;
    this->cube_type = 3;
    this->note = "";
    this->services = 4;
    this->phonetic_name = "";
    this->owner = 1;
    this->security = 0;
}

uint8_t node::calculateNextInstance(uint16_t _type)
{
    uint8_t occurrence = 0;
    for (int i = 0; i < this->numberOfAttributes; i++)
    {
        if(attributes[i]->getType() == _type)
        {
            occurrence++;
        }
    }
    return occurrence + 1;
}

nodeAttributes* node::AddAttributes(nodeAttributes* attributes)
{
    if(this->numberOfAttributes > MAX_NUMBER_OF_ATTRIBUTES)
        return nullptr;
    attributes->setNodeId(this->id);
    attributes->setId(this->numberOfAttributes + 10 * this->id);
    attributes->setInstance(this->calculateNextInstance(attributes->getType()));
    this->attributes[this->numberOfAttributes++] = attributes;
    return attributes;
}

DynamicJsonDocument node::GetJSONObject()
{
    DynamicJsonDocument jo(this->size());
    jo["id"] = this->id;
    jo["name"] = this->name;
    jo["profile"] = this->profile;
    jo["image"] = this->image;
    jo["favorite"] = this->favorite;
    jo["order"] = this->order;
    jo["protocol"] = this->protocol;
    jo["routing"] = this->routing;
    jo["state"] = this->state;
    jo["state_changed"] = this->state_changed;
    jo["added"] = this->added;
    jo["history"] = this->history;
    jo["cube_type"] = this->cube_type;
    jo["note"] = this->note;
    jo["services"] = this->services;
    jo["phonetic_name"] = this->phonetic_name;
    jo["owner"] = this->owner;
    jo["security"] = this->security;
    JsonArray attributes = jo.createNestedArray("attributes");
    for(int i = 0; i < this->numberOfAttributes; i++)
    {
        attributes.add(this->attributes[i]->GetJSONArray());
    }

    return jo;
}