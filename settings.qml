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
    color: "#1f2833"

    property WeatherStation weatherStation

    Rectangle {
        anchors.fill: parent
        color: "#1f2833"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            spacing: 10

            Label {
                text: "Einstellungen"
                color: "white"
                font.pixelSize: 20
                font.bold: true
            }

            Label {
                text: "Ort"
                color: "#d6e2f0"
            }
            TextField {
                id: locationField
                Layout.fillWidth: true
                text: settingsWindow.weatherStation ? settingsWindow.weatherStation.locationName : ""
                placeholderText: "Berlin (52.520008, 13.404954)"
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                TextField {
                    id: latitudeField
                    Layout.fillWidth: true
                    text: settingsWindow.weatherStation ? settingsWindow.weatherStation.latitude.toFixed(6) : ""
                    placeholderText: "Breitengrad"
                    validator: DoubleValidator { notation: DoubleValidator.StandardNotation }
                }

                TextField {
                    id: longitudeField
                    Layout.fillWidth: true
                    text: settingsWindow.weatherStation ? settingsWindow.weatherStation.longitude.toFixed(6) : ""
                    placeholderText: "Längengrad"
                    validator: DoubleValidator { notation: DoubleValidator.StandardNotation }
                }
            }

            Label {
                text: "Datenlieferant"
                color: "#d6e2f0"
            }
            ComboBox {
                Layout.fillWidth: true
                model: [settingsWindow.weatherStation ? settingsWindow.weatherStation.dataProvider : "Open-Meteo"]
                currentIndex: 0
                enabled: false
            }

            CheckBox {
                id: showSecondsCheck
                text: "Uhrzeit mit Sekunden anzeigen"
                checked: settingsWindow.weatherStation ? settingsWindow.weatherStation.showSeconds : true
                palette.text: "white"
            }

            Item { Layout.fillHeight: true }

            Button {
                Layout.alignment: Qt.AlignRight
                text: "Bestätigen & Schließen"
                onClicked: {
                    if (!settingsWindow.weatherStation) {
                        return
                    }

                    var lat = Number(latitudeField.text)
                    var lon = Number(longitudeField.text)
                    if (isNaN(lat) || isNaN(lon)) {
                        return
                    }

                    settingsWindow.weatherStation.locationName = locationField.text.length > 0 ? locationField.text : "Berlin"
                    settingsWindow.weatherStation.latitude = lat
                    settingsWindow.weatherStation.longitude = lon
                    settingsWindow.weatherStation.showSeconds = showSecondsCheck.checked
                    settingsWindow.weatherStation.fetch()
                    settingsWindow.close()
                }
            }
        }
    }
}
