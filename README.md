# homee-api
Dieses Projekt ist eine Möglichkeit, das Smarthome-System homee mit einem ESP32 Controller zu verbinden.
Hierdurch ist es möglich, Sensoren und Aktoren auf Basis eines ESP32 und ESP8266 zu erstellen.

Ein Beispiel ist hier verfügbar: https://github.com/DanielKnoop/homee-co2-sensor

Die API gliedert sich im Kern in drei Teile.
Das API Objekt selbst, Nodes und Attributes.

Zunächst in es notwendig, die homee-Api zu erstellen (den virtualHomee).

```cpp
#include "virtualHomee.hpp"
virtualHomee vhih;
```

Diesem virtualHomee müssen Nodes hinzugefügt werden (Vorzugsweise im Bereich Setup).
Es dürfen maximal 99 Nodes hinzugefügt werden.
```cpp
node* n1 = new node(10, 3001, "Luftsensor");
vhih.addNode(n1);
```
Der Konstruktor vom Node nimmt dabei folgende Attribute entgegen: NodeId, Profile, Name

Diesem Node müssen noch Attribute zugeordnet werden. Ein Node darf maximal 99 Attribute bekommen.

```cpp
    na1 = n1->AddAttributes(new nodeAttributes(5));
    na1->setUnit("°C");
    na1->setMinimumValue(-20);
    na1->setMaximumValue(60);
```

Im loop bekommen die Attribute dann ihre Werte. Es empfiehlt sich daher, die Nodes im globalen Bereich zu speichern.

```cpp
nodeAttributes* na1;
```

```cpp
unsigned long previousMillis = 0;
const long interval = 60000; 

void loop()
{
  //Wir dürfen kein delay oder so verwenden, da hierdurch die Kommunikation mit dem homee gestört werden kann.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    na1->setCurrentValue(random(-20, 60)); //schreibt einen zufälligen Wert zwischen -20 und +60
    vhih.updateAttribute(na1);
  }
}
```

Es ist auch möglich, Werte vom homee zu empfangen. Hierzu kann dem Attribute eine Callback-Funktion mitgegeben werden, welche gerufen wird, wenn der homee einen neuen Wert an das Attribut sendet.

```cpp
void setup()
{
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
}
```

Wenn der vhih fertig eingerichtet ist muss er noch mit 
```cpp
vhih.start();
```
gestartet werden.

Um den Sensor zum homee hinzuzufügen sucht man in der homee-app unter Geräte hinzufügen nach "homee in homee" und wählt dort "mit homee verbinden" aus. Hier gibt man die IP des ESP ein sowie irgendein Benutzernamen und Kennwort. Die Zugangsdaten werden vom vhih nicht geprüft, daher sind diese nicht relevant.

Das Projekt ist angelehnt an die node-red Implementierung [node-red-contrib-homee](https://github.com/stfnhmplr/node-red-contrib-homee)
von stfnhmplr. Vielen Dank, auch an alle anderen Vorreiter aus dem [homee-Forum](https://community.hom.ee).

Ich stehe in keiner Verbindung zur homee GmbH oder zu codeatelier GmbH
