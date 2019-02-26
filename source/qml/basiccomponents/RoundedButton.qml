import QtQuick 2.7
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

Item{
    id: roundedButton
    signal pressed()

    implicitHeight: 14 * settings.pixelDensity
    property alias text: label.text
    property string backgroundColor: "transparent"
    property string borderColor: "white"
    property string labelColor: "white"
    property int fontSize: 6 * settings.pixelDensity
    property int radius: 30 * settings.pixelDensity
    property string fontFamily: "Roboto"

    Rectangle {
        id: background
        z: 1
        anchors.fill: parent
        color: "white"
        opacity: 0.2
        visible: mouseArea.pressed
        radius: roundedButton.radius
    }


    Rectangle {
        anchors.fill: parent
        border.width: Math.max(1, Math.round(0.3 * settings.pixelDensity))
        border.color: borderColor
        radius: roundedButton.radius
        color: backgroundColor
        Text {
            id: label
            anchors.fill: parent
            anchors.leftMargin: 4 * settings.pixelDensity
            anchors.rightMargin: 4 * settings.pixelDensity
            font.family: roundedButton.fontFamily
            font.pixelSize: roundedButton.fontSize
            color: labelColor
            clip: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            roundedButton.pressed() //emit signal
        }
    }
}
