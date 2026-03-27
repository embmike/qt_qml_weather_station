# Wetterstation:


## 1. Bereits umgesetzte Funktionalität (nur zur Prüfung)

### 1.1.  Fenster: Wetterstation (main.qml)

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

  
### 1.2. Fenster: Einstellungen (settings.qml)

Einstellungen bietet folgende Parameter zum einstellen an:

- Ort: Standardwert Berlin
- Breitengrad (nur lesbar): Standardwert 52.52008; 
- Längengrad (nur lesbar): Standardwert 13.404954
- Datenlieferant (nur lesbar): Standardwert Open-Meteo
- Sekunden anzeigen: Standardwert ja, Haken gesetzt

Hat folgende Buttons und Funktionen:

- Button oben links: Geklickt. Geänderte Daten werden in das Objekt weatherStation geschrieben , dann wird das Fenster geschlossen. Die Wetterstation aktualisiert ihre Anzeigedaten.

- Button oben rechts: Geklickt. Das Fenster wird beendet, ohne das die Daten in das Objekt weatherStation geschrieben werden.

  

### 1.3. Open-Meteo und Geocoding für die Ermittlung der Wetterdaten (weather_station.h/.cpp)

Aublauf:

1. Benutzer gibt **Ort** ein, z. B. `Port d'Antdrax`
2. deine App ruft zuerst eine **Geocoding-API** auf
3. die API liefert **Latitude/Longitude**
4. danach rufst du wie bisher die Wetterdaten mit diesen Koordinaten ab.
   Open-Meteo hat dafür eine eigene Geocoding-API unter `https://geocoding-api.open-meteo.com/v1/search`, die einen Suchbegriff annimmt und passende Orte mit Koordinaten zurückgibt. Deine Wetterabfrage läuft ja schon über Open-Meteo und verwendet `QNetworkAccessManager`, `QNetworkRequest`, `QUrl` und `QUrlQuery`, was für den zusätzlichen Geocoding-Schritt genau passend ist.



## 2. Fehler

- Es gibt nichts zu tun: keine Fehler vorhanden. 😀



## 3. Neue Aufgaben - neue Funktionalität zum Umsetzen

- 🌤 kleines Wetter-Icon basierend auf API. 👨‍💻

  

## 4. Prüfungen

- Prüfe, ob alles wie vorgehend beschrieben auch funktioniert. 👨‍💻