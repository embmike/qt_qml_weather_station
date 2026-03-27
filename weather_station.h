/**
 * @file weather_station.h
 * @brief Backend-Klasse für Wetter- und Standortdaten der QML-Wetterstation.
 */

#pragma once

#include <QObject>

class QNetworkAccessManager;

/**
 * @brief Stellt Wetterdaten, Standortdaten und Geocoding-Funktionen für QML bereit.
 *
 * @details
 * Die Klasse kapselt:
 * - aktuelle Wetterwerte aus Open-Meteo,
 * - benutzerbezogene Einstellungen (Ort, Koordinaten, Sekundenanzeige),
 * - einen vorgeschalteten Geocoding-Schritt zur Ermittlung von Koordinaten aus einem Ortsnamen.
 */
class WeatherStation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double temperature READ temperature NOTIFY dataChanged)
    Q_PROPERTY(int humidity READ humidity NOTIFY dataChanged)
    Q_PROPERTY(double pressure READ pressure NOTIFY dataChanged)
    Q_PROPERTY(int weatherCode READ weatherCode NOTIFY dataChanged)

    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY settingsChanged)
    Q_PROPERTY(double latitude READ latitude WRITE setLatitude NOTIFY settingsChanged)
    Q_PROPERTY(double longitude READ longitude WRITE setLongitude NOTIFY settingsChanged)
    Q_PROPERTY(QString dataProvider READ dataProvider CONSTANT)
    Q_PROPERTY(bool showSeconds READ showSeconds WRITE setShowSeconds NOTIFY settingsChanged)

    Q_PROPERTY(bool locationLookupValid READ locationLookupValid NOTIFY locationLookupChanged)
    Q_PROPERTY(bool locationLookupPending READ locationLookupPending NOTIFY locationLookupChanged)
    Q_PROPERTY(QString lookupLocationName READ lookupLocationName NOTIFY locationLookupChanged)
    Q_PROPERTY(double lookupLatitude READ lookupLatitude NOTIFY locationLookupChanged)
    Q_PROPERTY(double lookupLongitude READ lookupLongitude NOTIFY locationLookupChanged)

public:
    /**
     * @brief Erstellt eine neue WeatherStation-Instanz.
     * @param parent Optionales Elternobjekt im Qt-Objektbaum.
     */
    explicit WeatherStation(QObject *parent = nullptr);

    /**
     * @brief Liefert die aktuelle Temperatur in Grad Celsius.
     * @return Temperaturwert.
     */
    double temperature() const noexcept { return _temperature; }

    /**
     * @brief Liefert die aktuelle relative Luftfeuchtigkeit in Prozent.
     * @return Luftfeuchtigkeit.
     */
    int humidity() const noexcept { return _humidity; }

    /**
     * @brief Liefert den aktuellen Luftdruck in hPa.
     * @return Luftdruckwert.
     */
    double pressure() const noexcept { return _pressure; }

    /**
     * @brief Liefert den aktuellen Open-Meteo-Wettercode.
     * @return Wettercode.
     */
    int weatherCode() const noexcept { return _weatherCode; }

    /**
     * @brief Liefert den aktuell gesetzten Ortsnamen.
     * @return Ortsname.
     */
    QString locationName() const { return _locationName; }

    /**
     * @brief Setzt den Ortsnamen.
     * @param name Neuer Ortsname.
     */
    void setLocationName(const QString &name);

    /**
     * @brief Liefert den aktuell verwendeten Breitengrad.
     * @return Breitengrad.
     */
    double latitude() const noexcept { return _latitude; }

    /**
     * @brief Setzt den Breitengrad.
     * @param value Neuer Breitengrad.
     */
    void setLatitude(double value);

    /**
     * @brief Liefert den aktuell verwendeten Längengrad.
     * @return Längengrad.
     */
    double longitude() const noexcept { return _longitude; }

    /**
     * @brief Setzt den Längengrad.
     * @param value Neuer Längengrad.
     */
    void setLongitude(double value);

    /**
     * @brief Liefert den Namen des Wetterdaten-Anbieters.
     * @return Datenanbieter (konstant: Open-Meteo).
     */
    QString dataProvider() const { return QStringLiteral("Open-Meteo"); }

    /**
     * @brief Gibt an, ob die Uhrzeit inklusive Sekunden dargestellt wird.
     * @return true, wenn Sekunden angezeigt werden.
     */
    bool showSeconds() const noexcept { return _showSeconds; }

    /**
     * @brief Aktiviert oder deaktiviert die Sekundenanzeige.
     * @param value Neuer Zustand.
     */
    void setShowSeconds(bool value);

    /**
     * @brief Kennzeichnet, ob das letzte Geocoding-Ergebnis gültig ist.
     * @return true bei gültigem Ergebnis.
     */
    bool locationLookupValid() const noexcept { return _locationLookupValid; }

    /**
     * @brief Kennzeichnet, ob aktuell eine Geocoding-Anfrage läuft.
     * @return true, solange die Anfrage aussteht.
     */
    bool locationLookupPending() const noexcept { return _locationLookupPending; }

    /**
     * @brief Liefert den durch Geocoding ermittelten Ortsnamen.
     * @return Ortsname aus Lookup.
     */
    QString lookupLocationName() const { return _lookupLocationName; }

    /**
     * @brief Liefert den durch Geocoding ermittelten Breitengrad.
     * @return Breitengrad aus Lookup.
     */
    double lookupLatitude() const noexcept { return _lookupLatitude; }

    /**
     * @brief Liefert den durch Geocoding ermittelten Längengrad.
     * @return Längengrad aus Lookup.
     */
    double lookupLongitude() const noexcept { return _lookupLongitude; }

    /**
     * @brief Lädt aktuelle Wetterdaten für die gespeicherten Koordinaten.
     *
     * @details
     * Die Methode ruft die Open-Meteo-API asynchron ab und aktualisiert bei Erfolg
     * Temperatur, Luftfeuchtigkeit, Luftdruck und Wettercode.
     */
    Q_INVOKABLE void fetch();

    /**
     * @brief Löst einen Ortsnamen in Koordinaten auf.
     * @param location Vom Benutzer eingegebener Ortsname.
     *
     * @details
     * Die Methode verwendet die Open-Meteo-Geocoding-API asynchron und schreibt
     * das Ergebnis in die Lookup-Properties.
     */
    Q_INVOKABLE void resolveLocation(const QString &location);

signals:
    /**
     * @brief Signalisiert geänderte Wetterdaten.
     */
    void dataChanged();

    /**
     * @brief Signalisiert geänderte Einstellungen.
     */
    void settingsChanged();

    /**
     * @brief Meldet einen Fehlertext für UI-Ausgaben.
     * @param message Fehlermeldung.
     */
    void errorOccurred(const QString &message);

    /**
     * @brief Signalisiert geänderten Zustand des Standort-Lookups.
     */
    void locationLookupChanged();

private:
    /**
     * @brief Setzt den internen Zustand des Standort-Lookups.
     * @param valid Gültigkeit des Ergebnisses.
     * @param pending Laufender Anfragezustand.
     * @param name Aufgelöster Ortsname.
     * @param latitude Aufgelöster Breitengrad.
     * @param longitude Aufgelöster Längengrad.
     */

    void setLookupState(bool valid,
                        bool pending,
                        const QString &name,
                        double latitude,
                        double longitude);

    /** @brief Zuletzt geladene Temperatur in Grad Celsius. */
    double _temperature {0.0};

    /** @brief Zuletzt geladene relative Luftfeuchtigkeit in Prozent. */
    int _humidity {0};

    /** @brief Zuletzt geladener Luftdruck in hPa. */
    double _pressure {0.0};

    /** @brief Zuletzt geladener Open-Meteo-Wettercode. */
    int _weatherCode {0};

    /** @brief Persistierter Ortsname. */
    QString _locationName {QStringLiteral("Berlin")};

    /** @brief Persistierter Breitengrad des Orts. */
    double _latitude {52.520007};

    /** @brief Persistierter Längengrad des Orts. */
    double _longitude {13.404954};

    /** @brief Einstellung zur Anzeige von Sekunden. */
    bool _showSeconds {true};

    /** @brief Kennzeichen für gültiges Geocoding-Ergebnis. */
    bool _locationLookupValid {true};

    /** @brief Kennzeichen für laufende Geocoding-Anfrage. */
    bool _locationLookupPending {false};

    /** @brief Durch Geocoding aufgelöster Ortsname. */
    QString _lookupLocationName {QStringLiteral("Berlin")};

    /** @brief Durch Geocoding aufgelöster Breitengrad. */
    double _lookupLatitude {52.520007};

    /** @brief Durch Geocoding aufgelöster Längengrad. */
    double _lookupLongitude {13.404954};

    /** @brief Netzwerkmanager für asynchrone API-Anfragen. */
    QNetworkAccessManager *_networkManager {nullptr};
};
