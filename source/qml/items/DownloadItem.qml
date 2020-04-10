import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import com.nextloadclient.nextloadclient 1.0
import "../basiccomponents"

BlankItem {
    QtObject {
        id: internal
        property string currentRequest: "";
    }

    HttpsRequestWorkerThread {
        id: restAPI
        signal addDownloadTask(string url);
        signal nextloadFolderExists();

        onAddDownloadTask: {
            ConnectionSettings.setBaseUrl(settings.url);
            loadingIndicator.visible = true;

            var addDownloadTaskRequest;
            if(settings.backend === "nextload-core") {
                internal.currentRequest = "upload";
                addDownloadTaskRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; UploadRequest{}',
                                                                  restAPI);
                var entry = "url: " + url + "\n" + "format: " + checkboxes.checkedButton.text;
                var filename = new Date().getTime() + ".yml";
                addDownloadTaskRequest.set(entry, filename);
                addDownloadTaskRequest.setToken(settings.token);
                restAPI.put(addDownloadTaskRequest);
            } else if(settings.backend === "ocDownloader") {
                addDownloadTaskRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; OcDownloaderAddRequest{}',
                                                                  restAPI);
                addDownloadTaskRequest.setDownloadUrl(url);
                addDownloadTaskRequest.setToken(settings.token);
                restAPI.post(addDownloadTaskRequest);
            } else {
                console.log("invalid backend")
            }
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
            if(settings.backend === "nextload-core") {
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
            } else if(settings.backend === "ocDownloader") {
                var jsonRes = JSON.parse(result);
                loadingIndicator.visible = false;
                if(errorCode === 0) {
                    if(jsonRes["ERROR"])
                        toast.show(qsTr("Error") + ":" + jsonRes["MESSAGE"]);
                    else
                        toast.show(qsTr("Success"));
                } else {
                    toast.show(qsTr("Unknown error"));
                }
            } else {
                console.log("invalid backend");
            }
        }
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#272838"
    }


    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("&Paste")
            enabled: downloadUrl.canPaste
            onTriggered: downloadUrl.paste()
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
        selectByMouse: true
        onTextEdited: {
            if(downloadUrl.text === "")
                downloadButton.enabled = false;
            else
                downloadButton.enabled = true;
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.RightButton
            onClicked: {
                var g = mapToGlobal(mouse.x, mouse.y);
                contextMenu.x = background.width/2;
                contextMenu.y = g.y - header.height - tabBar.height;
                contextMenu.open()
            }
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
        visible: (settings.backend === "ocDownloader") ? false : true


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
            restAPI.addDownloadTask(downloadUrl.text);
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
