#pragma once

#if defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#include <AsyncUDP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncUDP.h>
#endif

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <time.h>

#include "AsyncJson.h"
#include "ArduinoJson.h"
#include "virtualHomee/nodes.hpp"

class virtualHomee
{
private:
    String homeeId;
    String version;
    String access_token;

    AsyncWebServer server;
    AsyncWebSocket ws;
    AsyncUDP udp;

    nodes nds;

    DynamicJsonDocument getSettings();
    void startDiscovery();
    nodeAttributes* getAttributeWithId(uint32_t id);
    String getUrlParameterValue(String url, String parameterName);
    String gethomeeId();
    void sendWSMessage(DynamicJsonDocument doc, AsyncWebSocketClient *client);
    size_t numberOfWSClients = 0;
    void clientConnected();
    void clientDisconnected();
public:
    void start();
    void addNode(node* n);
    void updateAttribute(nodeAttributes* _nodeAttribute);
    void updateAttributeValue(nodeAttributes* _nodeAttribute, double _newValue);
    void updateAttributeData(nodeAttributes* _nodeAttribute, String _data);

    size_t getNumberOfWSClients();

    virtualHomee();
    virtualHomee(String _homeeId);
    ~virtualHomee();
};
