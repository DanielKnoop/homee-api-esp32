#include "nodeattributes.hpp"

void nodeAttributes::GetJSONObject(JsonObject doc)
{
    doc["id"] = this->value.id;
    doc["node_id"] = this->value.node_id;
    doc["instance"] = this->value.instance;
    doc["minimum"] = this->value.minimum;
    doc["maximum"] = this->value.maximum;
    doc["current_value"] = this->value.current_value;
    doc["target_value"] = this->value.target_value;
    doc["last_value"] = this->value.last_value;
    doc["unit"] = this->value.unit;
    doc["step_value"] = this->value.step_value;
    doc["editable"] = this->value.editable;
    doc["type"] = this->value.type;
    doc["state"] = this->value.state;
    doc["last_changed"] = this->value.last_changed;
    doc["changed_by"] = this->value.changed_by;
    doc["changed_by_id"] = this->value.changed_by_id;
    doc["based_on"] = this->value.based_on;
    doc["data"] = this->value.data;
    doc["name"] = this->value.name;
}

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

size_t nodeAttributes::size()
{
    return 350 + this->value.name.length() + this->value.data.length() + this->value.unit.length();
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

void nodeAttributes::serializeAttributes(Print& outputStream)
{
    //{"id":1000,"node_id":10,"instance":0,"minimum":-20,"maximum":60,"current_value":20.6,"target_value":20.6,"last_value":21,"unit":"°C","step_value":1,"editable":0,"type":5,"state":1,"last_changed":1661447550,"changed_by":1,"changed_by_id":0,"based_on":1,"data":"","name":""}

    outputStream.printf("{\"id\":%d,\"node_id\":%d,\"instance\":%d,\"minimum\":%f,\"maximum\":%f,\"current_value\":%f,\"target_value\":%f,\"last_value\":%f,\"unit\":\"%s\",\"step_value\":%f,\"editable\":%d,\"type\":%d,\"state\":%d,\"last_changed\":%d,\"changed_by\":%d,\"changed_by_id\":%d,\"based_on\":%d,\"data\":\"%s\",\"name\":\"%s\"}", 
        this->value.id, 
        this->value.node_id, 
        this->value.instance, 
        this->value.minimum, 
        this->value.maximum, 
        this->value.current_value, 
        this->value.target_value, 
        this->value.last_value,
        this->value.unit.c_str(),
        this->value.step_value, 
        this->value.editable, 
        this->value.type, 
        this->value.state, 
        this->value.last_changed, 
        this->value.changed_by, 
        this->value.changed_by_id, 
        this->value.based_on, 
        this->value.data.c_str(), 
        this->value.name.c_str());
}