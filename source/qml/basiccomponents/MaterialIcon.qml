import QtQuick 2.7

Text {
    signal clicked()

    id: icon
    font.family: materialDesignLoader.name
    font.pixelSize: 10 * settings.pixelDensity
    verticalAlignment: Text.AlignVCenter
    elide: Text.ElideRight
    color: "white"
    MouseArea{
        anchors.fill: parent
        onClicked: icon.clicked()
    }
}
