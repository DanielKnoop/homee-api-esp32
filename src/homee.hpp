#include <Arduino.h>
#include <Crypto.h>
#include <SHA512.h>
#include <base64.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>
#include <time.h>

#define HASH_SIZE 64

#ifndef homee_hpp
#define homee_hpp

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