#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>

#include "weather_station.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    qmlRegisterType<WeatherStation>("Backend", 1, 0, "WeatherStation");

    QQmlApplicationEngine engine;
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
