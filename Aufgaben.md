# Funktionalität:

## 1. Fenster: Wetterstation (main.qml)

Wetterstation zeigt folgendes an:

- Überschrift: Wetterstation
- Ort
- Datum
- Uhrzeit: a) mit Sekunden, b) ohne Sekunden
- Temperatur, Luftfeuchte und Luftdruck

Hat folgende Buttons:

- oben rechts: Einstellungen

- oben links: Applikation beenden

  
## 2. Fenster: Einstellungen (settings.qml)

Einstellungen bietet folgende Parameter zum einstellen an:

- Ort: Vorbelegung Berlin
- Breitengrad: Vorbelegung 52.52008
- Längengrad: Vorbelegung 13.404954
- Datenlieferant: Vorbelegung Open-Meteo; aktuell nicht veränderbar
- Urzeit mit Sekunden anzeigen: Vorbelegung ja, Haken gesetzt

Hat folgende Buttons:

- oben rechts: Daten wie, Ort, Breitengrad, Längengrad und "Uhrzeit mit Sekunden anzeigen" werden in das Objekt weatherStation geschrieben werden, dann wird das Fenster geschlossen. Die Wetterstation aktualisiert ihre Anzeigedaten.
- oben links: Das Fenster wird beendet, ohne das die Daten in in das Objekt weatherStation geschrieben werden.



## 3. Aufgaben

- Im Fenster: Einstellungen (settings.qml) soll es folgende Defaultwerte - siehe unten - geben. Hat der Benutzer einen Wert vergessen, werden die  Defaultwerte verwendet:

property real defaultLat: 52.520008
property real defaultLon: 13.404954
property string defaultLocation: "Berlin"

- Prüfe, ob alles wie vorgehend beschrieben auch funktioniert.