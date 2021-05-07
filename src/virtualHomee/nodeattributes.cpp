#include "nodeattributes.hpp"

DynamicJsonDocument nodeAttributes::GetJSONArray()
{
    DynamicJsonDocument doc(this->size());

    doc["id"] = this->id;
    doc["node_id"] = this->node_id;
    doc["instance"] = this->instance;
    doc["minimum"] = this->minimum;
    doc["maximum"] = this->maximum;
    doc["current_value"] = this->current_value;
    doc["target_value"] = this->target_value;
    doc["last_value"] = this->last_value;
    doc["unit"] = this->unit;
    doc["step_value"] = this->step_value;
    doc["editable"] = this->editable;
    doc["type"] = this->type;
    doc["state"] = this->state;
    doc["last_changed"] = this->last_changed;
    doc["changed_by"] = this->changed_by;
    doc["changed_by_id"] = this->changed_by_id;
    doc["based_on"] = this->based_on;
    doc["data"] = this->data;
    doc["name"] = this->name;

    return doc;
}

double_t nodeAttributes::getMinimumValue()
{
    return this->minimum;
}

double_t nodeAttributes::getMaximumValue()
{
    return this->maximum;
}

double_t nodeAttributes::getCurrentValue()
{
    return this->current_value;
}

size_t nodeAttributes::size()
{
    return 350 + this->name.length() + this->data.length() + this->unit.length();
}

void nodeAttributes::setMinimumValue(double_t _min)
{
    this->minimum = _min;
}

void nodeAttributes::setMaximumValue(double_t _max)
{
    this->maximum = _max;
}

void nodeAttributes::setUnit(String _unit)
{
    this->unit = _unit;
}

void nodeAttributes::setInstance(uint8_t _instance)
{
    this->instance = _instance;
}

uint16_t nodeAttributes::getType()
{
    return this->type;
}

nodeAttributes::nodeAttributes(uint16_t _type)
{
    this->type = _type;
}

nodeAttributes::nodeAttributes(uint16_t _type, uint32_t _id)
{
    this->id = _id;
    this->type = _type;
}

void nodeAttributes::setId(uint32_t _id)
{
    this->id = _id;
}

uint32_t nodeAttributes::getId()
{
    return this->id;
}

void nodeAttributes::setNodeId(uint32_t _node_id)
{
    this->node_id = _node_id;
}

uint32_t nodeAttributes::getNodeId()
{
    return this->node_id;
}

void nodeAttributes::setCurrentValue(double_t _currentValue)
{
    this->current_value = _currentValue;
}

void nodeAttributes::setEditable(uint8_t _editable)
{
    this->editable = _editable;
}

void nodeAttributes::setTargetValue(double_t _targetValue)
{
    this->target_value = _targetValue;

    if(this->callback)
    {
        callback(this);
    }
}

double_t nodeAttributes::getTargetValue()
{
    return this->target_value;
}

void nodeAttributes::setCallback(callbackFunction _callback)
{
    this->callback = _callback;
}