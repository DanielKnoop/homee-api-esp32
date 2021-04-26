
#include "homee.hpp"

SHA512 sha512;
Preferences preferences;

String homee::getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void homee::hashTostring(uint8_t array[], unsigned int len, char buffer[])
{
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'a' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'a' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

void homee::hashPassword(const char* password, size_t size, char* hash) {
  uint8_t value[HASH_SIZE];
  sha512.reset();
  sha512.update(password, size);
  sha512.finalize(value, sizeof(value));
  hashTostring(value, HASH_SIZE , hash);
}

void homee::getAccessToken() {
    char hash[HASH_SIZE*2 + 1];
    hashPassword(_password, strlen(_password), hash);
    String authBuffer = String(_user) + ":" + String(hash);
    String authString = "Basic " + base64::encode(authBuffer);

    HTTPClient http;
    http.begin(String(_host), 7681, String("/access_token"));
    http.addHeader("Authorization", authString);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST("device_name=" + String(_device) 
        + "&device_hardware_id=" + String(_deviceId) + "&device_os=5&device_type=4&device_app=1}");
    if(httpResponseCode>0) { 
        String response = http.getString(); 
        //access_token=mykJNlVWED31NYkG3xKhw9xc924ABtFwQBADnmDQoo8r20lXyh3rjLaYUQWRvGMt&user_id=8&device_id=29&expires=31536000
        String access_token = getValue(getValue( response, '&', 0), '=', 1);
        long expires = getValue(getValue(response, '&', 3), '=', 1).toInt();
        _token = (char*)access_token.c_str();
        preferences.putString("access_token", access_token);
        time_t now;
        time(&now);
        
    }
    else {
        //retry ?!?
    }
}

homee::homee(const char* host, const char* user, const char* password)
{
    preferences.begin("homee-api");
    _host = (char*)host;
    _user = (char*)user;
    _password = (char*)password;
    _device = (char*)WiFi.getHostname();
    _deviceId = (char*)WiFi.getHostname();
}

homee::~homee()
{
    preferences.end();
}
