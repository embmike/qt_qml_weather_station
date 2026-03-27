#pragma once

#include <QObject>

class QWindow;
class WeatherStation;

class AppSettings : public QObject
{
    Q_OBJECT

public:
    explicit AppSettings(QObject *parent = nullptr);

    Q_INVOKABLE void load(WeatherStation *weatherStation, QWindow *window) const;
    Q_INVOKABLE void save(WeatherStation *weatherStation, QWindow *window) const;
};
