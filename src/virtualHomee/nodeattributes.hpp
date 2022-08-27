#pragma once

#include <Arduino.h>
struct nodeAttributesValues
{
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

    void serialize(Print &outputStream)
    {
        outputStream.printf("{\"id\":%d,\"node_id\":%d,\"instance\":%d,\"minimum\":%f,\"maximum\":%f,\"current_value\":%f,\"target_value\":%f,\"last_value\":%f,\"unit\":\"%s\",\"step_value\":%f,\"editable\":%d,\"type\":%d,\"state\":%d,\"last_changed\":%d,\"changed_by\":%d,\"changed_by_id\":%d,\"based_on\":%d,\"data\":\"%s\",\"name\":\"%s\"}", 
            this->id, 
            this->node_id, 
            this->instance, 
            this->minimum, 
            this->maximum, 
            this->current_value, 
            this->target_value, 
            this->last_value,
            this->unit.c_str(),
            this->step_value, 
            this->editable, 
            this->type, 
            this->state, 
            this->last_changed, 
            this->changed_by, 
            this->changed_by_id, 
            this->based_on, 
            this->data.c_str(), 
            this->name.c_str());
    };
};

class nodeAttributes
{
    using callbackFunction = void (*)(nodeAttributes *changedAttribute);

public:
    nodeAttributes(uint16_t _type);
    nodeAttributes(uint16_t _type, uint32_t _id);
    ~nodeAttributes();

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
    String getData();
    void setData(String _data);
    void setStepValue(double_t _step_value);
    double_t getStepValue();
    void setState(uint8_t _state) { this->value.state = _state; }
    uint8_t getState() { return this->value.state; }

    void setCallback(callbackFunction _callback);

private:
    callbackFunction callback = nullptr;

    nodeAttributesValues value;

    uint32_t getTimestamp();

    void executeCallback();

    friend class node;
    friend class virtualHomee;
    friend struct nodeValues;
};
