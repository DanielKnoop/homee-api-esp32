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
nodeAttributes* schalterAttribute3;
nodeAttributes* schalterAttribute4;
nodeAttributes* schalterAttribute5;
nodeAttributes* schalterAttribute6;
nodeAttributes* schalterAttribute7;
nodeAttributes* schalterAttribute8;
nodeAttributes* schalterAttribute9;
nodeAttributes* schalterAttribute10;
nodeAttributes* schalterAttribute11;
nodeAttributes* schalterAttribute12;

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
  

  node* n2 = new node(20, 10, "Schalter2");
  schalterAttribute = n2->AddAttributes(new nodeAttributes(1, 200));
  schalterAttribute->setEditable(1);
  schalterAttribute->setMinimumValue(0);
  schalterAttribute->setMaximumValue(1);
  schalterAttribute->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n2->AddAttributes(new nodeAttributes(1, 201));
  n2->AddAttributes(new nodeAttributes(1, 202));
  n2->AddAttributes(new nodeAttributes(1, 203));

  node* n3 = new node(30, 10, "Schalter3");
  schalterAttribute3 = n3->AddAttributes(new nodeAttributes(1, 300));
  schalterAttribute3->setEditable(1);
  schalterAttribute3->setMinimumValue(0);
  schalterAttribute3->setMaximumValue(1);
  schalterAttribute3->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n3->AddAttributes(new nodeAttributes(1, 301));
  n3->AddAttributes(new nodeAttributes(1, 302));
  n3->AddAttributes(new nodeAttributes(1, 303));


  node* n4 = new node(40, 10, "Schalter4");
  schalterAttribute4 = n4->AddAttributes(new nodeAttributes(1, 400));
  schalterAttribute4->setEditable(1);
  schalterAttribute4->setMinimumValue(0);
  schalterAttribute4->setMaximumValue(1);
  schalterAttribute4->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n4->AddAttributes(new nodeAttributes(1, 401));
  n4->AddAttributes(new nodeAttributes(1, 402));
  n4->AddAttributes(new nodeAttributes(1, 403));


   node* n5 = new node(50, 10, "Schalter5");
  schalterAttribute5 = n5->AddAttributes(new nodeAttributes(1, 500));
  schalterAttribute5->setEditable(1);
  schalterAttribute5->setMinimumValue(0);
  schalterAttribute5->setMaximumValue(1);
  schalterAttribute5->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n5->AddAttributes(new nodeAttributes(1, 501));
  n5->AddAttributes(new nodeAttributes(1, 502));
  n5->AddAttributes(new nodeAttributes(1, 503));


   node* n6 = new node(60, 10, "Schalter6");
  schalterAttribute6 = n6->AddAttributes(new nodeAttributes(1, 600));
  schalterAttribute6->setEditable(1);
  schalterAttribute6->setMinimumValue(0);
  schalterAttribute6->setMaximumValue(1);
  schalterAttribute6->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n6->AddAttributes(new nodeAttributes(1, 601));
  n6->AddAttributes(new nodeAttributes(1, 602));
  n6->AddAttributes(new nodeAttributes(1, 603));


   node* n7 = new node(70, 10, "Schalter7");
  schalterAttribute7 = n7->AddAttributes(new nodeAttributes(1, 700));
  schalterAttribute7->setEditable(1);
  schalterAttribute7->setMinimumValue(0);
  schalterAttribute7->setMaximumValue(1);
  schalterAttribute7->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });
  n7->AddAttributes(new nodeAttributes(1, 701));
  n7->AddAttributes(new nodeAttributes(1, 702));
  n7->AddAttributes(new nodeAttributes(1, 703));


   node* n8 = new node(80, 10, "Schalter8");
  schalterAttribute8 = n8->AddAttributes(new nodeAttributes(1, 800));
  schalterAttribute8->setEditable(1);
  schalterAttribute8->setMinimumValue(0);
  schalterAttribute8->setMaximumValue(1);
  schalterAttribute8->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });      
  n8->AddAttributes(new nodeAttributes(1, 801));
  n8->AddAttributes(new nodeAttributes(1, 802));
  n8->AddAttributes(new nodeAttributes(1, 803));

   node* n9 = new node(90, 10, "Schalter9");
  schalterAttribute9 = n9->AddAttributes(new nodeAttributes(1, 900));
  schalterAttribute9->setEditable(1);
  schalterAttribute9->setMinimumValue(0);
  schalterAttribute9->setMaximumValue(1);
  schalterAttribute9->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });      
  n9->AddAttributes(new nodeAttributes(1, 901));
  n9->AddAttributes(new nodeAttributes(1, 902));
  n9->AddAttributes(new nodeAttributes(1, 903));

  node* n10 = new node(100, 10, "Schalter10");
  schalterAttribute10 = n10->AddAttributes(new nodeAttributes(1, 1000));
  schalterAttribute10->setEditable(1);
  schalterAttribute10->setMinimumValue(0);
  schalterAttribute10->setMaximumValue(1);
  schalterAttribute10->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });       
  n10->AddAttributes(new nodeAttributes(1, 1001));
  n10->AddAttributes(new nodeAttributes(1, 1002));
  n10->AddAttributes(new nodeAttributes(1, 1003));

  node* n11 = new node(110, 10, "Schalter11");
  schalterAttribute11 = n11->AddAttributes(new nodeAttributes(1, 1100));
  schalterAttribute11->setEditable(1);
  schalterAttribute11->setMinimumValue(0);
  schalterAttribute11->setMaximumValue(1);
  schalterAttribute11->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });       
  n11->AddAttributes(new nodeAttributes(1, 1101));
  n11->AddAttributes(new nodeAttributes(1, 1102));
  n11->AddAttributes(new nodeAttributes(1, 1103));

  node* n12 = new node(120, 10, "Schalter12");
  schalterAttribute12 = n12->AddAttributes(new nodeAttributes(1, 1200));
  schalterAttribute12->setEditable(1);
  schalterAttribute12->setMinimumValue(0);
  schalterAttribute12->setMaximumValue(1);
  schalterAttribute12->setCallback([](nodeAttributes* a)
  {
    a->setCurrentValue(a->getTargetValue());
    vhih.updateAttribute(a);
  });       
  n12->AddAttributes(new nodeAttributes(1, 1201));
  n12->AddAttributes(new nodeAttributes(1, 1202));
  n12->AddAttributes(new nodeAttributes(1, 1203));

  vhih.addNode(n1);
  vhih.addNode(n2);
  vhih.addNode(n3);
  vhih.addNode(n4);
  vhih.addNode(n5);
  vhih.addNode(n6);
  vhih.addNode(n7);
  vhih.addNode(n8);
  //vhih.addNode(n9);
  //vhih.addNode(n10);
  //vhih.addNode(n11);
  //vhih.addNode(n12);
  //vhih.addNode(n12);
  //vhih.addNode(n12);

  vhih.start();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

u_int64_t lastMillis = 0;
u_int64_t duration = 30000;

void loop() {
  u_int64_t currentMillis = millis();
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
  }
}
#endif