# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden in dieser Datei dokumentiert.
Das Format orientiert sich an [Keep a Changelog](https://keepachangelog.com/de/1.0.0/),
die Versionierung an [SemVer](https://semver.org/lang/de/).

## [0.4.0] — 2026-11-16

### Added
- `src/virtualHomee/homee_defines.h` — Enums für Node-Profile, Attribut-Typen, Protokolle
  und Status-Codes (49 Gruppen, 1064 Konstanten). Struktur und Namenskonvention stammen aus
  dem [homee-api](https://github.com/stfnhmplr/homee-api) JS-SDK (MIT), die Werte sind gegen
  die aktuelle homee-Web-App abgeglichen.
- `src/virtualHomee/homee_icons.h` — `NodeIcon*`-Konstanten für `node::setImage()`.
  Inoffiziell, aus dem WebSocket-Protokoll eines homee-Cubes zusammengetragen.
- `nodeAttributes::setName()` / `getName()`. Der Name wird nur serialisiert, wenn er gesetzt
  ist — ein leeres `"name"` an jedem Attribut würde die `GET:nodes`-Antwort unnötig aufblähen.
- `virtualHomee::setVersion()` / `getVersion()`. Die an den Hub gemeldete homee-Version war
  bisher nur einkompiliert; sie lässt sich jetzt vor `start()` überschreiben.
- Host-Tests für die numerische Serialisierung in `nodeAttributes::GetJSONObject()`,
  aufrufbar mit `pio test -e native`. `node`, `nodeAttributes` und `nodes` hängen von
  Arduino nur an `String`, der in `test/mocks/Arduino.h` nachgebildet wird — die Tests
  laufen damit ohne Hardware in unter zwei Sekunden.

### Changed
- Der Default für die gemeldete homee-Version ist jetzt `2.41.6+6628d59d`
  (vorher `2.41.3+46ad073c`).
- Beispielcode (`src/main.cpp`) und README nutzen die neuen Enum- und Icon-Konstanten
  statt unkommentierter Zahlen-Literale.

### Fixed
- Beschädigte `GET:nodes`-Antwort bei großen Attributwerten. `virtualHomee.hpp` hob
  `ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD` auf `1e9` an; mit ArduinoJson 7.4.3
  führt das bei Werten mit acht oder mehr Vorkommastellen zu einem Pufferüberlauf in
  `writeDecimals()`. Auf einem ESP32-D0WDQ6 erzeugte `16777215` — das `maximum` des
  Color-Attributs aus dem Beispiel — 149 statt 14 Bytes, gefüllt mit rohem Stack-Inhalt.
  Der Schwellwert steht jetzt wieder auf dem Default, und `nodeAttributes::GetJSONObject()`
  serialisiert ganzzahlige Werte als Integer. Damit bleibt `16777215` exakt und in
  Klartext-Notation, statt als `1.677722e7` (= 16777220) gerundet zu werden.
- `virtualHomee::stop()` schließt den UDP-Discovery-Socket wieder. Er blieb bisher nach dem
  Stoppen offen. `stopDiscoveryService()` setzt dabei `firstStart` zurück, damit ein
  folgendes `start()` den Discovery-Dienst erneut aufsetzt.
- `nodes::~nodes()` gibt die Nodes frei. `node::~node()` löste seine Attribute bereits auf und
  `nodes::RemoveNodeById()` löschte den entfernten Node — nur der Destruktor tat nichts.

Die beiden Konstanten-Header gehen auf einen Beitrag von
[Oxi75](https://github.com/Oxi75) zurück (PR #5).

## [0.3.0] — 2026-05-25

### Changed
- Migration auf ArduinoJson v7 und die aktuell gepflegten Async-Libraries
  (ESP32Async für ESP32, mathieucarbou/esphome für ESP8266).
- Deutlich geringere Heap-Fragmentierung: wiederverwendbarer Sendepuffer statt
  Allokation pro Nachricht.
- `GET:nodes` serialisiert in zwei Durchgängen direkt in den WebSocket-Puffer und
  vermeidet damit eine vollständige Kopie der Antwort.
- Dual-Core-Absicherung für ESP32 (Mutex um den Sendepfad, atomarer Client-Zähler).

### Fixed
- Attribut-Updates und Spitzenverbrauch im RAM.
- `virtualHomee.h` als Arduino-Einstiegspunkt wiederhergestellt.

### Added
- Board-Umgebungen für UM TinyS3 und Seeed XIAO ESP32-S3.

## Vor 0.3.0

Für die Versionen 0.2.4 bis 0.2.7 wurde kein Changelog geführt. Die vollständige Historie
ist über `git log` und die zugehörigen Tags einsehbar.
