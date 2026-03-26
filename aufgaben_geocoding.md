# Aufgabe - Geocoding-AP

Die Idee ist:

1. Benutzer gibt **Ort** ein, z. B. `Port d'Antdrax`
2. deine App ruft zuerst eine **Geocoding-API** auf
3. die API liefert **Latitude/Longitude**
4. danach rufst du wie bisher die Wetterdaten mit diesen Koordinaten ab.
    Open-Meteo hat dafür eine eigene Geocoding-API unter `https://geocoding-api.open-meteo.com/v1/search`, die einen Suchbegriff annimmt und passende Orte mit Koordinaten zurückgibt. Deine Wetterabfrage läuft ja schon über Open-Meteo und verwendet `QNetworkAccessManager`, `QNetworkRequest`, `QUrl` und `QUrlQuery`, was für den zusätzlichen Geocoding-Schritt genau passend ist.



## Beispiel: Implementierung in `weather_station.cpp`

So könnte die neue Methode aussehen:

```C++
void WeatherStation::resolveLocationAndFetch(const QString &location)
{
    const QString trimmed = location.trimmed();
    if (trimmed.isEmpty())
    {
        emit errorOccurred(QStringLiteral("Bitte einen Ort eingeben."));
        return;
    }

    QUrl url{QStringLiteral("https://geocoding-api.open-meteo.com/v1/search")};
    QUrlQuery query{};
    query.addQueryItem(QStringLiteral("name"), trimmed);
    query.addQueryItem(QStringLiteral("count"), QStringLiteral("1"));
    query.addQueryItem(QStringLiteral("language"), QStringLiteral("de"));
    query.addQueryItem(QStringLiteral("format"), QStringLiteral("json"));
    url.setQuery(query);

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("QtWeatherStation/1.0"));

    QNetworkReply *reply{_networkManager->get(request)};

    connect(reply, &QNetworkReply::finished, this, [this, reply, trimmed]() {
        const auto cleanup = [reply]() {
            reply->deleteLater();
        };

        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(QStringLiteral("Geocoding-Fehler: ") + reply->errorString());
            cleanup();
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject())
        {
            emit errorOccurred(QStringLiteral("Ungültige Geocoding-Antwort"));
            cleanup();
            return;
        }

        const QJsonArray results = doc.object().value(QStringLiteral("results")).toArray();
        if (results.isEmpty())
        {
            emit errorOccurred(QStringLiteral("Ort nicht gefunden: ") + trimmed);
            cleanup();
            return;
        }

        const QJsonObject first = results.at(0).toObject();

        _locationName = first.value(QStringLiteral("name")).toString(trimmed);
        _latitude = first.value(QStringLiteral("latitude")).toDouble();
        _longitude = first.value(QStringLiteral("longitude")).toDouble();

        emit settingsChanged();

        cleanup();

        fetch();
    });
}
```



## Änderung in `settings.qml`

Im Save-Handler würdest du dann unterscheiden:

### Fall A: Benutzer hat einen Ort eingegeben

Dann Ort in Koordinaten auflösen.

### Fall B: Benutzer hat nur Koordinaten geändert

Dann direkt setzen und `fetch()` aufrufen.

Zum Beispiel:

```qml
function applyAndClose() {
    if (!settingsWindow.weatherStation) {
        settingsWindow.visible = false
        return
    }

    var locationText = locationField.text.trim()
    var latitudeText = latitudeField.text.trim()
    var longitudeText = longitudeField.text.trim()

    settingsWindow.weatherStation.showSeconds = showSecondsCheck.checked

    if (locationText.length > 0 &&
        latitudeText.length === 0 &&
        longitudeText.length === 0) {
        settingsWindow.weatherStation.resolveLocationAndFetch(locationText)
        settingsWindow.visible = false
        return
    }

    var lat = latitudeText.length > 0 ? Number(latitudeText) : settingsWindow.defaultLat
    var lon = longitudeText.length > 0 ? Number(longitudeText) : settingsWindow.defaultLon

    lat = (!isNaN(lat) && lat !== 0) ? lat : settingsWindow.defaultLat
    lon = (!isNaN(lon) && lon !== 0) ? lon : settingsWindow.defaultLon

    settingsWindow.weatherStation.locationName =
            locationText.length > 0 ? locationText : settingsWindow.defaultLocation
    settingsWindow.weatherStation.latitude = lat
    settingsWindow.weatherStation.longitude = lon
    settingsWindow.weatherStation.fetch()
    settingsWindow.visible = false
}
```

