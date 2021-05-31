#if  defined(EXAMPLE_BUILD)

#include <Arduino.h>
#include "virtualHomee.hpp"

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#ifndef pSSID
  #define pSSID "WLAN"
#endif

#ifndef pWLANPASSWORD
  #define pWLANPASSWORD "PASSWORD"
#endif

const char* ssid = pSSID;
const char* password = pWLANPASSWORD;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

virtualHomee vhih("esp1");

nodeAttributes* na1;
nodeAttributes* na2;

nodeAttributes* schalterAttribute;

void setup() {
  // put your setup code here, to run once:
  //homee homee("homee-mac", "Benutzer", "Password");
  randomSeed(analogRead(0));

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  node* n1 = new node(10, 3001, "Luftsensor"); //CANodeProfileTemperatureAndHumiditySensor
  na1 = n1->AddAttributes(new nodeAttributes(5)); //CAAttributeTypeTemperature
  na2 = n1->AddAttributes(new nodeAttributes(7)); //CAAttributeTypeRelativeHumidity

  na1->setUnit("°C");
  na1->setMinimumValue(-20);
  na1->setMaximumValue(60);
  na1->setCurrentValue(21);
  na2->setUnit("%");
  

  node* n2 = new node(20, 10, "Schalter");
  schalterAttribute = n2->AddAttributes(new nodeAttributes(1, 200));
  schalterAttribute->setEditable(1);
  schalterAttribute->setMinimumValue(0);
  schalterAttribute->setMaximumValue(1);
  schalterAttribute->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });

  vhih.addNode(n1);
  vhih.addNode(n2);

  vhih.start();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

u_long lastMillis = 0;
u_long duration = 30000;

void loop() {
  u_long currentMillis = millis();
  if(lastMillis + duration < currentMillis)
  {
    lastMillis = currentMillis;
    double_t randValue = random(-15, 15) / 10.0;
    double_t newValue = na1->getCurrentValue() + randValue;
    if(newValue < na1->getMinimumValue())
    {
      newValue = na1->getMinimumValue();
    }
    if(newValue > na1->getMaximumValue())
    {
      newValue = na1->getMaximumValue();
    }
    vhih.updateAttributeValue(na1, newValue);
    time_t now = time(&now);
    Serial.print("Timestamp: ");
    Serial.println((uint32_t) time(&now));
  }
}
#endif