import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import Backend 1.0

Window {
    id: settingsWindow
    width: 360
    height: 300
    minimumWidth: 360
    minimumHeight: 300
    maximumWidth: 360
    maximumHeight: 300
    visible: false
    modality: Qt.ApplicationModal
    title: "Einstellungen"
    flags: Qt.FramelessWindowHint | Qt.Window
    color: "transparent"

    property WeatherStation weatherStation
    property real defaultLat: 52.520007
    property real defaultLon: 13.404954
    property string defaultLocation: "Berlin"

    function parseCoordinate(textValue, fallback) {
        var normalized = textValue.trim()
        if (normalized.length === 0)
            return fallback

        normalized = normalized.replace(",", ".")
        var directValue = Number(normalized)
        if (!isNaN(directValue))
            return directValue

        var match = normalized.match(/-?\d+(?:\.\d+)?/)
        if (match && match.length > 0) {
            var extractedValue = Number(match[0])
            if (!isNaN(extractedValue))
                return extractedValue
        }

        return fallback
    }

    function syncFieldsFromSettings() {
        if (!settingsWindow.weatherStation)
            return

        locationField.text = settingsWindow.weatherStation.locationName
        latitudeField.text = settingsWindow.weatherStation.latitude.toFixed(6)
        longitudeField.text = settingsWindow.weatherStation.longitude.toFixed(6)
        showSecondsCheck.checked = settingsWindow.weatherStation.showSeconds
    }

    onVisibleChanged: {
        if (visible)
            syncFieldsFromSettings()
    }

    Rectangle {
        anchors.fill: parent
        radius: 18
        color: "transparent"

        Image {
            anchors.fill: parent
            source: "qrc:/images/weather_bg.jpg"
            fillMode: Image.PreserveAspectCrop
        }

        Rectangle {
            anchors.fill: parent
            radius: 18
            color: "#7a1f2d3a"
            border.color: "#55ffffff"
            border.width: 1
        }

        MouseArea {
            anchors.fill: parent
            onPressed: function(mouse) {
                settingsWindow.startSystemMove()
            }
        }

        Item {
            id: formRoot
            anchors.fill: parent
            anchors.margins: 14

            function applyAndClose() {
                if (!settingsWindow.weatherStation) {
                    settingsWindow.visible = false
                    return
                }

                var locationText = locationField.text.trim()
                var latitudeText = latitudeField.text.trim()
                var longitudeText = longitudeField.text.trim()

                var lat = settingsWindow.parseCoordinate(latitudeText, settingsWindow.weatherStation.latitude)
                var lon = settingsWindow.parseCoordinate(longitudeText, settingsWindow.weatherStation.longitude)

                settingsWindow.weatherStation.locationName =
                        locationText.length > 0 ? locationText : settingsWindow.defaultLocation
                settingsWindow.weatherStation.latitude = lat
                settingsWindow.weatherStation.longitude = lon
                settingsWindow.weatherStation.showSeconds = showSecondsCheck.checked
                settingsWindow.weatherStation.fetch()
                settingsWindow.visible = false
            }

            function discardAndClose() {
                settingsWindow.visible = false
            }

            Rectangle {
                id: saveButton
                width: 28
                height: 28
                radius: 14
                anchors.top: parent.top
                anchors.left: parent.left
                color: saveMouse.containsMouse ? "#ccffffff" : "#99ffffff"
                border.color: "#40ffffff"
                border.width: 1
                z: 2

                Text {
                    anchors.centerIn: parent
                    text: "\u2713"
                    color: "#27313c"
                    font.pixelSize: 16
                    font.bold: true
                }

                MouseArea {
                    id: saveMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: function(mouse) {
                        mouse.accepted = true
                        formRoot.applyAndClose()
                    }
                }
            }

            Rectangle {
                id: cancelButton
                width: 28
                height: 28
                radius: 14
                anchors.top: parent.top
                anchors.right: parent.right
                color: cancelMouse.containsMouse ? "#ccffffff" : "#99ffffff"
                border.color: "#40ffffff"
                border.width: 1
                z: 2

                Text {
                    anchors.centerIn: parent
                    text: "\u2715"
                    color: "#27313c"
                    font.pixelSize: 15
                    font.bold: true
                }

                MouseArea {
                    id: cancelMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: function(mouse) {
                        mouse.accepted = true
                        formRoot.discardAndClose()
                    }
                }
            }

            ColumnLayout {
                anchors.fill: parent
                spacing: 10

                Text {
                    text: "Einstellungen"
                    color: "white"
                    font.pixelSize: 24
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                Item { Layout.preferredHeight: 4 }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Ort"
                        color: "#eef7ff"
                        font.pixelSize: 13
                    }

                    TextField {
                        id: locationField
                        Layout.fillWidth: true
                        placeholderText: "Berlin"

                        background: Rectangle {
                            radius: 10
                            color: "#88ffffff"
                            border.color: "#40ffffff"
                            border.width: 1
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: "Breitengrad"
                            color: "#eef7ff"
                            font.pixelSize: 13
                        }

                        TextField {
                            id: latitudeField
                            Layout.fillWidth: true
                            placeholderText: "52.52008"
                            validator: DoubleValidator { notation: DoubleValidator.StandardNotation }

                            background: Rectangle {
                                radius: 10
                                color: "#88ffffff"
                                border.color: "#40ffffff"
                                border.width: 1
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: "Längengrad"
                            color: "#eef7ff"
                            font.pixelSize: 13
                        }

                        TextField {
                            id: longitudeField
                            Layout.fillWidth: true
                            placeholderText: "13.404954"
                            validator: DoubleValidator { notation: DoubleValidator.StandardNotation }

                            background: Rectangle {
                                radius: 10
                                color: "#88ffffff"
                                border.color: "#40ffffff"
                                border.width: 1
                            }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 4

                    Text {
                        text: "Datenlieferant"
                        color: "#eef7ff"
                        font.pixelSize: 13
                    }

                    ComboBox {
                        Layout.fillWidth: true
                        model: [settingsWindow.weatherStation ? settingsWindow.weatherStation.dataProvider : "Open-Meteo"]
                        currentIndex: 0
                        enabled: false

                        background: Rectangle {
                            radius: 10
                            color: "#88ffffff"
                            border.color: "#40ffffff"
                            border.width: 1
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Text {
                        text: "Sekunden anzeigen"
                        color: "#eef7ff"
                        font.pixelSize: 13
                        Layout.fillWidth: true
                    }

                    CheckBox {
                        id: showSecondsCheck
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }
}
