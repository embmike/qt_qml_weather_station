#include "weather_station.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

WeatherStation::WeatherStation(QObject *parent)
    : QObject{parent}
    , _networkManager{new QNetworkAccessManager(this)}
{
}

void WeatherStation::setLookupState(bool valid,
                                    bool pending,
                                    const QString &name,
                                    double latitude,
                                    double longitude)
{
    const bool changed = (_locationLookupValid != valid)
                         || (_locationLookupPending != pending)
                         || (_lookupLocationName != name)
                         || !qFuzzyCompare(_lookupLatitude, latitude)
                         || !qFuzzyCompare(_lookupLongitude, longitude);

    _locationLookupValid = valid;
    _locationLookupPending = pending;
    _lookupLocationName = name;
    _lookupLatitude = latitude;
    _lookupLongitude = longitude;

    if (changed)
    {
        emit locationLookupChanged();
    }
}

void WeatherStation::setLocationName(const QString &name)
{
    if (_locationName == name)
    {
        return;
    }

    _locationName = name;
    emit settingsChanged();
}

void WeatherStation::setLatitude(double value)
{
    if (qFuzzyCompare(_latitude, value))
    {
        return;
    }

    _latitude = value;
    emit settingsChanged();
}

void WeatherStation::setLongitude(double value)
{
    if (qFuzzyCompare(_longitude, value))
    {
        return;
    }

    _longitude = value;
    emit settingsChanged();
}

void WeatherStation::setShowSeconds(bool value)
{
    if (_showSeconds == value)
    {
        return;
    }

    _showSeconds = value;
    emit settingsChanged();
}

void WeatherStation::fetch()
{
    QUrl url{QStringLiteral("https://api.open-meteo.com/v1/forecast")};
    QUrlQuery query{};

    query.addQueryItem(QStringLiteral("latitude"), QString::number(_latitude, 'f', 6));
    query.addQueryItem(QStringLiteral("longitude"), QString::number(_longitude, 'f', 6));
    query.addQueryItem(QStringLiteral("current"),
                       QStringLiteral("temperature_2m,relative_humidity_2m,surface_pressure"));
    query.addQueryItem(QStringLiteral("timezone"), QStringLiteral("auto"));
    url.setQuery(query);

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("QtWeatherStation/1.0"));

    QNetworkReply *reply{_networkManager->get(request)};

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto cleanup = [reply]() {
            reply->deleteLater();
        };

        if (reply->error() != QNetworkReply::NoError)
        {
            emit errorOccurred(QStringLiteral("HTTP-Fehler: ") + reply->errorString());
            cleanup();
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject())
        {
            emit errorOccurred(QStringLiteral("Ungültige JSON-Antwort"));
            cleanup();
            return;
        }

        const QJsonObject current = doc.object().value(QStringLiteral("current")).toObject();
        if (current.isEmpty())
        {
            emit errorOccurred(QStringLiteral("Feld 'current' fehlt in der Antwort"));
            cleanup();
            return;
        }

        _temperature = current.value(QStringLiteral("temperature_2m")).toDouble();
        _humidity = current.value(QStringLiteral("relative_humidity_2m")).toInt();
        _pressure = current.value(QStringLiteral("surface_pressure")).toDouble();

        emit dataChanged();
        cleanup();
    });
}

void WeatherStation::resolveLocation(const QString &location)
{
    const QString trimmed = location.trimmed();

    if (trimmed.isEmpty())
    {
        setLookupState(true, false, _locationName, _latitude, _longitude);
        return;
    }

    setLookupState(false, true, trimmed, 0.0, 0.0);

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
            setLookupState(false, false, trimmed, 0.0, 0.0);
            emit errorOccurred(QStringLiteral("Geocoding-Fehler: ") + reply->errorString());
            cleanup();
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject())
        {
            setLookupState(false, false, trimmed, 0.0, 0.0);
            emit errorOccurred(QStringLiteral("Ungültige Geocoding-Antwort"));
            cleanup();
            return;
        }

        const QJsonArray results = doc.object().value(QStringLiteral("results")).toArray();
        if (results.isEmpty())
        {
            setLookupState(false, false, trimmed, 0.0, 0.0);
            cleanup();
            return;
        }

        const QJsonObject first = results.at(0).toObject();
        const QString resolvedName = first.value(QStringLiteral("name")).toString(trimmed);
        const double latitude = first.value(QStringLiteral("latitude")).toDouble();
        const double longitude = first.value(QStringLiteral("longitude")).toDouble();

        setLookupState(true, false, resolvedName, latitude, longitude);
        cleanup();
    });
}
