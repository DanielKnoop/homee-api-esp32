# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden in dieser Datei dokumentiert.
Das Format orientiert sich an [Keep a Changelog](https://keepachangelog.com/de/1.0.0/),
die Versionierung an [SemVer](https://semver.org/lang/de/).

## [Unreleased]

### Fixed
- Beschädigte `GET:nodes`-Antwort bei großen Attributwerten. `virtualHomee.hpp` hob
  `ARDUINOJSON_POSITIVE_EXPONENTIATION_THRESHOLD` auf `1e9` an; mit ArduinoJson 7.4.3
  führt das bei Werten mit acht oder mehr Vorkommastellen zu einem Pufferüberlauf in
  `writeDecimals()`. Auf einem ESP32-D0WDQ6 erzeugte `16777215` — das `maximum` des
  Color-Attributs aus dem Beispiel — 149 statt 14 Bytes, gefüllt mit rohem Stack-Inhalt.
  Der Schwellwert steht jetzt wieder auf dem Default, und `nodeAttributes::GetJSONObject()`
  serialisiert ganzzahlige Werte als Integer. Damit bleibt `16777215` exakt und in
  Klartext-Notation, statt als `1.677722e7` (= 16777220) gerundet zu werden.

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
