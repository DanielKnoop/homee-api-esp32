#include "node.hpp"

size_t node::size()
{
    size_t s = 325 + this->name.length() + this->phonetic_name.length() 
        + this->image.length() + this->note.length() ;
    for(uint8_t i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        s += this->GetAttribute(i)->size();
    }
    return s;
}

uint8_t node::GetNumberOfAttributes()
{
    return this->attributes.size();
}

nodeAttributes* node::GetAttribute(uint8_t n)
{
    if(n < this->GetNumberOfAttributes())
    {
        return attributes[n];
    }
    return nullptr;
}

node::node(uint32_t id, uint32_t profile, const String& name)
{
    this->id = id;
    this->name = name;
    this->profile = profile;
}

uint8_t node::calculateNextInstance(uint16_t _type)
{
    uint8_t occurrence = 0;
    for (int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        if(this->GetAttribute(i)->getType() == _type)
        {
            occurrence++;
        }
    }
    return occurrence;
}

nodeAttributes* node::AddAttributes(nodeAttributes* attribute)
{
    attribute->setNodeId(this->id);
    if(attribute->getId() == 0)
    {
        attribute->setId(this->GetNumberOfAttributes() + (100 + 1) * this->id);
    }
    attribute->setInstance(this->calculateNextInstance(attribute->getType()));
    this->attributes.push_back(attribute);
    return attribute;
}

void node::AddJSONObject(JsonObject jsonObject)
{
    jsonObject["id"] = this->id;
    jsonObject["name"] = this->name;
    jsonObject["profile"] = this->profile;
    jsonObject["image"] = this->image;
    jsonObject["favorite"] = this->favorite;
    jsonObject["order"] = this->order;
    jsonObject["protocol"] = this->protocol;
    jsonObject["routing"] = this->routing;
    jsonObject["state"] = this->state;
    jsonObject["state_changed"] = this->state_changed;
    jsonObject["added"] = this->added;
    jsonObject["history"] = this->history;
    jsonObject["cube_type"] = this->cube_type;
    jsonObject["note"] = this->note;
    jsonObject["services"] = this->services;
    jsonObject["phonetic_name"] = this->phonetic_name;
    jsonObject["owner"] = this->owner;
    jsonObject["security"] = this->security;
    JsonArray attributes = jsonObject.createNestedArray("attributes");
    for(int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        JsonObject attribute = attributes.createNestedObject();
        this->attributes[i]->GetJSONObject(attribute);
    }    
}

void node::AddJSONArrayElement(JsonArray jsonArray)
{
    JsonObject jsonObject = jsonArray.createNestedObject();
    this->AddJSONObject(jsonObject);
}

String node::getImage() 
{
    return this->image;
}

void node::setImage(String _image)
{
    this->image = _image;
}

node::~node()
{
    for(int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        delete attributes[i];
    }
    attributes.clear();
    Serial.println("Node Removed");
}

void node::setState(uint8_t _state)
{
    this->state = _state;
    this->state_changed = getTimestamp();
}

uint32_t node::getTimestamp()
{
    time_t now = time(&now);
    return time(&now);
}