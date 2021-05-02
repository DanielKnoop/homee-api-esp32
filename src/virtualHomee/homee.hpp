#ifdef USEIT

#pragma once

#if defined(ESP32)
#include <WiFi.h>

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include <Crypto.h>
#include <SHA512.h>
#include <base64.h>
#include <Preferences.h>
#include <time.h>
#include <HTTPClient.h>

#define HASH_SIZE 64

class homee
{
private:
    char* _host;
    char* _user;
    char* _password;
    char* _token;
    char* _device;
    char* _deviceId;

    void getAccessToken();
    void hashPassword(const char* password, size_t size, char* hash);
    void hashTostring(uint8_t array[], unsigned int len, char buffer[]);
    String getValue(String data, char separator, int index);
public:
    homee(const char* host, const char* user, const char* password);
    ~homee();
};


#endif