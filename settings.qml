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
    property var saveSettingsCallback: null
    property string defaultLocation: "Berlin"
    property real defaultLat: 52.520007
    property real defaultLon: 13.404954

    property bool previewValid: true
    property bool previewPending: false
    property string previewLatText: ""
    property string previewLonText: ""

    function syncFieldsFromSettings() {
        if (!settingsWindow.weatherStation)
            return

        locationField.text = settingsWindow.weatherStation.locationName
        previewLatText = settingsWindow.weatherStation.latitude.toFixed(6)
        previewLonText = settingsWindow.weatherStation.longitude.toFixed(6)
        previewValid = true
        previewPending = false
        showSecondsCheck.checked = settingsWindow.weatherStation.showSeconds
    }

    function applyAndClose() {
        if (!settingsWindow.weatherStation) {
            settingsWindow.close()
            return
        }

        var locationText = locationField.text.trim()

        if (locationText.length === 0) {
            settingsWindow.weatherStation.locationName = settingsWindow.defaultLocation
            settingsWindow.weatherStation.latitude = settingsWindow.defaultLat
            settingsWindow.weatherStation.longitude = settingsWindow.defaultLon
            settingsWindow.weatherStation.showSeconds = showSecondsCheck.checked
            settingsWindow.weatherStation.fetch()
            if (settingsWindow.saveSettingsCallback)
                settingsWindow.saveSettingsCallback()
            settingsWindow.close()
            return
        }

        if (!previewValid || previewPending)
            return

        settingsWindow.weatherStation.locationName = locationText
        settingsWindow.weatherStation.latitude = Number(previewLatText)
        settingsWindow.weatherStation.longitude = Number(previewLonText)
        settingsWindow.weatherStation.showSeconds = showSecondsCheck.checked
        settingsWindow.weatherStation.fetch()
        if (settingsWindow.saveSettingsCallback)
            settingsWindow.saveSettingsCallback()
        settingsWindow.close()
    }

    function discardAndClose() {
        settingsWindow.close()
    }

    onVisibleChanged: {
        if (visible)
            syncFieldsFromSettings()
    }

    Connections {
        target: settingsWindow.weatherStation

        function onLocationLookupChanged() {
            if (!settingsWindow.visible || !settingsWindow.weatherStation)
                return

            previewPending = settingsWindow.weatherStation.locationLookupPending
            previewValid = settingsWindow.weatherStation.locationLookupValid

            if (previewValid) {
                previewLatText = settingsWindow.weatherStation.lookupLatitude.toFixed(6)
                previewLonText = settingsWindow.weatherStation.lookupLongitude.toFixed(6)
            } else {
                previewLatText = ""
                previewLonText = ""
            }
        }
    }

    Timer {
        id: geocodeTimer
        interval: 450
        repeat: false
        onTriggered: {
            if (!settingsWindow.weatherStation)
                return
            settingsWindow.weatherStation.resolveLocation(locationField.text)
        }
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
            anchors.fill: parent
            anchors.margins: 14

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
                        settingsWindow.applyAndClose()
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
                        settingsWindow.discardAndClose()
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
                        color: (!previewPending && locationField.text.trim().length > 0 && !previewValid) ? "#ff8b8b" : "#eef7ff"
                        font.pixelSize: 13
                    }

                    TextField {
                        id: locationField
                        Layout.fillWidth: true
                        placeholderText: "Berlin"
                        color: (!previewPending && text.trim().length > 0 && !previewValid) ? "#b00020" : "#27313c"
                        selectedTextColor: "white"
                        selectionColor: "#5a8fd8"

                        onTextEdited: {
                            previewPending = true
                            previewValid = true
                            previewLatText = ""
                            previewLonText = ""
                            geocodeTimer.restart()
                        }

                        background: Rectangle {
                            radius: 10
                            color: "#88ffffff"
                            border.color: (!previewPending && locationField.text.trim().length > 0 && !previewValid) ? "#ff8b8b" : "#40ffffff"
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
                            readOnly: true
                            text: previewLatText
                            placeholderText: ""
                            color: "#6e6e6e"

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
                            readOnly: true
                            text: previewLonText
                            placeholderText: ""
                            color: "#6e6e6e"

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

                        contentItem: Text {
                            leftPadding: 12
                            rightPadding: 12
                            text: parent.displayText
                            color: "#6e6e6e"
                            font.pixelSize: 14
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
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
                        Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                        leftPadding: 0
                        rightPadding: 0
                        topPadding: 0
                        bottomPadding: 0
                        spacing: 0

                        indicator: Rectangle {
                            x: showSecondsCheck.width - width
                            y: showSecondsCheck.height - height
                            implicitWidth: 16
                            implicitHeight: 16
                            radius: 3
                            border.color: "#40ffffff"
                            border.width: 1
                            color: "#ddffffff"

                            Rectangle {
                                anchors.centerIn: parent
                                width: 8
                                height: 8
                                radius: 2
                                color: "#f2a33a"
                                visible: showSecondsCheck.checked
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }
}
