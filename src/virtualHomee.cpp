#include "virtualHomee.hpp"

void virtualHomee::setSettings() {
    DynamicJsonDocument doc(1024);
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
    doc["settings"]["available_ssids"] = serialized("[\"homeeWifi\"]");
    doc["settings"]["time"] = 1562707105;
    doc["settings"]["civil_time"] = "2019-07-09 23:18:25";
    doc["settings"]["version"] = this->version;
    doc["settings"]["uid"] = this->homeeId;
    doc["settings"]["gateway_id"] = 1313337;
    doc["settings"]["cubes"] = serialized("[]");
    doc["settings"]["extensions"] = serialized("{   \
        \"weather\": {                      \
            \"enabled\": 1                 \
        },                              \
        \"amazon_alexa\": {                 \
            \"enabled\": 0                 \
        },                              \
        \"google_assistant\": {             \
            \"enabled\": 0,                 \
            \"syncing\": 0                 \
        },                              \
        \"apple_homekit\": {                \
            \"enabled\": 0,                 \
            \"paired\": 0,                  \
            \"config_number\": 1,           \
            \"syncing\": 0                 \
        },                              \
        \"ftp\": {                          \
            \"enabled\": 0                 \
        },                              \
        \"history\": {                      \
            \"enabled\": 0                 \
        },                              \
        \"backup\": {                       \
            \"enabled\": 0                 \
        }}");
    String json;
    
    serializeJson(doc, json);
    settings = json;
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
    DynamicJsonDocument attributeJson = _nodeAttribute->GetJSONArray();
    DynamicJsonDocument doc(512 + 32);
    doc["attribute"] = attributeJson;

    String json;
    serializeJson(doc, json);
    Serial.println(json);
    ws.textAll(json);
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
        Serial.println("OPTIONS Request");
    });

    server.on("/access_token", HTTP_DELETE, [](AsyncWebServerRequest *request) {
    
    });

    server.on("/access_token", HTTP_POST, [this](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse(200
            , "application/x-www-form-urlencoded"
            , "access_token=" + this->access_token + "&user_id=1&device_id=1&expires=31536000");
        response->addHeader("set-cookie", "access_token=" + this->access_token + ";Max-Age=2592000;");
        request->send(response);
        Serial.println(request->url());
    });
    ws.onEvent([this](AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
        if(type == WS_EVT_CONNECT) {
            //client connected
            Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
            //client->printf("Hello Client %u :)", client->id());
            //client->ping();
        } else if(type == WS_EVT_DISCONNECT) {
            //client disconnected
            Serial.printf("ws[%s] disconnect: %u\n", server->url(), client->id());
        } else if(type == WS_EVT_PONG) {
            Serial.println("PONG");
        } else if(type == WS_EVT_DATA) {
            AwsFrameInfo * info = (AwsFrameInfo*)arg;
            if(info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
                data[len] = 0;
                String message = (char*)data;
                Serial.println(message);
                if(message.equalsIgnoreCase("GET:Settings")) {
                    client->text(settings);
                } else if(message.equalsIgnoreCase("GET:nodes")) { 
                    String json;
                    serializeJson(nds.GetJSONArray(), json);
                    Serial.println(json);
                    client->text(json);
                } else if(message.equalsIgnoreCase("GET:all")) { 

                } else if(message.substring(0, 9).equalsIgnoreCase("PUT:nodes")) { //PUT:nodes/0/attributes?IDs=200&target_value=0.000000
                    int32_t attributeId = this->getUrlParameterValue(message, "IDs").toInt();
                    double_t targetValue = atof(this->getUrlParameterValue(message, "target_value").c_str());

                    nodeAttributes* changedNode = this->getAttributeWithId(attributeId);
                    changedNode->setTargetValue(targetValue);
                } else if(message.equalsIgnoreCase("ping")) {
                    client->text("pong") ;
                } else if(message.substring(0, 10).equalsIgnoreCase("POST:nodes")) { //"POST:nodes?protocol=21&compatibility_check=1&my_version=2.32.0+eb5e9b1a

                    if(message.indexOf("compatibility_check=1") >= 0)
                    {
                        DynamicJsonDocument doc(512);
                        doc["compatibility_check"]["compatible"] = true;
                        doc["compatibility_check"]["account"] = true;
                        doc["compatibility_check"]["external_homee_status"] = "none";
                        doc["compatibility_check"]["your_version"] = true;
                        doc["compatibility_check"]["my_version"] = this->version;
                        doc["compatibility_check"]["my_homeeID"] = this->homeeId;

                        String json;
                        serializeJson(doc, json);
                        Serial.println(json);

                        client->text(json);

                    } else if(message.indexOf("start_pairing=1") >= 0)
                    {
                        DynamicJsonDocument doc(512);
                        doc["pairing"]["access_token"] = this->access_token;
                        doc["pairing"]["expires"] = 31536000;
                        doc["pairing"]["userID"] = 1;
                        doc["pairing"]["deviceID"] = 1;

                        String json;
                        serializeJson(doc, json);

                        Serial.println(json);
                        client->text(json);
                    }

                } else if(message == "DELETE:users/1/devices/1") {
                    DynamicJsonDocument doc(512);
                    doc["warning"]["code"] = 600;
                    doc["warning"]["description"] = "Your device got removed.";
                    doc["warning"]["message"] = "You have been logged out.";
                    doc["warning"]["data"] = serialized("{}");

                    String json;
                    serializeJson(doc, json);
                    client->text(json);
                }
                else {
                    
                }
            }   
        }
        
    });

    server.addHandler(&ws);
    server.begin();
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
    this->setSettings();
    this->nds.AddNode(new node(-1, 1, "homee"));
}

virtualHomee::~virtualHomee()
{
    ws.closeAll();
}