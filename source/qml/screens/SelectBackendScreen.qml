import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

BlankScreen {
    id: selectBackendScreen

    Rectangle {
        anchors.fill: parent
        color: "#272838"
    }

    Pane{
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 20 * settings.pixelDensity
        background: Rectangle {
            anchors.fill: parent
            color: "#f3de8a"
        }

        Text {
            text: qsTr("NEXTLOAD")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12 * settings.pixelDensity
            font.family: headerFont.name
        }
    }

    Text{
        id: selectBackendText
        color: "white"
        text: qsTr("Which backend do you want to use?")
        font.pixelSize: 8 * settings.pixelDensity
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: header.bottom
        anchors.topMargin: 30 * settings.pixelDensity
    }

    ButtonGroup {
        id: checkboxes
        buttons: formats.children
    }


    ColumnLayout {
        id: formats
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: parent.width/2.7
        anchors.top: selectBackendText.bottom
        anchors.topMargin: 10 * settings.pixelDensity
        width: parent.width


        RadioButton {
            property string backend: "nextload-core";
            checked: true
            text: qsTr("nextload-core")
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 6 * settings.pixelDensity
        }

        RadioButton {
            property string backend: "ocDownloader";
            text: qsTr("ocDownloader")
            enabled: true
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 6 * settings.pixelDensity
        }
    }

    Button {
        id: doneButton
        anchors.top: formats.bottom
        anchors.topMargin: 20 * settings.pixelDensity
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        Material.background: "#f3de8a"
        font.pixelSize: 8 * settings.pixelDensity
        height: 20 * settings.pixelDensity
        text: qsTr("CONTINUE")
        onClicked: {
            stackView.replace(Qt.resolvedUrl("qrc:/source/qml/screens/SetupScreen.qml"));
            stackView.currentItem.connectSuccessful.connect(main.onSuccessfulConnect);
            stackView.currentItem.selectedBackend = checkboxes.checkedButton.backend;
        }
    }
}
