#include "weather_station.h"

WeatherStation::WeatherStation(QObject *parent)
    : QObject{parent}
{}

double WeatherStation::get_Temperature()
{
    return .0;
}

double WeatherStation::get_AirPressure()
{
    return .0;
}

double WeatherStation::get_Humidity()
{
    return .0;
}
