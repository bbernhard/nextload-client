import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import com.nextloadclient.nextloadclient 1.0
import "../basiccomponents"

BlankScreen {
    id: downloadScreen

    QtObject {
        id: internal
        property string currentRequest: "";
    }

    HttpsRequestWorkerThread {
        id: restAPI
        signal uploadTask(string url);
        signal nextloadFolderExists();

        onUploadTask: {
            ConnectionSettings.setBaseUrl(settings.url);
            loadingIndicator.visible = true;
            internal.currentRequest = "upload";
            var uploadTaskRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; UploadRequest{}',
                                                              restAPI);
            var entry = "url: " + url + "\n" + "format: " + checkboxes.checkedButton.text;
            var filename = new Date().getTime() + ".yml";
            uploadTaskRequest.set(entry, filename);
            uploadTaskRequest.setToken(settings.token);
            restAPI.put(uploadTaskRequest);
        }

        onNextloadFolderExists: {
            ConnectionSettings.setBaseUrl(settings.url);
            internal.currentRequest = "checkfolder";
            loadingIndicator.visible = true;
            var testConnectionRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; ListFolderContentsRequest{}',
                                                              restAPI);
            testConnectionRequest.setFolderName("nextload");
            testConnectionRequest.setToken(settings.token);
            restAPI.propfind(testConnectionRequest);
        }
    }

    Connections {
        target: restAPI
        onResultReady: {
            if(internal.currentRequest === "upload") {
                if(errorCode === 0) {
                    loadingIndicator.visible = false;
                    toast.show(qsTr("Success"));
                } else { //couldn't upload task. Check whether folder exists
                    restAPI.nextloadFolderExists();
                }
            } else if(internal.currentRequest == "checkfolder") { //upload request wasn't successful, so we now check
                //if the nextload folder exists. If it doesn't exist, the server application is probably not running
                //(or someone accidentally deleted the folder)
                loadingIndicator.visible = false;
                if(errorCode === 0) {
                    toast.show(qsTr("Couldn't process request - unknown error"));
                } else {
                    toast.show(qsTr("Couldn't find nextload folder."));
                }
            }
        }
    }

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

        MaterialButton {
            anchors.right: parent.right
            anchors.rightMargin: 0.2 * settings.pixelDensity
            anchors.verticalCenter: parent.verticalCenter
            text: "\ue5d4"
            highlightColor: "black"
            onClicked: {
                menu.open();
            }
        }
    }


    TextField {
        id: downloadUrl
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Material.accent: "#eb9486"
        Material.theme: Material.Dark
        placeholderText: qsTr("Enter Url")
        font.pixelSize: 6 * settings.pixelDensity
        width: parent.width - 20 * settings.pixelDensity
        horizontalAlignment: Text.AlignHCenter
        onTextEdited: {
            if(downloadUrl.text === "")
                downloadButton.enabled = false;
            else
                downloadButton.enabled = true;
        }
    }

    ButtonGroup {
        id: checkboxes
        buttons: formats.children
    }


    RowLayout {
        id: formats
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 5 * settings.pixelDensity
        anchors.top: downloadUrl.bottom
        width: parent.width


        RadioButton {
            checked: true
            text: qsTr("mp3")
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 4 * settings.pixelDensity
        }

        RadioButton {
            text: qsTr("ogg")
            enabled: false
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 4 * settings.pixelDensity
        }

        RadioButton {
            text: qsTr("webm")
            enabled: false
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 4 * settings.pixelDensity
        }

        RadioButton {
            text: qsTr("flv")
            enabled: false
            Material.theme: Material.Dark
            Material.accent: "#eb9486"
            font.pixelSize: 4 * settings.pixelDensity
        }
    }

    Menu {
        id: menu
        y: header.y + header.height
        x: parent.width - menu.width
        Material.theme: Material.Dark

        MenuItem {
            text: "Disconnect"
            Material.theme: Material.Dark
            font.pixelSize: 6 * settings.pixelDensity

            onClicked: {
                settings.token = "";
                settingsStorage.url = "";
            }
        }
    }

    Button {
        id: downloadButton
        enabled: false
        anchors.top: formats.bottom
        anchors.topMargin: 10 * settings.pixelDensity
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        Material.background: "#f3de8a"
        font.pixelSize: 6 * settings.pixelDensity
        height: 20 * settings.pixelDensity
        text: qsTr("GO")
        onClicked: {
            restAPI.uploadTask(downloadUrl.text);
        }
    }

    Toast {
        id: toast
        anchors.top: downloadButton.bottom
        anchors.topMargin: 5 * settings.pixelDensity
    }

    LoadingIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        visible: false
    }
}
