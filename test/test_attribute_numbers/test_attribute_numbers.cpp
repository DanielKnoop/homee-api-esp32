// Regression tests for numeric serialization in nodeAttributes::GetJSONObject().
//
// These exist because of a defect that reached production: virtualHomee.hpp
// raised ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD to 1e9 so that large
// values such as the color attribute's maximum of 16777215 would serialize in
// plain notation. With ArduinoJson 7.4.3 that setting instead overruns a stack
// buffer in writeDecimals(), producing roughly 149 bytes of malformed JSON
// containing raw stack contents in place of the expected 14.
//
// What these tests do and do not guard, verified by reintroducing each defect:
//
//   - Removing setNumber() from GetJSONObject() fails three of them, including
//     "Expected 16777215 Was 16777220" -- the rounding that 1.677722e7 implies.
//
//   - Raising the threshold again leaves them green, and that is not a gap in
//     the tests. setNumber() sends integral values through writeInteger(), so
//     the float path is never entered for them. It cannot be entered by a
//     non-integral value either: float has a 24-bit mantissa, so every
//     representable value at or above 2^23 (8388608) is already an integer,
//     while the defect needs eight integral digits, i.e. at least 1e7. Any
//     non-integral value that large is therefore stored as a double and gets
//     nine decimal places, which is safe. The default threshold is kept in
//     virtualHomee.hpp as defense in depth for code that serializes a double
//     directly; that header pulls in WiFi and cannot be compiled on the host,
//     so no test here can cover it.

#include <unity.h>
#include <cstring>
#include <ArduinoJson.h>
#include "virtualHomee/nodeattributes.hpp"

namespace {

// Serialize a single attribute the way GET:nodes does.
String serializeAttribute(nodeAttributes& a)
{
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();
    a.GetJSONObject(obj);
    String out;
    serializeJson(doc, out);
    return out;
}

bool contains(const String& haystack, const char* needle)
{
    return std::strstr(haystack.c_str(), needle) != nullptr;
}

}  // namespace

// The color attribute's maximum. Exactly representable as a float, eight
// integral digits — precisely the shape that triggered the overrun.
void test_large_integral_value_is_exact(void)
{
    nodeAttributes a(23);
    a.setMaximumValue(16777215);
    String json = serializeAttribute(a);

    TEST_ASSERT_TRUE_MESSAGE(contains(json, "\"maximum\":16777215"),
                             "maximum must serialize exactly as 16777215");
}

// 1.677722e7 would be both scientific and rounded (to 16777220).
void test_no_scientific_notation(void)
{
    const double values[] = {16777215, 16777216, 100000000, 10000000};
    for (double v : values) {
        nodeAttributes a(23);
        a.setMaximumValue(v);
        String json = serializeAttribute(a);
        TEST_ASSERT_FALSE_MESSAGE(contains(json, "e7") || contains(json, "e8"),
                                  "no value may serialize in scientific notation");
    }
}

// The guard against the buffer overrun itself: a single attribute object is a
// few hundred bytes at most. The defect produced ~135 bytes of garbage for one
// field alone.
void test_output_length_is_sane(void)
{
    nodeAttributes a(23);
    a.setMaximumValue(16777215);
    a.setMinimumValue(0);
    a.setCurrentValue(0);
    String json = serializeAttribute(a);

    TEST_ASSERT_LESS_THAN_MESSAGE(400, json.length(),
                                  "oversized output indicates a serializer overrun");
}

// Garbage bytes break the parse; valid output survives a roundtrip.
void test_output_is_valid_json(void)
{
    nodeAttributes a(23);
    a.setMaximumValue(16777215);
    String json = serializeAttribute(a);

    JsonDocument check;
    DeserializationError err = deserializeJson(check, json.c_str());
    TEST_ASSERT_EQUAL_MESSAGE(DeserializationError::Ok, err.code(),
                              "serialized attribute must parse back");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16777215, check["maximum"].as<int32_t>(),
                                    "maximum must survive the roundtrip exactly");
}

// Integral storage must not cost us fractional values.
void test_fractional_values_are_preserved(void)
{
    nodeAttributes a(2);
    a.setCurrentValue(21.37);
    a.setStepValue(0.5);
    String json = serializeAttribute(a);

    TEST_ASSERT_TRUE_MESSAGE(contains(json, "\"current_value\":21.37"),
                             "fractional current_value must keep its decimals");
    TEST_ASSERT_TRUE_MESSAGE(contains(json, "\"step_value\":0.5"),
                             "fractional step_value must keep its decimals");
}

// Negative and zero values take the same path.
void test_negative_and_zero_values(void)
{
    nodeAttributes a(5);
    a.setMinimumValue(-20);
    a.setCurrentValue(0);
    String json = serializeAttribute(a);

    TEST_ASSERT_TRUE_MESSAGE(contains(json, "\"minimum\":-20"), "minimum must be -20");
    TEST_ASSERT_TRUE_MESSAGE(contains(json, "\"current_value\":0"), "current_value must be 0");
}

// Values beyond int32 must stay on the double path rather than be cast (UB).
void test_value_outside_int32_range(void)
{
    nodeAttributes a(4);
    a.setMaximumValue(5e9);   // > INT32_MAX
    String json = serializeAttribute(a);

    JsonDocument check;
    TEST_ASSERT_EQUAL_MESSAGE(DeserializationError::Ok,
                              deserializeJson(check, json.c_str()).code(),
                              "out-of-range value must still produce valid JSON");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5e9, check["maximum"].as<double>(),
                                     "out-of-range value must keep its magnitude");
}

void setUp(void) {}
void tearDown(void) {}

int main(int, char**)
{
    UNITY_BEGIN();
    RUN_TEST(test_large_integral_value_is_exact);
    RUN_TEST(test_no_scientific_notation);
    RUN_TEST(test_output_length_is_sane);
    RUN_TEST(test_output_is_valid_json);
    RUN_TEST(test_fractional_values_are_preserved);
    RUN_TEST(test_negative_and_zero_values);
    RUN_TEST(test_value_outside_int32_range);
    return UNITY_END();
}
