#pragma once

#include <Arduino.h>
#include "nodeattributes.hpp"

#define MAX_NUMBER_OF_NODES 99
#define MAX_NUMBER_OF_ATTRIBUTES 30

struct nodeValues
{
    int32_t id = 0;
    String name = ""; 
    uint32_t profile = 0;
    String image = "default";
    uint8_t favorite = 0;
    uint8_t order = 1;
    uint8_t protocol = 3;
    uint8_t routing = 0;
    uint8_t state = 1;
    uint32_t state_changed = 1618853497;
    uint32_t added = 1618853497;
    uint8_t history = 0;
    uint8_t cube_type = 3;
    String note = "";
    uint8_t services = 4;
    String phonetic_name = "";
    uint8_t owner = 1;
    uint8_t security = 0;
    nodeAttributes* attributes[MAX_NUMBER_OF_ATTRIBUTES];
    uint8_t numberOfAttributes = 0;

    void serialize(Print &outputStream)
    {
        outputStream.printf("{\"id\":%d,\"name\":\"%s\",\"profile\":%d,\"image\":\"%s\",\"favorite\":%d,\"order\":%d,\"protocol\":%d,\"routing\":%d,\"state\":%d,\"state_changed\":%d,\"added\":%d,\"history\":%d,\"cube_type\":%d,\"note\":\"%s\",\"services\":%d,\"phonetic_name\":\"%s\",\"owner\":%d,\"security\":%d,\"attributes\":[", 
            this->id, 
            this->name.c_str(), 
            this->profile, 
            this->image.c_str(), 
            this->favorite, 
            this->order, 
            this->protocol, 
            this->routing, 
            this->state, 
            this->state_changed, 
            this->added, 
            this->history, 
            this->cube_type, 
            this->note.c_str(), 
            this->services, 
            this->phonetic_name.c_str(), 
            this->owner, 
            this->security);
    for(int i = 0; i < numberOfAttributes; i++)
    {
        if(i > 0)
        {
            outputStream.print(',');
        }
        this->attributes[i]->value.serialize(outputStream);
    }
    outputStream.print("]}");
    }

};

class node
{
    public:
        node(uint32_t id, uint32_t profile, const String& name);
        ~node();
        
        nodeAttributes* AddAttributes(nodeAttributes* attributes);
        uint8_t GetNumberOfAttributes();
        nodeAttributes* GetAttribute(uint8_t n);

        void setImage(String _image);
        String getImage();
        int32_t getId() {return value.id;};
        void setState(uint8_t _state);
        uint8_t getState() { return this->value.state; };
        void setNote(String _note) { this->value.note = _note; }
        String getNote() { return this->value.note; }

    private:

        nodeValues value;

        uint8_t calculateNextInstance(uint16_t _type);
        uint32_t getTimestamp();

        friend class virtualHomee;
        friend class nodes;
        friend class virtualHomeeValues;
};
