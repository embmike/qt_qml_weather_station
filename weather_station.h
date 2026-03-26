#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include <QObject>

/**
 * @brief Wetterstation
 *
 * @details Die Klasse enthält die Sensoren und stellt die Messwerte für die Anzege bereit.
 * - Hat die 3 Sensoren (Temperatur, Luftdruck und Luftfeuchte) und verwaltet sie in einer Liste.
 * - Jede Minute werden die Messwerte mittels eines Threads aus den Sensoren ausgelesen.
 *   Und in eine Messwertstruktur gepackt und die in einer Ringpuffer abgelegt.
 * - Über drei Schnittstellenfunktionen werden die letzten Messwerte (letzte Messwertstruktur) bereitgestellt.
 */
class WeatherStation : public QObject
{
    Q_OBJECT
public:
    explicit WeatherStation(QObject *parent = nullptr);

    double get_Temperature();
    double get_AirPressure();
    double get_Humidity();

signals:
};

#endif // WEATHER_STATION_H
