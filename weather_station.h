#pragma once

#include <QObject>

class QNetworkAccessManager;

class WeatherStation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY dataChanged)
    Q_PROPERTY(double pressure READ pressure NOTIFY dataChanged)

    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY settingsChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY settingsChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY settingsChanged)
    Q_PROPERTY(QString dataProvider READ dataProvider CONSTANT)
    Q_PROPERTY(bool showSeconds READ showSeconds WRITE setShowSeconds NOTIFY settingsChanged)

public:
    explicit WeatherStation(QObject *parent = nullptr);

    double temperature() const noexcept { return m_temperature; }
    int humidity() const noexcept { return m_humidity; }
    double pressure() const noexcept { return m_pressure; }

    QString locationName() const { return m_locationName; }
    void setLocationName(const QString &name);

    double latitude() const noexcept { return m_latitude; }
    void setLatitude(double value);

    double longitude() const noexcept { return m_longitude; }
    void setLongitude(double value);

    QString dataProvider() const { return QStringLiteral("Open-Meteo"); }

    bool showSeconds() const noexcept { return m_showSeconds; }
    void setShowSeconds(bool value);

    Q_INVOKABLE void fetch();

signals:
    void dataChanged();
    void settingsChanged();
    void errorOccurred(const QString &message);

private:
    double m_temperature {0.0};
    int m_humidity {0};
    double m_pressure {0.0};

    QString m_locationName {QStringLiteral("Berlin")};
    double m_latitude {52.52008};
    double m_longitude {13.404954};
    bool m_showSeconds {true};

    QNetworkAccessManager *m_networkManager {nullptr};
};
