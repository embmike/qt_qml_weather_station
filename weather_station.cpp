#include "weather_station.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

WeatherData::WeatherData(QObject *parent)
    : QObject(parent)
{
}

void WeatherData::setLocationName(const QString &name)
{
    if (m_locationName == name)
    {
        return;
    }

    m_locationName = name;
    emit locationNameChanged();
}

void WeatherData::fetch(double latitude, double longitude)
{
    auto *manager = new QNetworkAccessManager(this);

    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", QString::number(latitude, 'f', 6));
    query.addQueryItem("longitude", QString::number(longitude, 'f', 6));
    query.addQueryItem("current", "temperature_2m,relative_humidity_2m,surface_pressure");
    query.addQueryItem("timezone", "auto");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "QtWeatherStation/1.0");

    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto cleanup = [reply]() {
            reply->deleteLater();
        };

        if (reply->error() != QNetworkReply::NoError) {
            emit errorOccurred("HTTP-Fehler: " + reply->errorString());
            cleanup();
            return;
        }

        const QByteArray payload = reply->readAll();
        const QJsonDocument doc = QJsonDocument::fromJson(payload);

        if (!doc.isObject()) {
            emit errorOccurred("Ungültige JSON-Antwort");
            cleanup();
            return;
        }

        const QJsonObject root = doc.object();
        const QJsonObject current = root.value("current").toObject();

        if (current.isEmpty()) {
            emit errorOccurred("Feld 'current' fehlt in der Antwort");
            cleanup();
            return;
        }

        m_temperature = current.value("temperature_2m").toDouble();
        m_humidity    = current.value("relative_humidity_2m").toInt();
        m_pressure    = current.value("surface_pressure").toDouble();

        emit dataChanged();
        cleanup();
    });
}