#pragma once

#include "ArduinoJson.h"
#include "nodeattributes.hpp"

#define MAX_NUMBER_OF_NODES 99
#define MAX_NUMBER_OF_ATTRIBUTES 99

class node
{
    private:
        int32_t id;
        String name;
        uint32_t profile;
        String image;
        uint8_t favorite;
        uint8_t order;
        uint8_t protocol;
        uint8_t routing;
        uint8_t state;
        uint32_t state_changed;
        uint32_t added;
        uint8_t history;
        uint8_t cube_type;
        String note;
        uint8_t services;
        String phonetic_name;
        uint8_t owner;
        uint8_t security;
        nodeAttributes* attributes[MAX_NUMBER_OF_ATTRIBUTES];
        uint8_t numberOfAttributes = 0;

        uint8_t calculateNextInstance(uint16_t _type);
    public:
        node(uint32_t id, uint32_t profile, String name);
        nodeAttributes* AddAttributes(nodeAttributes* attributes);
        DynamicJsonDocument GetJSONObject();
        uint8_t GetNumberOfAttributes();
        nodeAttributes* GetAttribute(uint8_t n);
        size_t size();
};
