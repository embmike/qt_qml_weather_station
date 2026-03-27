import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Backend 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 400
    height: 300
    minimumWidth: 400
    maximumWidth: 400
    minimumHeight: 300
    maximumHeight: 300

    title: "Wetterstation"

    flags: Qt.FramelessWindowHint | Qt.Window
    color: "transparent"

    property var deLocale: Qt.locale("de_DE")
    property date now: new Date()
    property string weatherError: ""

    AppSettings {
        id: appSettings
    }

    WeatherStation {
        id: weatherStation
        onDataChanged: weatherError = ""
        onErrorOccurred: function(message) {
            weatherError = message
        }
    }

    Timer {
        interval: 1000
        running: true
        repeat: true
        onTriggered: root.now = new Date()
    }

    Timer {
        interval: 600000
        running: true
        repeat: true
        onTriggered: weatherStation.fetch()
    }

    Component.onCompleted: {
        appSettings.load(weatherStation, root)
        weatherStation.fetch()
    }

    onClosing: function(closeEvent) {
        appSettings.save(weatherStation, root)
    }

    Loader {
        id: settingsWindowLoader
        active: true
        source: "settings.qml"
        onLoaded: {
            if (item) {
                item.weatherStation = weatherStation
            }
        }
    }

    background: Item {
        Image {
            anchors.fill: parent
            source: "qrc:/images/weather_bg.jpg"
            fillMode: Image.PreserveAspectCrop
        }

        Rectangle {
            anchors.fill: parent
            radius: 18
            color: "#66000000"
            border.color: "#55ffffff"
            border.width: 1
        }
    }

    Rectangle {
        anchors.fill: parent
        anchors.margins: 10
        radius: 18
        color: "transparent"

        MouseArea {
            anchors.fill: parent
            onPressed: function(mouse) {
                root.startSystemMove()
            }
        }

        Item {
            anchors.fill: parent
            anchors.margins: 16

            Rectangle {
                id: settingsButton
                width: 28
                height: 28
                radius: 14
                anchors.top: parent.top
                anchors.left: parent.left
                color: settingsMouse.containsMouse ? "#c0a0a0a0" : "#90909090"
                border.color: "#40ffffff"
                border.width: 1
                z: 2

                Text {
                    anchors.centerIn: parent
                    text: "⚙"
                    color: "#e5e5e5"
                    font.pixelSize: 16
                }

                MouseArea {
                    id: settingsMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (settingsWindowLoader.item) {
                            settingsWindowLoader.item.show()
                            settingsWindowLoader.item.requestActivate()
                        }
                    }
                }
            }

            Rectangle {
                id: closeButton
                width: 28
                height: 28
                radius: 14
                anchors.top: parent.top
                anchors.right: parent.right
                color: closeMouse.containsMouse ? "#ccffffff" : "#99ffffff"
                border.color: "#40ffffff"
                border.width: 1
                z: 2

                Text {
                    anchors.centerIn: parent
                    text: "\uD83D\uDEAA"
                    font.pixelSize: 16
                }

                MouseArea {
                    id: closeMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: Qt.quit()
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.topMargin: 4
                spacing: 10

                Text {
                    text: "Wetterstation"
                    color: "white"
                    font.pixelSize: 24
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: weatherStation.locationName
                    color: "#e8f3ff"
                    font.pixelSize: 16
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: root.deLocale.toString(root.now, "ddd, dd.MM.yyyy")
                    color: "#e8f3ff"
                    font.pixelSize: 18
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    text: Qt.formatTime(root.now, weatherStation.showSeconds ? "HH:mm:ss" : "HH:mm")
                    color: "white"
                    font.pixelSize: 34
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Text {
                    visible: root.weatherError.length > 0
                    text: root.weatherError
                    color: "#ffd2d2"
                    font.pixelSize: 11
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 82
                        Layout.preferredWidth: 1
                        radius: 14
                        color: "#55ffffff"
                        border.color: "#40ffffff"
                        border.width: 1

                        Column {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4

                            Text {
                                width: parent.width
                                text: "Temperatur"
                                color: "#eef7ff"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                width: parent.width
                                text: Number(weatherStation.temperature).toLocaleString(root.deLocale, 'f', 1) + " °C"
                                color: "white"
                                font.pixelSize: 22
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 82
                        Layout.preferredWidth: 1
                        radius: 14
                        color: "#55ffffff"
                        border.color: "#40ffffff"
                        border.width: 1

                        Column {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4

                            Text {
                                width: parent.width
                                text: "Feuchte"
                                color: "#eef7ff"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                width: parent.width
                                text: Math.round(weatherStation.humidity) + " %"
                                color: "white"
                                font.pixelSize: 22
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 82
                        Layout.preferredWidth: 1.6
                        radius: 14
                        color: "#55ffffff"
                        border.color: "#40ffffff"
                        border.width: 1

                        Column {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4

                            Text {
                                width: parent.width
                                text: "Druck"
                                color: "#eef7ff"
                                font.pixelSize: 14
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Text {
                                width: parent.width
                                text: Number(weatherStation.pressure).toLocaleString(root.deLocale, 'f', 1) + " hPa"
                                color: "white"
                                font.pixelSize: 22
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }
}
