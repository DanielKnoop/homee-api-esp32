#include "virtualHomee.hpp"

DynamicJsonDocument virtualHomee::getSettings() {
    DynamicJsonDocument doc(600);
    doc["settings"]["address"] = "";
    doc["settings"]["city"] = "";
    doc["settings"]["zip"] = 11111;
    doc["settings"]["state"] = "BW";
    doc["settings"]["latitude"] = "";
    doc["settings"]["longitude"] = "";
    doc["settings"]["country"] = "Germany";
    doc["settings"]["language"] = "de";
    doc["settings"]["wlan_dhcp"] = 1;
    doc["settings"]["remote_access"] = 1;
    doc["settings"]["beta"] = 0;
    doc["settings"]["webhooks_key"] = "WEBHOOKKEY";
    doc["settings"]["automatic_location_detection"] = 0;
    doc["settings"]["polling_interval"] = 60;
    doc["settings"]["timezone"] = "Europe%2FBerlin";
    doc["settings"]["enable_analytics"] = 0;
    doc["settings"]["wlan_enabled"] = 1;
    doc["settings"]["wlan_ip_address"] = "192.168.178.222";
    doc["settings"]["wlan_ssid"] = "homeeWifi";
    doc["settings"]["wlan_mode"] = 2;
    doc["settings"]["online"] = 0;
    doc["settings"]["lan_enabled"] = 1;
    doc["settings"].createNestedArray("available_ssids").add("homeeWifi"); // = serialized("[\"homeeWifi\"]");
    doc["settings"]["time"] = 1562707105;
    doc["settings"]["civil_time"] = "2019-07-09 23:18:25";
    doc["settings"]["version"] = this->version;
    doc["settings"]["uid"] = this->homeeId;
    doc["settings"]["gateway_id"] = 1313337;
    doc["settings"].createNestedArray("cubes");
    return doc;
}

void virtualHomee::addNode(node* n)
{
    nds.AddNode(n);
}

nodeAttributes* virtualHomee::getAttributeWithId(uint32_t id)
{
    for(uint8_t i = 0; i < nds.GetNumberOfNodes(); i++)
    {
        for(uint8_t j = 0; j <  nds.GetNode(i)->GetNumberOfAttributes(); j++)
        {
            if(nds.GetNode(i)->GetAttribute(j)->getId() == id)
            {
                return nds.GetNode(i)->GetAttribute(j);
            }
        }
    }
    return nullptr;
}

void virtualHomee::updateAttribute(nodeAttributes* _nodeAttribute)
{
    ws.cleanupClients();
    DynamicJsonDocument attributeJson = _nodeAttribute->GetJSONArray();
    DynamicJsonDocument doc(400);
    doc["attribute"] = attributeJson;

    char json[400];
    size_t len = serializeJson(doc, json, 400);
    ws.textAll(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE
    Serial.print("DEBUG: compatibility_check. JSON Length: ");
    Serial.println(len);
    Serial.println(json);
#endif
}

String virtualHomee::getUrlParameterValue(String url, String parameterName)
{
    int index=url.indexOf(parameterName + "=");
    String substr = url.substring(index + parameterName.length() + 1);
    index = substr.indexOf("&");
    if(index < 0)
    {
        return substr;
    }
    else 
    {
        return substr.substring(0, index);
    }
}

void virtualHomee::start() 
{   
    server.on("/access_token", HTTP_OPTIONS, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(204);
        response->addHeader("access-control-allow-methods", "POST, DELETE");
        response->addHeader("access-control-allow-origin", "*");
        request->send(response);
    });

    server.on("/access_token", HTTP_DELETE, [](AsyncWebServerRequest *request) {
    
    });

    server.on("/access_token", HTTP_POST, [this](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(200
            , "application/x-www-form-urlencoded"
            , "access_token=" + this->access_token + "&user_id=1&device_id=1&expires=31536000");
        response->addHeader("set-cookie", "access_token=" + this->access_token + ";Max-Age=2592000;");
        request->send(response);
    });

    ws.onEvent([this](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
        if(type == WS_EVT_CONNECT) {
#ifdef DEBUG_VIRTUAL_HOMEE
            Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
#endif
        } else if(type == WS_EVT_DISCONNECT) {
#ifdef DEBUG_VIRTUAL_HOMEE            
            Serial.printf("ws[%s] disconnect: %u\n", server->url(), client->id());
#endif
        } else if(type == WS_EVT_PONG) {
#ifdef DEBUG_VIRTUAL_HOMEE            
            Serial.println("PONG");
#endif
        } else if(type == WS_EVT_DATA) {
            AwsFrameInfo * info = (AwsFrameInfo*)arg;
            if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = 0;
                String message = (char*)data;
 #ifdef DEBUG_VIRTUAL_HOMEE               
                Serial.println(message);
#endif
                if(message.equalsIgnoreCase("GET:Settings")) {
                    DynamicJsonDocument doc = this->getSettings();
                    char json[600];
                    size_t len = serializeJson(doc, json, 600);
                    client->text(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("DEBUG: Get Settings. JSON Length: ");
                    Serial.println(len);
                    Serial.println(json);
#endif
                } else if(message.equalsIgnoreCase("GET:nodes")) { 
                    size_t s = nds.size();
                    char json[s];
                    size_t len = serializeJson(nds.GetJSONArray(), json, s);
                    client->text(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("DEBUG: get nodes. JSON Length: ");
                    Serial.println(len);
                    Serial.println(json);
#endif
                } else if(message.substring(0, 9).equalsIgnoreCase("PUT:nodes")) { //PUT:nodes/0/attributes?IDs=200&target_value=0.000000
                    int32_t attributeId = this->getUrlParameterValue(message, "IDs").toInt();
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("Attribute ID: ");
                    Serial.println(attributeId);
#endif
                    double_t targetValue = atof(this->getUrlParameterValue(message, "target_value").c_str());
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("Target Value: ");
                    Serial.println(targetValue);
#endif
                    nodeAttributes* changedNode = this->getAttributeWithId(attributeId);
                    if(changedNode != nullptr)
                    {
                        changedNode->setTargetValue(targetValue);
                    }
#ifdef DEBUG_VIRTUAL_HOMEE
                    else
                    {
                        Serial.println("Achtung: Node nicht gefunden");
                    }
#endif
                } else if(message.equalsIgnoreCase("ping")) {
                    client->text("pong") ;
                } else if(message.substring(0, 10).equalsIgnoreCase("POST:nodes")) { //"POST:nodes?protocol=21&compatibility_check=1&my_version=2.32.0+eb5e9b1a

                    if(message.indexOf("compatibility_check=1") >= 0)
                    {
                        StaticJsonDocument<200> doc;
                        doc["compatibility_check"]["compatible"] = true;
                        doc["compatibility_check"]["account"] = true;
                        doc["compatibility_check"]["external_homee_status"] = "none";
                        doc["compatibility_check"]["your_version"] = true;
                        doc["compatibility_check"]["my_version"] = this->version;
                        doc["compatibility_check"]["my_homeeID"] = this->homeeId;

                        char json[200];
                        size_t len = serializeJson(doc, json);
                        client->text(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE                        
                        Serial.print("DEBUG: compatibility_check. JSON Length: ");
                        Serial.println(len);
                        Serial.println(json);
#endif
                    } else if(message.indexOf("start_pairing=1") >= 0)
                    {
                        StaticJsonDocument<150> doc;
                        doc["pairing"]["access_token"] = this->access_token;
                        doc["pairing"]["expires"] = 31536000;
                        doc["pairing"]["userID"] = 1;
                        doc["pairing"]["deviceID"] = 1;

                        char json[150];
                        size_t len = serializeJson(doc, json);
                        client->text(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE
                        Serial.print("DEBUG: start_pairing. JSON Length: ");
                        Serial.println(len);
                        Serial.println(json);
#endif
                    }

                } else if(message == "DELETE:users/1/devices/1") {
                    StaticJsonDocument<150> doc;
                    doc["warning"]["code"] = 600;
                    doc["warning"]["description"] = "Your device got removed.";
                    doc["warning"]["message"] = "You have been logged out.";
                    doc["warning"]["data"] = serialized("{}");

                    char json[150];
                    size_t len = serializeJson(doc, json);
                    client->text(json, len);
#ifdef DEBUG_VIRTUAL_HOMEE
                    Serial.print("DEBUG: Delete User. JSON Length: ");
                    Serial.println(len);
                    Serial.println(json);
#endif
                }
                else {
                    
                }
            }   
        }
        ws.cleanupClients();
    });

    server.addHandler(&ws);
    server.begin();
    this->startDiscovery();
}

void virtualHomee::startDiscovery()
{
    if(udp.listen(15263))
    {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([this](AsyncUDPPacket packet) 
        {
            String message = packet.readString();
            Serial.print("UDP Message reveived: ");
            Serial.println(message);

            if(message.equalsIgnoreCase(this->gethomeeId()))
            {
                packet.printf("initialized:%s:%s:homee", this->gethomeeId().c_str(), this->gethomeeId().c_str());
            }
        });
    }
}

String virtualHomee::gethomeeId()
{
    return this->homeeId;
}

virtualHomee::virtualHomee(String _homeeId) 
    : virtualHomee()
{
    this->homeeId = _homeeId;
}

virtualHomee::virtualHomee()
    : server(7681),
    ws("/connection")
{
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    this->homeeId = mac;
    this->version = "2.25.0 (ed9c50)";
    this->access_token = "iK8sd0SmfulPqbnsXYqqzebLrGb0tWjaNKFmt7jHfrz1Fkj1aRwJWWc7uFnElKjs";
    this->nds.AddNode(new node(-1, 1, "homee"));
}

virtualHomee::~virtualHomee()
{
    ws.closeAll();
}