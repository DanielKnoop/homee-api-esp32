#pragma once

#define ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD 1e9

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
    const char* access_token = "iK8sd0SmfulPqbnsXYqqzebLrGb0tWjaNKFmt7jHfrz1Fkj1aRwJWWc7uFnElKjs";

    AsyncWebServer server;
    AsyncWebSocket ws;
    AsyncUDP udp;

    nodes nds;
    bool firstStart = true;

    void getSettings(JsonObject jsonDoc);
    void startDiscoveryService();
    //void stopDiscoveryService();
    nodeAttributes* getAttributeWithId(uint32_t id);
    String getUrlParameterValue(const String& url,const String& parameterName);
    String gethomeeId();
    void sendWSMessage(AsyncWebSocketJsonBuffer * jsonBuffer, AsyncWebSocketClient *client);
    size_t numberOfWSClients = 0;
    void clientConnected();
    void clientDisconnected();
    void initializeWebServer();
    void initializeWebsocketServer();
    static void handleHttpPostRequest(virtualHomee* context, AsyncWebServerRequest *request);
    static void handleHttpOptionsAccessToken(AsyncWebServerRequest *request);
public:
    void start();
    void stop();
    void addNode(node* n);
    void removeNodeById(uint32_t node_id);
    node* getNodeById(int32_t node_id);
    nodeAttributes* getAttributeById(uint32_t _id); 
    void updateAttribute(nodeAttributes* _nodeAttribute);
    void updateAttributeValue(nodeAttributes* _nodeAttribute, double _newValue);
    void updateAttributeData(nodeAttributes* _nodeAttribute, const String& _data);
    void updateNode(node* _node);
    size_t getNumberOfWSClients();
    String getHomeeId();
    void setHomeeId(const String& _homeeId);

    virtualHomee();
    virtualHomee(const String& _homeeId);
    ~virtualHomee();
};
