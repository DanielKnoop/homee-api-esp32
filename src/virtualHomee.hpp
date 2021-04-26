#ifndef virtualHomee_hpp
#define virtualHomee_hpp

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "nodes.hpp"

class virtualHomee
{
private:
    String homeeId;
    String version;
    String settings;
    String access_token;

    AsyncWebServer server;
    AsyncWebSocket ws;

    nodes nds;

    void setSettings();

    nodeAttributes* getAttributeWithId(uint32_t id);
    String getUrlParameterValue(String url, String parameterName);

public:
    void start();
    void addNode(node* n);
    void updateAttribute(nodeAttributes* _nodeAttribute);

    virtualHomee();
    ~virtualHomee();
};

#endif