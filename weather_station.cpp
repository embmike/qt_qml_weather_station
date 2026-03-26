#include "weather_station.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

WeatherStation::WeatherStation(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
}

void WeatherStation::setLocationName(const QString &name)
{
    if (m_locationName == name) {
        return;
    }

    m_locationName = name;
    emit settingsChanged();
}

void WeatherStation::setLatitude(double value)
{
    if (qFuzzyCompare(m_latitude, value)) {
        return;
    }

    m_latitude = value;
    emit settingsChanged();
}

void WeatherStation::setLongitude(double value)
{
    if (qFuzzyCompare(m_longitude, value)) {
        return;
    }

    m_longitude = value;
    emit settingsChanged();
}

void WeatherStation::setShowSeconds(bool value)
{
    if (m_showSeconds == value) {
        return;
    }

    m_showSeconds = value;
    emit settingsChanged();
}

void WeatherStation::fetch()
{
    QUrl url(QStringLiteral("https://api.open-meteo.com/v1/forecast"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("latitude"), QString::number(m_latitude, 'f', 6));
    query.addQueryItem(QStringLiteral("longitude"), QString::number(m_longitude, 'f', 6));
    query.addQueryItem(QStringLiteral("current"), QStringLiteral("temperature_2m,relative_humidity_2m,surface_pressure"));
    query.addQueryItem(QStringLiteral("timezone"), QStringLiteral("auto"));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("QtWeatherStation/1.0"));

    QNetworkReply *reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto cleanup = [reply]() {
            reply->deleteLater();
        };

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred(QStringLiteral("HTTP-Fehler: ") + reply->errorString());
            cleanup();
            return;
        }

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject()) {
            emit errorOccurred(QStringLiteral("Ungültige JSON-Antwort"));
            cleanup();
            return;
        }

        const QJsonObject current = doc.object().value(QStringLiteral("current")).toObject();
        if (current.isEmpty()) {
            emit errorOccurred(QStringLiteral("Feld 'current' fehlt in der Antwort"));
            cleanup();
            return;
        }

        m_temperature = current.value(QStringLiteral("temperature_2m")).toDouble();
        m_humidity = current.value(QStringLiteral("relative_humidity_2m")).toInt();
        m_pressure = current.value(QStringLiteral("surface_pressure")).toDouble();

        emit dataChanged();
        cleanup();
    });
}
