/**
 * @file app_settings.h
 * @brief Persistenz von Fenster- und Wetterstations-Einstellungen via QSettings.
 */

#pragma once

#include <QObject>

class QWindow;
class WeatherStation;

/**
 * @brief Lädt und speichert Anwendungseinstellungen.
 *
 * @details
 * Gespeichert werden Positionsdaten des Fensters sowie Wetterstations-Parameter
 * wie Ort, Koordinaten und die Sekundenanzeige.
 */
class AppSettings : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Erstellt eine neue AppSettings-Instanz.
     * @param parent Optionales Elternobjekt im Qt-Objektbaum.
     */
    explicit AppSettings(QObject *parent = nullptr);

    /**
     * @brief Lädt Einstellungen aus persistentem Speicher.
     * @param weatherStation Zielobjekt für Wetterstations-Einstellungen.
     * @param window Zielobjekt für Fensterposition.
     */
    Q_INVOKABLE void load(WeatherStation *weatherStation, QWindow *window) const;
    /**
     * @brief Speichert Einstellungen in persistenten Speicher.
     * @param weatherStation Quellobjekt für Wetterstations-Einstellungen.
     * @param window Quellobjekt für Fensterposition.
     */
    Q_INVOKABLE void save(WeatherStation *weatherStation, QWindow *window) const;
};
