/**
 * @file main.cpp
 * @brief Einstiegspunkt der QML-Wetterstations-Anwendung.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>

#include "app_settings.h"
#include "weather_station.h"

/**
 * @brief Einstiegspunkt der Anwendung.
 * @param argc Anzahl der Kommandozeilenargumente.
 * @param argv Kommandozeilenargumente.
 * @return Rückgabecode der Qt-Ereignisschleife.
 */
int main(int argc, char *argv[])
{
    // qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard")); Wird im Moment nicht verwendet.

    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("qt_qml_weather_station"));
    QCoreApplication::setApplicationName(QStringLiteral("qt_qml_weather_station"));

    qmlRegisterType<WeatherStation>("Backend", 1, 0, "WeatherStation");
    qmlRegisterType<AppSettings>("Backend", 1, 0, "AppSettings");

    QQmlApplicationEngine engine{};

    const QUrl url(QStringLiteral("qrc:/qt_qml_weather_station/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QCoreApplication::exec();
}
