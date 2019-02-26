import QtQuick 2.7
import "../effects"

Item {
    id: closeButtonMenuBar

    property string name: ""
    property string description: ""
    property string backgroundColor: "#131313"
    signal clicked();

    implicitHeight: 25 * settings.pixelDensity

    Rectangle {
        id: header
    }

    Rectangle {
        anchors.fill: parent
        color: closeButtonMenuBar.backgroundColor

        Text{
            id: closeIcon
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5 * settings.pixelDensity
            font.pixelSize: 10 * settings.pixelDensity
            font.family: adventuresFontLoader.name
            color: "white"
            text: "X"
        }


        Text{
            id: name
            anchors.verticalCenter: closeIcon.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenterOffset: (closeButtonMenuBar.description === "") ? 0.5 * settings.pixelDensity : -2 * settings.pixelDensity
            font.pixelSize: (closeButtonMenuBar.description === "") ? 10 * settings.pixelDensity : 7 * settings.pixelDensity
            font.family: adventuresFontLoader.name
            color: "white"
            text: closeButtonMenuBar.name

        }

        Text {
            id: description
            anchors.top: name.bottom
            anchors.topMargin: 0 * settings.pixelDensity
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: adventuresFontLoader.name
            font.pixelSize: 5 * settings.pixelDensity
            color: "white"
            text: closeButtonMenuBar.description
            visible: (closeButtonMenuBar.description === "") ? false: true

        }

        RippleEffect{
            anchors.fill: parent
            onAnimationDone: closeButtonMenuBar.clicked();
        }
    }
}
