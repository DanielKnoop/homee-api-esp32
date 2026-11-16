#pragma once

// Minimal Arduino shim for the native (host) test environment.
//
// node, nodeAttributes and nodes depend on exactly one Arduino facility:
// the String type. They use no WiFi, no Serial, no millis() and no delay(),
// which is what makes them testable on the host in the first place.
//
// String *wraps* std::string rather than deriving from it. Deriving looks
// tempting and compiles for serialization, but it breaks deserialization:
// ArduinoJson selects its Reader by partial specialization, and a type that
// is both derived from ::String and carries std::string's const_iterator
// matches two specializations at once (ArduinoStringReader and
// IteratorReader), which is ambiguous. Wrapping exposes only what the
// library and ArduinoJson actually need.
//
// What ArduinoJson requires of this type:
//   - c_str() and length()  -> StringAdapter, for storing a String as a value
//   - operator=(const char*) and concat()
//                           -> Writer<::String>, for serializeJson(doc, str)
//
// This header is force-included via `-include Arduino.h` because the library
// headers expect String to be in scope already.

// Force-included into every translation unit, including Unity's C sources,
// so everything below is guarded: a C compiler must see an empty header.
#ifdef __cplusplus

#include <ctime>
#include <cstdint>
#include <cstring>
#include <string>

class String
{
public:
    String() = default;
    String(const char* s) : s_(s ? s : "") {}
    String(const std::string& s) : s_(s) {}

    String& operator=(const char* s)
    {
        s_.assign(s ? s : "");
        return *this;
    }

    const char* c_str() const { return s_.c_str(); }
    size_t length() const     { return s_.length(); }

    // ArduinoJson's Writer<::String> clears the destination with
    // `str = (const char*)0` and then appends through concat(), so both
    // must tolerate a null pointer.
    bool concat(const char* s)
    {
        if (s) s_.append(s);
        return true;
    }

    bool operator==(const char* rhs) const { return s_ == (rhs ? rhs : ""); }
    bool operator==(const String& rhs) const { return s_ == rhs.s_; }

    const std::string& str() const { return s_; }

private:
    std::string s_;
};

#endif  // __cplusplus
