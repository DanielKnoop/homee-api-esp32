#include "virtualHomee.hpp"

namespace {
    class SharedBufferPrint : public Print {
        std::vector<uint8_t>& _buf;
    public:
        explicit SharedBufferPrint(std::vector<uint8_t>& buf) : _buf(buf) {}
        size_t write(uint8_t c) override {
            _buf.push_back(c);
            return 1;
        }
        size_t write(const uint8_t* data, size_t len) override {
            _buf.insert(_buf.end(), data, data + len);
            return len;
        }
    };
}

void virtualHomee::getSettings(JsonObject jsonDoc)
{
    jsonDoc["settings"]["address"] = "";
    jsonDoc["settings"]["city"] = "";
    jsonDoc["settings"]["zip"] = 11111;
    jsonDoc["settings"]["state"] = F("BW");
    jsonDoc["settings"]["latitude"] = "";
    jsonDoc["settings"]["longitude"] = "";
    jsonDoc["settings"]["country"] = F("Germany");
    jsonDoc["settings"]["language"] = F("de");
    jsonDoc["settings"]["wlan_dhcp"] = 1;
    jsonDoc["settings"]["remote_access"] = 1;
    jsonDoc["settings"]["beta"] = 0;
    jsonDoc["settings"]["webhooks_key"] = F("WEBHOOKKEY");
    jsonDoc["settings"]["automatic_location_detection"] = 0;
    jsonDoc["settings"]["polling_interval"] = 60;
    jsonDoc["settings"]["timezone"] = F("Europe%2FBerlin");
    jsonDoc["settings"]["enable_analytics"] = 0;
    jsonDoc["settings"]["wlan_enabled"] = 1;
    jsonDoc["settings"]["wlan_ip_address"] = F("192.168.178.222");
    jsonDoc["settings"]["wlan_ssid"] = F("homeeWifi");
    jsonDoc["settings"]["wlan_mode"] = 2;
    jsonDoc["settings"]["online"] = 0;
    jsonDoc["settings"]["lan_enabled"] = 1;
    jsonDoc["settings"]["available_ssids"].to<JsonArray>().add("homeeWifi");
    jsonDoc["settings"]["time"] = 1562707105;
    jsonDoc["settings"]["civil_time"] = F("2019-07-09 23:18:25");
    jsonDoc["settings"]["version"] = this->version;
    jsonDoc["settings"]["uid"] = this->homeeId;
    jsonDoc["settings"]["gateway_id"] = 1313337;
    jsonDoc["settings"]["local_ssl_enabled"] = false;
    jsonDoc["settings"]["b2b_partner"] = F("homee");
    jsonDoc["settings"]["homee_name"] = this->homeeId;
    jsonDoc["settings"]["cubes"].to<JsonArray>();

}

void virtualHomee::addNode(node *n)
{
    nds.AddNode(n);
}

node* virtualHomee::getNodeById(int32_t node_id)
{
    return nds.GetNodeById(node_id);
}


void virtualHomee::updateAttribute(nodeAttributes *_nodeAttribute)
{
    ws.cleanupClients();
    if (ws.count() == 0) return;

    VHIH_MUTEX_TAKE(_mutex);
    JsonDocument doc;
    _nodeAttribute->GetJSONObject(doc["attribute"].to<JsonObject>());
    _txBuf.clear();
    _txBuf.reserve(measureJson(doc));
    SharedBufferPrint out(_txBuf);
    serializeJson(doc, out);
#ifdef DEBUG_VIRTUAL_HOMEE
    Serial.printf("DEBUG updateAttribute: clients=%u len=%u\n", ws.count(), _txBuf.size());
    Serial.write(_txBuf.data(), _txBuf.size());
    Serial.println();
#endif
    ws.textAll(_txBuf.data(), _txBuf.size());
    VHIH_MUTEX_GIVE(_mutex);
}

void virtualHomee::updateNode(node* _node)
{
    ws.cleanupClients();
    if (ws.count() == 0) return;

    VHIH_MUTEX_TAKE(_mutex);
    JsonDocument doc;
    _node->AddJSONObject(doc["node"].to<JsonObject>());
    _txBuf.clear();
    _txBuf.reserve(measureJson(doc));
    SharedBufferPrint out(_txBuf);
    serializeJson(doc, out);
    ws.textAll(_txBuf.data(), _txBuf.size());
    VHIH_MUTEX_GIVE(_mutex);
}

String virtualHomee::getUrlParameterValue(const String& url, const String& parameterName)
{
    int index = url.indexOf(parameterName + "=");
    String substr = url.substring(index + parameterName.length() + 1);
    index = substr.indexOf("&");
    if (index < 0)
    {
        return substr;
    }
    else
    {
        return substr.substring(0, index);
    }
}

nodeAttributes* virtualHomee::getAttributeById(uint32_t _id)
{
    for(int i = 0; i < this->nds.GetNumberOfNodes(); i++)
    {
        for(int j = 0; j < this->nds.GetNode(i)->GetNumberOfAttributes(); j++)
        {
            if(this->nds.GetNode(i)->GetAttribute(j)->getId() == _id)
            {
                return this->nds.GetNode(i)->GetAttribute(j);
            }
        }
    }
    return nullptr;
}

void virtualHomee::handleHttpOptionsAccessToken(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse(204);
    response->addHeader("access-control-allow-methods", "POST, DELETE");
    response->addHeader("access-control-allow-origin", "*");
    request->send(response);
}

void virtualHomee::handleHttpPostRequest(virtualHomee* context, AsyncWebServerRequest *request)
{ 
    char _buff[128];

    sprintf_P(_buff, PSTR("access_token=%s&user_id=1&device_id=1&expires=31536000"), context->access_token);
    AsyncWebServerResponse *response = request->beginResponse(200, "application/x-www-form-urlencoded", _buff);

    sprintf_P(_buff, PSTR("access_token=%s;Max-Age=2592000;"), context->access_token);
    response->addHeader("set-cookie", _buff);

    request->send(response);
}

void virtualHomee::initializeWebServer()
{
    server.on("/access_token", HTTP_OPTIONS, handleHttpOptionsAccessToken);
    server.on("/access_token", HTTP_DELETE, [](AsyncWebServerRequest *request) {});
    server.on("/access_token", HTTP_POST, [this](AsyncWebServerRequest *request){handleHttpPostRequest(this, request);});
}
void virtualHomee::initializeWebsocketServer()
{
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
    {
        if (type == WS_EVT_CONNECT)
        {
            this->clientConnected();
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
            Serial.println(client->remoteIP());
#endif
        }
        else if (type == WS_EVT_DISCONNECT)
        {
            this->clientDisconnected();
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.printf("ws[%s] disconnect: %u\n", server->url(), client->id());
#endif
        }
        else if (type == WS_EVT_PONG)
        {
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.println("PONG");
#endif
        }
        else if (type == WS_EVT_DATA)
        {
            AwsFrameInfo *info = (AwsFrameInfo *)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
            {
                data[len] = 0;
                String message = (char *)data;
#ifdef DEBUG_VIRTUAL_HOMEE
                Serial.print("DEBUG: Received Message: ");
                Serial.println(message);
#endif
                if (message.equalsIgnoreCase("GET:Settings"))
                {
                    JsonDocument doc;
                    this->getSettings(doc.to<JsonObject>());
                    this->sendWSMessage(doc, client);
                }
                else if (message.equalsIgnoreCase("GET:nodes"))
                {
                    VHIH_MUTEX_TAKE(_mutex);
                    _txBuf.clear();
                    _txBuf.reserve(nds.size());
                    SharedBufferPrint out(_txBuf);
                    out.print("{\"nodes\":[");
                    for (uint8_t i = 0; i < nds.GetNumberOfNodes(); i++) {
                        if (i > 0) out.print(',');
                        JsonDocument nodeDoc;
                        nds.GetNode(i)->AddJSONObject(nodeDoc.to<JsonObject>());
                        serializeJson(nodeDoc, out);
                    }
                    out.print("]}");
                    client->text(_txBuf.data(), _txBuf.size());
                    VHIH_MUTEX_GIVE(_mutex);
                }
                else if (message.substring(0, 9).equalsIgnoreCase("PUT:nodes")) //PUT:nodes/0/attributes?IDs=200&target_value=0.000000
                {
                    int32_t attributeId = this->getUrlParameterValue(message, "IDs").toInt();
                    double_t targetValue = this->getUrlParameterValue(message, "target_value").toDouble();
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("Attribute ID: ");
                    Serial.println(attributeId);

                    Serial.print("Target Value: ");
                    Serial.println(targetValue);
#endif
                    nodeAttributes *changedNode = this->getAttributeById(attributeId);
                    if (changedNode != nullptr)
                    {
                        changedNode->setTargetValue(targetValue);
                        changedNode->executeCallback();
                    }
#ifdef DEBUG_VIRTUAL_HOMEE
                    else
                    {
                        Serial.println("Achtung: Node nicht gefunden");
                    }
#endif
                }
                else if (message.substring(0, 10).equalsIgnoreCase("POST:nodes")) //"POST:nodes?protocol=21&compatibility_check=1&my_version=2.32.0+eb5e9b1a
                {
                    if (message.indexOf("compatibility_check=1") >= 0)
                    {
                        JsonDocument jsonDoc;
                        jsonDoc["compatibility_check"]["compatible"] = true;
                        jsonDoc["compatibility_check"]["account"] = true;
                        jsonDoc["compatibility_check"]["external_homee_status"] = F("none");
                        jsonDoc["compatibility_check"]["your_version"] = true;
                        jsonDoc["compatibility_check"]["my_version"] = this->version;
                        jsonDoc["compatibility_check"]["my_homeeID"] = this->homeeId;
                        this->sendWSMessage(jsonDoc, client);
                    }
                    else if (message.indexOf("start_pairing=1") >= 0)
                    {
                        JsonDocument jsonDoc;
                        jsonDoc["pairing"]["access_token"] = this->access_token;
                        jsonDoc["pairing"]["expires"] = 315360000;
                        jsonDoc["pairing"]["userID"] = 1;
                        jsonDoc["pairing"]["deviceID"] = 1;
                        this->sendWSMessage(jsonDoc, client);
                    }
                }
                else if (message == "DELETE:users/1/devices/1")
                {
                    JsonDocument jsonDoc;
                    jsonDoc["warning"]["code"] = 600;
                    jsonDoc["warning"]["description"] = F("Your device got removed.");
                    jsonDoc["warning"]["message"] = F("You have been logged out.");
                    jsonDoc["warning"]["data"] = serialized("{}");
                    this->sendWSMessage(jsonDoc, client);
                    client->close(4444, "DEVICE_DISCONNECT");
                }
            }
        }
        ws.cleanupClients();
    });

}

void virtualHomee::sendWSMessage(JsonDocument& doc, AsyncWebSocketClient *client)
{
#ifdef DEBUG_VIRTUAL_HOMEE
    Serial.print("DEBUG: Send Message: ");
    Serial.println(measureJson(doc));
    Serial.println();
#endif
    VHIH_MUTEX_TAKE(_mutex);
    _txBuf.clear();
    _txBuf.reserve(measureJson(doc));
    SharedBufferPrint out(_txBuf);
    serializeJson(doc, out);
    client->text(_txBuf.data(), _txBuf.size());
    VHIH_MUTEX_GIVE(_mutex);
}

void virtualHomee::start()
{
    server.begin();
    if(this->firstStart)
    {
        this->firstStart = false;
        this->startDiscoveryService();
    }
}

void virtualHomee::stop()
{
    //Disconnect all Clients
    ws.closeAll(4444, "DEVICE_DISCONNECT");
    ws.cleanupClients();
    //Stop Services
    server.end();
    //this->stopDiscoveryService();
}

void virtualHomee::startDiscoveryService()
{
    if (udp.listen(15263))
    {
#ifdef DEBUG_VIRTUAL_HOMEE
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
#endif
        udp.onPacket([this](AsyncUDPPacket packet)
        {
            char msgBuf[packet.length() + 1];
            memcpy(msgBuf, packet.data(), packet.length());
            msgBuf[packet.length()] = '\0';
            String message(msgBuf);
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.print("UDP Message reveived: ");
            Serial.println(message);
#endif

            if (message.equalsIgnoreCase(this->getHomeeId()))
            {
                String reply = "initialized:" + this->getHomeeId() + ":" + this->getHomeeId() + ":homee";
                packet.write((const uint8_t*)reply.c_str(), reply.length());
            }
        });
    }
}

//void virtualHomee::stopDiscoveryService()
//{
//    udp.close();
//}

void virtualHomee::updateAttributeValue(nodeAttributes *_attribute, double _value)
{
    _attribute->setTargetValue(_value);
    this->updateAttribute(_attribute);
    _attribute->setCurrentValue(_value);
    this->updateAttribute(_attribute);
}

void virtualHomee::updateAttributeData(nodeAttributes* _attribute, const String& _data)
{
    _attribute->setData(_data);
    this->updateAttribute(_attribute);
}

void virtualHomee::clientConnected()
{
    this->numberOfWSClients++;
}

void virtualHomee::clientDisconnected()
{
    this->numberOfWSClients--;
}

size_t virtualHomee::getNumberOfWSClients()
{
    return this->numberOfWSClients;
}

String virtualHomee::getHomeeId()
{
    return this->homeeId;
}
void virtualHomee::setHomeeId(const String& _homeeId)
{
    this->homeeId = _homeeId;
}

virtualHomee::virtualHomee(const String& _homeeId)
    : virtualHomee()
{
    this->homeeId = _homeeId;
}

virtualHomee::virtualHomee()
    : server(7681),
      ws("/connection")
{
#if defined(ESP32)
    _mutex = xSemaphoreCreateMutex();
#endif
    _txBuf.reserve(512);

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    this->homeeId = mac;
    this->version = "2.41.3+46ad073c";
    this->nds.AddNode(new node(-1, 1, "homee"));

    initializeWebServer();
    initializeWebsocketServer();
    server.addHandler(&ws);
}

virtualHomee::~virtualHomee()
{
    ws.closeAll();
#if defined(ESP32)
    if (_mutex) vSemaphoreDelete(_mutex);
#endif
}

void virtualHomee::removeNodeById(uint32_t node_id)
{
    nds.RemoveNodeById(node_id);
}