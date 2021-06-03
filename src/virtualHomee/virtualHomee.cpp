#include "virtualHomee.hpp"

DynamicJsonDocument virtualHomee::getSettings()
{
    DynamicJsonDocument doc(2000);
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
    doc["settings"].createNestedArray("available_ssids").add("homeeWifi");
    doc["settings"]["time"] = 1562707105;
    doc["settings"]["civil_time"] = "2019-07-09 23:18:25";
    doc["settings"]["version"] = this->version;
    doc["settings"]["uid"] = this->homeeId;
    doc["settings"]["gateway_id"] = 1313337;
    doc["settings"]["local_ssl_enabled"] = false;
    doc["settings"]["b2b_partner"] = "homee";
    doc["settings"]["homee_name"] = this->homeeId;
    doc["settings"].createNestedArray("cubes");
    /*
    doc["settings"]["extensions"]["weather"]["enabled"] = 1;
    doc["settings"]["extensions"]["amazon_alexa"]["enabled"] = 0;
    doc["settings"]["extensions"]["google_assistant"]["enabled"] = 0;
    doc["settings"]["extensions"]["google_assistant"]["syncing"] = 0;
    doc["settings"]["extensions"]["apple_homekit"]["enabled"] = 0;
    doc["settings"]["extensions"]["apple_homekit"]["paired"] = 0;
    doc["settings"]["extensions"]["apple_homekit"]["config_number"] = 1;
    doc["settings"]["extensions"]["apple_homekit"]["syncing"] = 0;
    doc["settings"]["extensions"]["ftp"]["enabled"] = 0;
    doc["settings"]["extensions"]["history"]["enabled"] = 0;
    doc["settings"]["extensions"]["backup"]["enabled"] = 0;
    */
    return doc;
}

void virtualHomee::addNode(node *n)
{
    nds.AddNode(n);
}

nodeAttributes *virtualHomee::getAttributeWithId(uint32_t id)
{
    for (uint8_t i = 0; i < nds.GetNumberOfNodes(); i++)
    {
        for (uint8_t j = 0; j < nds.GetNode(i)->GetNumberOfAttributes(); j++)
        {
            if (nds.GetNode(i)->GetAttribute(j)->getId() == id)
            {
                return nds.GetNode(i)->GetAttribute(j);
            }
        }
    }
    return nullptr;
}

void virtualHomee::updateAttribute(nodeAttributes *_nodeAttribute)
{
    ws.cleanupClients();
    DynamicJsonDocument doc(400);
    doc["attribute"] = _nodeAttribute->GetJSONArray();
    size_t len = measureJson(doc);
    AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len);
    serializeJson(doc, buffer->get(), len + 1);
    ws.textAll(buffer);
}

String virtualHomee::getUrlParameterValue(String url, String parameterName)
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

void virtualHomee::start()
{
    server.on("/access_token", HTTP_OPTIONS, [](AsyncWebServerRequest *request)
              {
                  AsyncWebServerResponse *response = request->beginResponse(204);
                  response->addHeader("access-control-allow-methods", "POST, DELETE");
                  response->addHeader("access-control-allow-origin", "*");
                  request->send(response);
              });

    server.on("/access_token", HTTP_DELETE, [](AsyncWebServerRequest *request) {

    });

    server.on("/access_token", HTTP_POST, [this](AsyncWebServerRequest *request)
              {
                  AsyncWebServerResponse *response = request->beginResponse(200, "application/x-www-form-urlencoded", "access_token=" + this->access_token + "&user_id=1&device_id=1&expires=31536000");
                  response->addHeader("set-cookie", "access_token=" + this->access_token + ";Max-Age=2592000;");
                  request->send(response);
              });
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
               {
                   if (type == WS_EVT_CONNECT)
                   {
#ifdef DEBUG_VIRTUAL_HOMEE
                       Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
                       Serial.println(client->remoteIP());
#endif
                   }
                   else if (type == WS_EVT_DISCONNECT)
                   {
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
                               DynamicJsonDocument doc = this->getSettings();
                               this->sendWSMessage(doc, client);
                           }
                           else if (message.equalsIgnoreCase("GET:nodes"))
                           {
                               DynamicJsonDocument doc = nds.GetJSONArray();
                               this->sendWSMessage(doc, client);
                           }
                           else if (message.substring(0, 9).equalsIgnoreCase("PUT:nodes")) //PUT:nodes/0/attributes?IDs=200&target_value=0.000000
                           {
                               int32_t attributeId = this->getUrlParameterValue(message, "IDs").toInt();
                               double_t targetValue = atof(this->getUrlParameterValue(message, "target_value").c_str());
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

                                   //client->text("{ \"warning\": {\"code\":104,\"description\":\"A cube started the learn mode..\",\"message\":\"homee started the learn mode.\",\"data\":{}}}");

                                   DynamicJsonDocument doc(200);
                                   doc["compatibility_check"]["compatible"] = true;
                                   doc["compatibility_check"]["account"] = true;
                                   doc["compatibility_check"]["external_homee_status"] = "none";
                                   doc["compatibility_check"]["your_version"] = true;
                                   doc["compatibility_check"]["my_version"] = this->version;
                                   doc["compatibility_check"]["my_homeeID"] = this->homeeId;

                                   this->sendWSMessage(doc, client);
                               }
                               else if (message.indexOf("start_pairing=1") >= 0)
                               {
                                   //client->text("{ \"warning\": {\"code\":104,\"description\":\"A cube started the learn mode..\",\"message\":\"homee started the learn mode.\",\"data\":{}}}");
                                   //client->text("{\"user\":{\"id\":2,\"username\":\"External_Homee_Service_User\",\"forename\":\"\",\"surname\":\"\",\"image\":\"\",\"role\":5,\"type\":1,\"email\":\"\",\"phone\":\"\",\"added\":1622576848,\"homee_image\":\"\",\"access\":1,\"presence_detection\":false,\"cube_push_notifications\":0,\"cube_email_notifications\":0,\"cube_sms_notifications\":0,\"warning_push_notifications\":0,\"warning_email_notifications\":0,\"warning_sms_notifications\":0,\"node_push_notifications\":0,\"node_email_notifications\":0,\"node_sms_notifications\":0,\"update_push_notifications\":0,\"update_email_notifications\":0,\"update_sms_notifications\":0,\"homeegram_push_notifications\":0,\"homeegram_email_notifications\":0,\"homeegram_sms_notifications\":0,\"api_push_notifications\":0,\"api_email_notifications\":0,\"api_sms_notifications\":0,\"plan_push_notifications\":0,\"plan_email_notifications\":0,\"plan_sms_notifications\":0,\"devices\":[]}}");
                                   //{"pairing":{"access_token":"FJ12BiCuy8ncDClmgOFoToInESHRaOGxltQ27haENM3kALawkOxtaRUhzxXpg6q4","expires":315360000,"userID":5,"deviceID":15}}
                                   DynamicJsonDocument doc(150);
                                   doc["pairing"]["access_token"] = this->access_token;
                                   doc["pairing"]["expires"] = 315360000;
                                   doc["pairing"]["userID"] = 2;
                                   doc["pairing"]["deviceID"] = 2;

                                   this->sendWSMessage(doc, client);
                               }
                           }
                           else if (message == "DELETE:users/1/devices/1")
                           {
                               //client->text("{\"user\":{\"id\":1,\"username\":\"homee\",\"forename\":\"\",\"surname\":\"\",\"image\":\"\",\"role\":5,\"type\":1,\"email\":\"\",\"phone\":\"\",\"added\":1622576848,\"homee_image\":\"\",\"access\":1,\"presence_detection\":false,\"cube_push_notifications\":0,\"cube_email_notifications\":0,\"cube_sms_notifications\":0,\"warning_push_notifications\":0,\"warning_email_notifications\":0,\"warning_sms_notifications\":0,\"node_push_notifications\":0,\"node_email_notifications\":0,\"node_sms_notifications\":0,\"update_push_notifications\":0,\"update_email_notifications\":0,\"update_sms_notifications\":0,\"homeegram_push_notifications\":0,\"homeegram_email_notifications\":0,\"homeegram_sms_notifications\":0,\"api_push_notifications\":0,\"api_email_notifications\":0,\"api_sms_notifications\":0,\"plan_push_notifications\":0,\"plan_email_notifications\":0,\"plan_sms_notifications\":0,\"devices\":[]}}");

                               DynamicJsonDocument doc(150);
                               doc["warning"]["code"] = 600;
                               doc["warning"]["description"] = "Your device got removed.";
                               doc["warning"]["message"] = "You have been logged out.";
                               doc["warning"]["data"] = serialized("{}");
                               this->sendWSMessage(doc, client);
                           }
                       }
                   }
                   ws.cleanupClients();
               });

    server.addHandler(&ws);
    server.begin();
    this->startDiscovery();
}

void virtualHomee::sendWSMessage(DynamicJsonDocument doc, AsyncWebSocketClient *client)
{
    size_t len = measureJson(doc);
    AsyncWebSocketMessageBuffer *buffer = ws.makeBuffer(len);
    if (buffer)
    {
        serializeJson(doc, (char *)buffer->get(), len + 1);
        client->text(buffer);
    }

#ifdef DEBUG_VIRTUAL_HOMEE
    Serial.print("DEBUG: Send Message: ");
    Serial.println(len);
    serializeJsonPretty(doc, Serial);
    Serial.println();
#endif
}

void virtualHomee::startDiscovery()
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
    //this->version = "2.25.0 (ed9c50)";
    this->version = "2.33.0-rc.1+2a3ebfd2";
    this->access_token = "iK8sd0SmfulPqbnsXYqqzebLrGb0tWjaNKFmt7jHfrz1Fkj1aRwJWWc7uFnElKjs";
    this->nds.AddNode(new node(-1, 1, "homee"));
}

virtualHomee::~virtualHomee()
{
    ws.closeAll();
}