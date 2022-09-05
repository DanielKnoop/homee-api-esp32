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
    std::vector<node*> nodes;

    void serialize(Print &outputStream)
    {
    outputStream.print("{\"nodes\":[");
    for (int i = 0; i < nodes.size(); i++) 
    {
        if (i > 0)
        {
            outputStream.print(',');
        }
        nodes[i]->value.serialize(outputStream);
    }
    outputStream.print("]}");        
    }
};

struct virtualHomeeSettings
{
    void serialize(Print &outputStream)
    {
        outputStream.printf(PSTR("{\"settings\":{\"address\":\"\",\"city\":\"\",\"zip\":\"\",\"state\":\"\",\"country\":\"\",\"language\":\"de\",\"remote_access\":1,\"webhooks_key\":\"\",\"automatic_location_detection\":0,\"polling_interval\":60.0,\"timezone\":\"Europe%2FBerlin\",\"homee_name\":\"WLAN\",\"latitude\":50,\"longitude\":7,\"b2b_partner\":\"homee\",\"enable_analytics\":0,\"beta\":0,\"local_ssl_enabled\":false,\"wlan_enabled\":0,\"wlan_ssid\":\"Homee\",\"wlan_mode\":2,\"internet_access\":true,\"lan_enabled\":1,\"lan_ip_address\":\"192.168.178.2\",\"available_ssids\":[],\"time\":1661883884,\"civil_time\":\"2022-08-30 20:24:4\",\"version\":\"2.37.1+75531af0\",\"uid\":\"\",\"cubes\":[],\"extensions\":{}}}"));
    }
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

    virtualHomee();
    virtualHomee(const String &_homeeId);
    ~virtualHomee();

private:
    virtualHomeeValues value;
    virtualHomeeSettings settings;

    AsyncWebServer server;
    AsyncWebSocket ws;
    AsyncUDP udp;

    void startDiscoveryService();
    String getUrlParameterValue(const String &url, const String &parameterName);
    size_t numberOfWSClients = 0;
    void clientConnected();
    void clientDisconnected();
    void initializeWebServer();
    void initializeWebsocketServer();
    static void handleHttpPostRequest(virtualHomee *context, AsyncWebServerRequest *request);
    static void handleHttpOptionsAccessToken(AsyncWebServerRequest *request);
};
