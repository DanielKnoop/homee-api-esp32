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

#include "virtualHomee/node.hpp"

#include "virtualHomee/MeasureBuffer.h"
#include "virtualHomee/WriteBuffer.h"

struct virtualHomeeValues
{
    String homeeId;
    String version;
    const char *access_token = "iK8sd0SmfulPqbnsXYqqzebLrGb0tWjaNKFmt7jHfrz1Fkj1aRwJWWc7uFnElKjs";

    node *nodes[MAX_NUMBER_OF_NODES];
    uint8_t numberOfNodes = 0;
};
class virtualHomee
{
public:
    void start();
    void addNode(node *n);
    node *getNodeById(int32_t node_id);
    node *getNode(uint8_t n);
    uint8_t GetNumberOfNodes();
    nodeAttributes *getAttributeById(uint32_t _id);
    String getHomeeId();
    void setHomeeId(const String &_homeeId);

    void updateAttribute(nodeAttributes *_nodeAttribute);
    void updateAttributeValue(nodeAttributes *_nodeAttribute, double _newValue);
    void updateAttributeData(nodeAttributes *_nodeAttribute, const String &_data);
    void updateNode(node *_node);

    size_t getNumberOfWSClients();

    size_t measureSerializeNodes();
    void serializeNodes(Print &outputStream);

    virtualHomee();
    virtualHomee(const String &_homeeId);
    ~virtualHomee();

private:
    virtualHomeeValues value;

    AsyncWebServer server;
    AsyncWebSocket ws;
    AsyncUDP udp;

    void startDiscoveryService();
    String getUrlParameterValue(const String &url, const String &parameterName);
    String gethomeeId();
    void sendWSMessage(AsyncWebSocketJsonBuffer *jsonBuffer, AsyncWebSocketClient *client);
    size_t numberOfWSClients = 0;
    void clientConnected();
    void clientDisconnected();
    void initializeWebServer();
    void initializeWebsocketServer();
    static void handleHttpPostRequest(virtualHomee *context, AsyncWebServerRequest *request);
    static void handleHttpOptionsAccessToken(AsyncWebServerRequest *request);
};
