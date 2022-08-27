#include "nodeattributes.hpp"

double_t nodeAttributes::getMinimumValue()
{
    return this->value.minimum;
}

double_t nodeAttributes::getMaximumValue()
{
    return this->value.maximum;
}

double_t nodeAttributes::getCurrentValue()
{
    return this->value.current_value;
}

void nodeAttributes::setMinimumValue(double_t _min)
{
    this->value.minimum = _min;
}

void nodeAttributes::setMaximumValue(double_t _max)
{
    this->value.maximum = _max;
}

void nodeAttributes::setUnit(String _unit)
{
    this->value.unit = _unit;
}

void nodeAttributes::setInstance(uint8_t _instance)
{
    this->value.instance = _instance;
}

uint16_t nodeAttributes::getType()
{
    return this->value.type;
}

nodeAttributes::nodeAttributes(uint16_t _type)
{
    this->value.type = _type;
}

nodeAttributes::nodeAttributes(uint16_t _type, uint32_t _id)
{
    this->value.id = _id;
    this->value.type = _type;
}

void nodeAttributes::setId(uint32_t _id)
{
    this->value.id = _id;
}

uint32_t nodeAttributes::getId()
{
    return this->value.id;
}

void nodeAttributes::setNodeId(uint32_t _node_id)
{
    this->value.node_id = _node_id;
}

uint32_t nodeAttributes::getNodeId()
{
    return this->value.node_id;
}

uint32_t nodeAttributes::getTimestamp()
{
    time_t now = time(&now);
    return time(&now);
}

void nodeAttributes::setCurrentValue(double_t _currentValue)
{
    this->value.last_value = this->value.current_value;
    this->value.current_value = _currentValue;
    this->value.last_changed = getTimestamp();
}

void nodeAttributes::setEditable(uint8_t _editable)
{
    this->value.editable = _editable;
}

void nodeAttributes::setTargetValue(double_t _targetValue)
{
    this->value.target_value = _targetValue;
}

double_t nodeAttributes::getTargetValue()
{
    return this->value.target_value;
}

String nodeAttributes::getData()
{
    return this->value.data;
}

void nodeAttributes::setData(String _data)
{
    this->value.data = _data;
}

void nodeAttributes::setCallback(callbackFunction _callback)
{
    this->callback = _callback;
}

double_t nodeAttributes::getStepValue()
{
    return this->value.step_value;
}

void nodeAttributes::setStepValue(double_t _step_value)
{
    this->value.step_value = _step_value;
}

void nodeAttributes::executeCallback()
{
    if (this->callback)
    {
        callback(this);
    }
}

nodeAttributes::~nodeAttributes()
{
    callback = nullptr;
}