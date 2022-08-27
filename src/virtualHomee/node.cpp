#include "node.hpp"

size_t node::size()
{
    size_t s = 325 + this->value.name.length() + this->value.phonetic_name.length() 
        + this->value.image.length() + this->value.note.length() ;
    for(uint8_t i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        s += this->GetAttribute(i)->size();
    }
    return s;
}

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

void node::AddJSONObject(JsonObject jsonObject)
{
    jsonObject["id"] = this->value.id;
    jsonObject["name"] = this->value.name;
    jsonObject["profile"] = this->value.profile;
    jsonObject["image"] = this->value.image;
    jsonObject["favorite"] = this->value.favorite;
    jsonObject["order"] = this->value.order;
    jsonObject["protocol"] = this->value.protocol;
    jsonObject["routing"] = this->value.routing;
    jsonObject["state"] = this->value.state;
    jsonObject["state_changed"] = this->value.state_changed;
    jsonObject["added"] = this->value.added;
    jsonObject["history"] = this->value.history;
    jsonObject["cube_type"] = this->value.cube_type;
    jsonObject["note"] = this->value.note;
    jsonObject["services"] = this->value.services;
    jsonObject["phonetic_name"] = this->value.phonetic_name;
    jsonObject["owner"] = this->value.owner;
    jsonObject["security"] = this->value.security;
    JsonArray attributes = jsonObject.createNestedArray("attributes");
    for(int i = 0; i < this->value.numberOfAttributes; i++)
    {
        JsonObject attribute = attributes.createNestedObject();
        this->value.attributes[i]->GetJSONObject(attribute);
    }    
}

void node::AddJSONArrayElement(JsonArray jsonArray)
{
    JsonVariant jsonObject = jsonArray.addElement();
    this->AddJSONObject(jsonObject);
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

void node::serializeNode(Print& outputStream)
{
    outputStream.printf("{\"id\":%d,\"name\":\"%s\",\"profile\":%d,\"image\":\"%s\",\"favorite\":%d,\"order\":%d,\"protocol\":%d,\"routing\":%d,\"state\":%d,\"state_changed\":%d,\"added\":%d,\"history\":%d,\"cube_type\":%d,\"note\":\"%s\",\"services\":%d,\"phonetic_name\":\"%s\",\"owner\":%d,\"security\":%d,\"attributes\":[", 
        this->value.id, 
        this->value.name.c_str(), 
        this->value.profile, 
        this->value.image.c_str(), 
        this->value.favorite, 
        this->value.order, 
        this->value.protocol, 
        this->value.routing, 
        this->value.state, 
        this->value.state_changed, 
        this->value.added, 
        this->value.history, 
        this->value.cube_type, 
        this->value.note.c_str(), 
        this->value.services, 
        this->value.phonetic_name.c_str(), 
        this->value.owner, 
        this->value.security);
    for(int i = 0; i < this->GetNumberOfAttributes(); i++)
    {
        if(i > 0)
        {
            outputStream.print(',');
        }
        this->GetAttribute(i)->serializeAttributes(outputStream);
    }
    outputStream.print("]}");
}