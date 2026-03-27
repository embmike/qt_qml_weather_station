# Funktionalität:



## 1. Fenster: Wetterstation (main.qml)

Wetterstation zeigt folgendes an:

- Überschrift: Wetterstation
- Ort
- Datum
- Uhrzeit: a) mit Sekunden, b) ohne Sekunden
- Temperatur, Luftfeuchte und Luftdruck

Hat folgende Buttons:

- Button oben links: Einstellungen

- Button oben rechts: Applikation beenden

Hat folgende Funktionen:


- Applikation starten: Wird das Fenster angezeigt, Fenster-Position, Ort und Sekunden anzeigen werden geladen und die Anzeige entsprechend aktualisiert (Ort, Uhrzeit, Temperatur, Luftfeuchte und Luftdruck) 

- Applikation beenden:  Geklickt. Fenster-Position, Ort und Sekunden anzeigen werden gesichert und dann die Applikation beendet.

- Einstellungen: Geklickt. Das Fenster Einstellungen (settings.xml) öffnet sich.

  
## 2. Fenster: Einstellungen (settings.qml)

Einstellungen bietet folgende Parameter zum einstellen an:

- Ort: Standardwert Berlin
- Breitengrad (nur lesbar): Standardwert 52.52008; 
- Längengrad (nur lesbar): Standardwert 13.404954
- Datenlieferant (nur lesbar): Standardwert Open-Meteo
- Sekunden anzeigen: Standardwert ja, Haken gesetzt

Hat folgende Buttons und Funktionen:

- Button oben links: Geklickt. Geänderte Daten werden in das Objekt weatherStation geschrieben , dann wird das Fenster geschlossen. Die Wetterstation aktualisiert ihre Anzeigedaten.
- Button oben rechts: Geklickt. Das Fenster wird beendet, ohne das die Daten in das Objekt weatherStation geschrieben werden.



## 3. Fehler

- Einstellungen (settings.xml), Button oben links : Geklickt, Die Daten werden übernommen aber das Fenster wird nicht geschlossen
- Wetterstation (main.qml), Button oben rechts: Geklickt, das Fenster wird geschlossen und damit die Applikation beendet, aber anscheinend werden die Daten wie Fenster-Position, Ort und Sekunden anzeigen nicht gesichert. Denn wird die Applikation erneut gestartet, ist das Fenster an der Standardposition (Default), sowie auch die anderen Anzeigedaten (Ort und Sekunden anzeigen) stehen auf ihre Standardwerte (Default).



## 4. Aufgaben

- Prüfe, ob alles wie vorgehend beschrieben auch funktioniert.