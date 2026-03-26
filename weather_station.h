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

    double temperature() const noexcept { return _temperature; }
    int humidity() const noexcept { return _humidity; }
    double pressure() const noexcept { return _pressure; }

    QString locationName() const { return _locationName; }
    void setLocationName(const QString &name);

    double latitude() const noexcept { return _latitude; }
    void setLatitude(double value);

    double longitude() const noexcept { return _longitude; }
    void setLongitude(double value);

    QString dataProvider() const { return QStringLiteral("Open-Meteo"); }

    bool showSeconds() const noexcept { return _showSeconds; }
    void setShowSeconds(bool value);

    Q_INVOKABLE void fetch();

signals:
    void dataChanged();
    void settingsChanged();
    void errorOccurred(const QString &message);

private:
    double _temperature {0.0};
    int _humidity {0};
    double _pressure {0.0};

    QString _locationName {QStringLiteral("Berlin")};
    double _latitude {52.520007};
    double _longitude {13.404954};
    bool _showSeconds {true};

    QNetworkAccessManager *_networkManager {nullptr};
};
