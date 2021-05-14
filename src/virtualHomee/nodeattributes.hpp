#pragma once

#include "ArduinoJson.h"

class nodeAttributes
{
    private:
        using callbackFunction = void(*) (nodeAttributes* changedAttribute);
        callbackFunction callback = nullptr;
        uint32_t id = 0;
        uint32_t node_id = 0;
        uint8_t instance = 0;
        double_t minimum = 0;
        double_t maximum = 100;
        double_t current_value = 0;
        double_t target_value = 0;
        double_t last_value = 0;
        String unit = "";
        double_t step_value = 1;
        uint8_t editable = 0;
        uint16_t type;
        uint8_t state = 1;
        uint32_t last_changed = 1619366694;
        uint8_t changed_by = 1;
        uint8_t changed_by_id = 0;
        uint8_t based_on = 1;
        String data = "";
        String name = "";
    public:
        nodeAttributes(uint16_t _type);
        nodeAttributes(uint16_t _type, uint32_t _id);
        
        void setId(uint32_t _id);
        uint32_t getId();
        void setNodeId(uint32_t _node_id);
        uint32_t getNodeId();
        uint16_t getType();
        void setInstance(uint8_t _instance);
        void setMinimumValue(double_t _min);
        double_t getMinimumValue();
        void setMaximumValue(double_t _max);
        double_t getMaximumValue();
        void setUnit(String _unit);
        void setCurrentValue(double_t _currentValue);
        double_t getCurrentValue();
        void setEditable(uint8_t _editable);
        double_t getTargetValue();
        void setTargetValue(double_t _targetValue);
        
        void setCallback(callbackFunction _callback);
        DynamicJsonDocument GetJSONArray();
        size_t size();
};
