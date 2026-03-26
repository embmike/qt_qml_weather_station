#pragma once

#include <QObject>

class WeatherData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY dataChanged)
    Q_PROPERTY(double pressure READ pressure NOTIFY dataChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)

public:
    explicit WeatherData(QObject *parent = nullptr);

    double temperature() const noexcept { return m_temperature; }
    int humidity() const noexcept { return m_humidity; }
    double pressure() const noexcept { return m_pressure; }

    QString locationName() const { return m_locationName; }
    void setLocationName(const QString &name);

    Q_INVOKABLE void fetch(double latitude, double longitude);

signals:
    void dataChanged();
    void locationNameChanged();
    void errorOccurred(const QString &message);

private:
    double m_temperature {0.0};
    int m_humidity {0};
    double m_pressure {0.0};
    QString m_locationName;
};
