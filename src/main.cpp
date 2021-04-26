#include <Arduino.h>
#include <WiFi.h>
//#include "homee.hpp"
#include "virtualHomee.hpp"
#include "ArduinoJson.h"
#include "nodes.hpp"

#ifndef pSSID
  #define pSSID "WLAN"
#endif

#ifndef pWLANPASSWORD
  #define pWLANPASSWORD "PASSWORD"
#endif

const char* ssid = pSSID;
const char* password = pWLANPASSWORD;

virtualHomee vhih;

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

  na1->setUnit("%C2%B0C");
  na1->setMinimumValue(-20);
  na1->setMaximumValue(60);
  na2->setUnit("%25");
  

  node* n2 = new node(20, 10, "Schalter");
  schalterAttribute = n2->AddAttributes(new nodeAttributes(1));
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
}

void loop() {
  // put your main code here, to run repeatedly:

  sleep(10);

  na1->setCurrentValue(random(-20, 60));
  vhih.updateAttribute(na1);
}