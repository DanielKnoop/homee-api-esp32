#include "virtualHomee.hpp"

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
    jsonDoc["settings"].createNestedArray("available_ssids").add("homeeWifi");
    jsonDoc["settings"]["time"] = 1562707105;
    jsonDoc["settings"]["civil_time"] = F("2019-07-09 23:18:25");
    jsonDoc["settings"]["version"] = this->value.version;
    jsonDoc["settings"]["uid"] = this->value.homeeId;
    jsonDoc["settings"]["gateway_id"] = 1313337;
    jsonDoc["settings"]["local_ssl_enabled"] = false;
    jsonDoc["settings"]["b2b_partner"] = F("homee");
    jsonDoc["settings"]["homee_name"] = this->value.homeeId;
    jsonDoc["settings"].createNestedArray("cubes");

}

uint8_t virtualHomee::GetNumberOfNodes()
{
    return this->value.numberOfNodes;
}

void virtualHomee::addNode(node* n)
{
    if(this->value.numberOfNodes > MAX_NUMBER_OF_NODES)
        return;
    this->value.nodes[this->value.numberOfNodes++] = n;
}

node* virtualHomee::getNodeById(int32_t node_id)
{
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        if(this->getNode(i)->getId() == node_id)
        {
            return this->getNode(i);
        }
    }
    return nullptr;
}

nodeAttributes *virtualHomee::getAttributeWithId(uint32_t id)
{
    for (uint8_t i = 0; i < this->GetNumberOfNodes(); i++)
    {
        for (uint8_t j = 0; j < this->getNode(i)->GetNumberOfAttributes(); j++)
        {
            if (this->getNode(i)->GetAttribute(j)->getId() == id)
            {
                return this->getNode(i)->GetAttribute(j);
            }
        }
    }
    return nullptr;
}

node *virtualHomee::getNode(uint8_t n)
{
    return this->value.nodes[n];
}

void virtualHomee::updateAttribute(nodeAttributes *_nodeAttribute)
{
    ws.cleanupClients();
    AsyncWebSocketJsonBuffer* buffer = ws.makeJsonBuffer(false, _nodeAttribute->size());
    JsonVariant doc = buffer->getRoot();
    JsonObject attribute = doc.createNestedObject("attribute");
    _nodeAttribute->GetJSONObject(attribute);
    buffer->setLength();
    ws.textAll(buffer);
}

void virtualHomee::updateNode(node* _node)
{
    ws.cleanupClients();
    AsyncWebSocketJsonBuffer* buffer = ws.makeJsonBuffer(false, _node->size());
    JsonVariant doc = buffer->getRoot();
    JsonObject node = doc.createNestedObject("node");
    _node->AddJSONObject(node);
    buffer->setLength();
    ws.textAll(buffer);
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
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        for(int j = 0; j < this->getNode(i)->GetNumberOfAttributes(); j++)
        {
            if(this->getNode(i)->GetAttribute(j)->getId() == _id)
            {
                return this->getNode(i)->GetAttribute(j);
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

    sprintf_P(_buff, PSTR("access_token=%s&user_id=1&device_id=1&expires=31536000"), context->value.access_token);
    AsyncWebServerResponse *response = request->beginResponse(200, "application/x-www-form-urlencoded", _buff);

    sprintf_P(_buff, PSTR("access_token=%s;Max-Age=2592000;"), context->value.access_token);
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
                    AsyncWebSocketJsonBuffer * jsonBuffer = ws.makeJsonBuffer();
                    JsonVariant doc = jsonBuffer->getRoot();
                    this->getSettings(doc);

                    this->sendWSMessage(jsonBuffer, client);
                    
                }
                else if (message.equalsIgnoreCase("GET:nodes"))
                {  
                    size_t size = this->measureSerializeNodes();

#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("DEBUG: Reserve Buffer Size: ");
                    Serial.println(size);
#endif 
                    AsyncWebSocketMessageBuffer * buffer = ws.makeBuffer(size);
                    WriteBuffer writeBuffer(buffer->get(), buffer->length());
                    this->serializeNodes(writeBuffer);
                    client->text(buffer);
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
                    nodeAttributes *changedNode = this->getAttributeWithId(attributeId);
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
                        AsyncWebSocketJsonBuffer * jsonBuffer = ws.makeJsonBuffer(false, 200);
                        JsonVariant jsonDoc = jsonBuffer->getRoot();
                        jsonDoc["compatibility_check"]["compatible"] = true;
                        jsonDoc["compatibility_check"]["account"] = true;
                        jsonDoc["compatibility_check"]["external_homee_status"] = F("none");
                        jsonDoc["compatibility_check"]["your_version"] = true;
                        jsonDoc["compatibility_check"]["my_version"] = this->value.version;
                        jsonDoc["compatibility_check"]["my_homeeID"] = this->value.homeeId;

                        this->sendWSMessage(jsonBuffer, client);
                    }
                    else if (message.indexOf("start_pairing=1") >= 0)
                    {
                        AsyncWebSocketJsonBuffer * jsonBuffer = ws.makeJsonBuffer(false, 200);
                        JsonVariant jsonDoc = jsonBuffer->getRoot();
                        jsonDoc["pairing"]["access_token"] = this->value.access_token;
                        jsonDoc["pairing"]["expires"] = 315360000;
                        jsonDoc["pairing"]["userID"] = 1;
                        jsonDoc["pairing"]["deviceID"] = 1;

                        this->sendWSMessage(jsonBuffer, client);
                    }
                }
                else if (message == "DELETE:users/1/devices/1")
                {
                    AsyncWebSocketJsonBuffer * jsonBuffer = ws.makeJsonBuffer(false, 150);
                    JsonVariant jsonDoc = jsonBuffer->getRoot();
                    jsonDoc["warning"]["code"] = 600;
                    jsonDoc["warning"]["description"] = F("Your device got removed.");
                    jsonDoc["warning"]["message"] = F("You have been logged out.");
                    jsonDoc["warning"]["data"] = serialized("{}");
                    this->sendWSMessage(jsonBuffer, client);
                    client->close(4444, "DEVICE_DISCONNECT");
                }
            }
        }
        ws.cleanupClients();
    });

}

void virtualHomee::start()
{
    initializeWebServer();
    initializeWebsocketServer();

    server.addHandler(&ws);
    server.begin();
    this->startDiscoveryService();
}

void virtualHomee::sendWSMessage(AsyncWebSocketJsonBuffer * jsonBuffer, AsyncWebSocketClient *client)
{ 
#ifdef DEBUG_VIRTUAL_HOMEE
    Serial.print("DEBUG: Send Message: ");
    Serial.println(measureJson(jsonBuffer->getRoot()));
    //serializeJsonPretty(jsonBuffer->getRoot(), Serial);
    Serial.println();
#endif
    jsonBuffer->setLength();
    client->text(jsonBuffer);
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
            String message = packet.readString();
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.print("UDP Message reveived: ");
            Serial.println(message);
#endif

            if (message.equalsIgnoreCase(this->gethomeeId()))
            {
                packet.printf("initialized:%s:%s:homee", this->gethomeeId().c_str(), this->gethomeeId().c_str());
            }
        });
    }
}

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

String virtualHomee::gethomeeId()
{
    return this->value.homeeId;
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
    return this->value.homeeId;
}
void virtualHomee::setHomeeId(const String& _homeeId)
{
    this->value.homeeId = _homeeId;
}

virtualHomee::virtualHomee(const String& _homeeId)
    : virtualHomee()
{
    this->value.homeeId = _homeeId;
}

virtualHomee::virtualHomee()
    : server(7681),
      ws("/connection")
{
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    this->value.homeeId = mac;
    this->value.version = "2.25.0 (ed9c50)";
    this->addNode(new node(-1, 1, "homee"));
}

virtualHomee::~virtualHomee()
{
    ws.closeAll();
}

size_t virtualHomee::measureSerializeNodes()
{
    MeasureBuffer buffer;
    this->serializeNodes(buffer);

    return buffer.size();
}

void virtualHomee::serializeNodes(Print& outputStream)
{
    outputStream.print("{\"nodes\":[");
    for(int i = 0; i < this->GetNumberOfNodes(); i++)
    {
        if(i > 0)
        {
            outputStream.print(',');
        }
        this->getNode(i)->serializeNode(outputStream);
    }
    outputStream.print("]}");
}
