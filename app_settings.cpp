#include "app_settings.h"

#include <QSettings>
#include <QVariant>
#include <QWindow>

#include "weather_station.h"

namespace
{
constexpr auto locationKey = "weather/locationName";
constexpr auto latitudeKey = "weather/latitude";
constexpr auto longitudeKey = "weather/longitude";
constexpr auto showSecondsKey = "weather/showSeconds";
constexpr auto windowXKey = "window/x";
constexpr auto windowYKey = "window/y";
}

AppSettings::AppSettings(QObject *parent)
    : QObject{parent}
{
}

void AppSettings::load(WeatherStation *weatherStation, QWindow *window) const
{
    QSettings settings{};

    if (weatherStation)
    {
        const QString defaultLocation = weatherStation->locationName();
        const QString locationName = settings.value(QString::fromLatin1(locationKey), defaultLocation).toString();
        const double latitude = settings.value(QString::fromLatin1(latitudeKey), weatherStation->latitude()).toDouble();
        const double longitude =
            settings.value(QString::fromLatin1(longitudeKey), weatherStation->longitude()).toDouble();
        const bool showSeconds = settings.value(QString::fromLatin1(showSecondsKey),
                                                weatherStation->showSeconds())
                                     .toBool();
        weatherStation->setLocationName(locationName);
        weatherStation->setLatitude(latitude);
        weatherStation->setLongitude(longitude);
        weatherStation->setShowSeconds(showSeconds);
    }

    if (window)
    {
        const QVariant storedX = settings.value(QString::fromLatin1(windowXKey));
        const QVariant storedY = settings.value(QString::fromLatin1(windowYKey));

        if (storedX.isValid() && storedY.isValid())
        {
            window->setPosition(storedX.toInt(), storedY.toInt());
        }
    }
}

void AppSettings::save(WeatherStation *weatherStation, QWindow *window) const
{
    QSettings settings{};

    if (weatherStation)
    {
        settings.setValue(QString::fromLatin1(locationKey), weatherStation->locationName());
        settings.setValue(QString::fromLatin1(latitudeKey), weatherStation->latitude());
        settings.setValue(QString::fromLatin1(longitudeKey), weatherStation->longitude());
        settings.setValue(QString::fromLatin1(showSecondsKey), weatherStation->showSeconds());
    }

    if (window)
    {
        settings.setValue(QString::fromLatin1(windowXKey), window->x());
        settings.setValue(QString::fromLatin1(windowYKey), window->y());
    }

    settings.sync();
}
