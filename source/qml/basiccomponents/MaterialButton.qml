import QtQuick 2.7
import "../effects"

Item {
    id: materialButton
    signal clicked()
    property alias text: icon.text
    property alias font: icon.font
    property int margins: 0
    property string backgroundColor: "transparent"
    property string highlightColor: "white"
    property bool round: false

    implicitWidth: container.width

    Rectangle{
        id: container
        width: icon.contentWidth + materialButton.margins
        height: icon.contentHeight + materialButton.margins
        anchors.centerIn: parent
        radius: materialButton.round ? width/2 : 0
        color: mouseArea.pressed ? materialButton.highlightColor : materialButton.backgroundColor

        Text {
            id: icon
            anchors.centerIn: parent
            font.family: materialDesignLoader.name
            font.pixelSize: 10 * settings.pixelDensity
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            color: materialButton.highlightColor
        }

        RippleEffect{
            id: mouseArea
            anchors.fill: parent
            onAnimationStarted: materialButton.clicked()
        }
    }
}
