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
#include <vector>

#if defined(ESP32)
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <atomic>
#define VHIH_MUTEX_TAKE(m) xSemaphoreTake(m, portMAX_DELAY)
#define VHIH_MUTEX_GIVE(m) xSemaphoreGive(m)
#else
// ESP8266 is single-core — no mutex needed
#define VHIH_MUTEX_TAKE(m) (void)0
#define VHIH_MUTEX_GIVE(m) (void)0
#endif

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

    // Reusable TX buffer — protected by _mutex on ESP32.
    // Eliminates per-message heap allocation in the hot path.
#if defined(ESP32)
    SemaphoreHandle_t _mutex = nullptr;
    std::atomic<size_t> numberOfWSClients{0};
#else
    size_t numberOfWSClients = 0;
#endif
    std::vector<uint8_t> _txBuf;

    void getSettings(JsonObject jsonDoc);
    void startDiscoveryService();
    //void stopDiscoveryService();
    String getUrlParameterValue(const String& url,const String& parameterName);
    void sendWSMessage(JsonDocument& doc, AsyncWebSocketClient *client);
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
