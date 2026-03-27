#include "app_settings.h"

#include <QSettings>
#include <QVariant>
#include <QWindow>

#include "weather_station.h"

namespace
{
constexpr auto locationKey = "weather/locationName";
constexpr auto windowXKey = "window/x";
constexpr auto windowYKey = "window/y";
}

AppSettings::AppSettings(QObject *parent)
    : QObject(parent)
{
}

void AppSettings::load(WeatherStation *weatherStation, QWindow *window) const
{
    QSettings settings;

    if (weatherStation)
    {
        const QString defaultLocation = weatherStation->locationName();
        const QString locationName = settings.value(QString::fromLatin1(locationKey), defaultLocation).toString();
        weatherStation->setLocationName(locationName);
    }

    if (window)
    {
        const QVariant storedX = settings.value(QString::fromLatin1(windowXKey));
        const QVariant storedY = settings.value(QString::fromLatin1(windowYKey));

        if (storedX.isValid() && storedY.isValid())
        {
            window->setX(storedX.toDouble());
            window->setY(storedY.toDouble());
        }
    }
}

void AppSettings::save(const WeatherStation *weatherStation, const QWindow *window) const
{
    QSettings settings;

    if (weatherStation)
    {
        settings.setValue(QString::fromLatin1(locationKey), weatherStation->locationName());
    }

    if (window)
    {
        settings.setValue(QString::fromLatin1(windowXKey), window->x());
        settings.setValue(QString::fromLatin1(windowYKey), window->y());
    }

    settings.sync();
}
