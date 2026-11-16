#include "nodeattributes.hpp"

namespace {

// Store values that are exactly integral as integers rather than floats.
//
// Two reasons. First, ArduinoJson's default exponentiation threshold is 1e7,
// so a double like the color maximum 16777215 would otherwise serialize as
// "1.677722e7" — scientific notation, and rounded to 16777220 on top of it.
// Second, and worse, ArduinoJson 7.4.3 stores any double that fits exactly in
// a float as a float, which leaves decomposeFloat() with six decimal places;
// it subtracts one per integral digit, so eight or more digits drive the count
// negative and writeDecimals() overruns its stack buffer. Integers take
// writeInteger() instead and sidestep both problems.
//
// JSON does not distinguish 21 from 21.0, and ArduinoJson already strips
// trailing zeros, so the emitted shape is unchanged for ordinary values.
inline void setNumber(JsonObject doc, const char* key, double_t value)
{
    // Range check first — casting an out-of-range double to int32_t is UB.
    if (value >= -2147483648.0 && value <= 2147483647.0 &&
        value == static_cast<double_t>(static_cast<int32_t>(value)))
    {
        doc[key] = static_cast<int32_t>(value);
    }
    else
    {
        doc[key] = value;
    }
}

}  // namespace

void nodeAttributes::GetJSONObject(JsonObject doc)
{
    doc["id"] = this->id;
    doc["node_id"] = this->node_id;
    doc["instance"] = this->instance;
    setNumber(doc, "minimum", this->minimum);
    setNumber(doc, "maximum", this->maximum);
    setNumber(doc, "current_value", this->current_value);
    setNumber(doc, "target_value", this->target_value);
    setNumber(doc, "last_value", this->last_value);
    doc["unit"] = this->unit;
    setNumber(doc, "step_value", this->step_value);
    doc["editable"] = this->editable;
    doc["type"] = this->type;
    doc["state"] = this->state;
    doc["last_changed"] = this->last_changed;
    doc["changed_by"] = this->changed_by;
    doc["changed_by_id"] = this->changed_by_id;
    doc["based_on"] = this->based_on;
    doc["data"] = this->data;
    // Only emitted when set — an empty "name" on every attribute would inflate
    // the GET:nodes payload for no benefit.
    if (this->name.length() > 0) {
        doc["name"] = this->name;
    }
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
    return 350 + this->data.length() + this->unit.length() + this->name.length();
}

void nodeAttributes::setName(const String& _name)
{
    this->name = _name;
}

String nodeAttributes::getName()
{
    return this->name;
}

void nodeAttributes::setMinimumValue(double_t _min)
{
    this->minimum = _min;
}

void nodeAttributes::setMaximumValue(double_t _max)
{
    this->maximum = _max;
}

void nodeAttributes::setUnit(const String& _unit)
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

uint32_t nodeAttributes::getTimestamp()
{
    return (uint32_t)time(NULL);
}

void nodeAttributes::setCurrentValue(double_t _currentValue)
{
    this->last_value = this->current_value;
    this->current_value = _currentValue;
    uint32_t ts = getTimestamp();
    if (ts > this->last_changed) {
        this->last_changed = ts;
    }
}

void nodeAttributes::setEditable(uint8_t _editable)
{
    this->editable = _editable;
}

void nodeAttributes::setTargetValue(double_t _targetValue)
{
    this->target_value = _targetValue;
}

double_t nodeAttributes::getTargetValue()
{
    return this->target_value;
}

String nodeAttributes::getData()
{
    return this->data;
}

void nodeAttributes::setData(const String& _data)
{
    this->data = _data;
}

void nodeAttributes::setCallback(callbackFunction _callback)
{
    this->callback = _callback;
}

double_t nodeAttributes::getStepValue()
{
    return this->step_value;
}

void nodeAttributes::setStepValue(double_t _step_value)
{
    this->step_value = _step_value;
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