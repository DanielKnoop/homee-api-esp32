#pragma once

#include "ArduinoJson.h"
#include "nodeattributes.hpp"

#define MAX_NUMBER_OF_NODES 99
#define MAX_NUMBER_OF_ATTRIBUTES 99

class node
{
    private:
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

        uint8_t calculateNextInstance(uint16_t _type);
        uint32_t getTimestamp();
    public:
        node(uint32_t id, uint32_t profile, String name);
        ~node();
        nodeAttributes* AddAttributes(nodeAttributes* attributes);
        void AddJSONArrayElement(JsonArray jsonArray);
        void AddJSONObject(JsonObject jsonObject);
        uint8_t GetNumberOfAttributes();
        nodeAttributes* GetAttribute(uint8_t n);
        size_t size();
        void setImage(String _image);
        String getImage();
        int32_t getId() {return id;};
        void setState(uint8_t _state);
        uint8_t getState() { return this->state; };
        void setNote(String _note) { this->note = _note; }
        String getNote() { return this->note; }
};
