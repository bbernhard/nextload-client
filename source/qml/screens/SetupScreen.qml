import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import com.nextloadclient.nextloadclient 1.0
import "../basiccomponents"

BlankScreen {
    id: setupScreen
    objectName: "SetupScreen"
    property string selectedBackend: "";
    signal connectSuccessful();

    HttpsRequestWorkerThread {
        id: restAPI
        signal testConnection(string url, string token);

        onTestConnection: {
            loadingIndicator.visible = true;
            ConnectionSettings.setBaseUrl(url);
            var testConnectionRequest;
            if(setupScreen.selectedBackend === "nextload-core") {
                testConnectionRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; ListFolderContentsRequest{}',
                                                              restAPI);
                testConnectionRequest.setFolderName("");
                testConnectionRequest.setToken(token);
                restAPI.get(testConnectionRequest);
            } else if(setupScreen.selectedBackend === "ocDownloader") {
                testConnectionRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; OcDownloaderVersionRequest{}',
                                                           restAPI);
                testConnectionRequest.setToken(token);
                restAPI.post(testConnectionRequest);
            }
        }
    }

    Connections {
        target: restAPI
        onResultReady: {
            if(errorCode === 0) {
                var testConnectionSuccessful = false;
                if(setupScreen.selectedBackend === "nextload-core")
                    testConnectionSuccessful = true;
                else if(setupScreen.selectedBackend === "ocDownloader") {
                    if(result === "") {
                        toast.show(qsTr("Couldn't connect, please make sure ocDownloader is installed and enabled!"));
                        loadingIndicator.visible = false;
                    } else {
                        try {
                            var jsonRes = JSON.parse(result);
                            if ("RESULT" in jsonRes && jsonRes["RESULT"]) {
                                testConnectionSuccessful = true;
                            }
                            else {
                                toast.show(qsTr("Couldn't connect, please make sure ocDownloader is installed and enabled!"));
                                loadingIndicator.visible = false;
                            }
                        } catch(error) {
                            toast.show(qsTr("Couldn't connect, please make sure ocDownloader is installed and enabled!"));
                            loadingIndicator.visible = false;
                        }
                    }
                }

                if(testConnectionSuccessful) {
                    settings.token = nextcloudToken.text;
                    settingsStorage.backend = setupScreen.selectedBackend;
                    settings.backend = settingsStorage.backend;
                    settingsStorage.url = nextcloudUrl.text;
                    loadingIndicator.visible = false;
                    setupScreen.connectSuccessful();
                }
            } else {
                toast.show(qsTr("Couldn't connect, please check URL and token"));
                loadingIndicator.visible = false;
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
    }

    Text {
        id: selectedBackendStaticText
        text: qsTr("Backend")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: header.bottom
        anchors.topMargin: 20 * settings.pixelDensity
        font.pixelSize: 4 * settings.pixelDensity
        color: "white"
    }

    Text {
        id: selectedBackend
        text: setupScreen.selectedBackend
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: selectedBackendStaticText.bottom
        font.pixelSize: 7 * settings.pixelDensity
        color: "white"
    }

    TextField {
        id: nextcloudUrl
        anchors.top: selectedBackend.bottom
        anchors.topMargin: parent.width/3
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 10 * settings.pixelDensity
        placeholderText: qsTr("Enter URL")
        Material.accent: "#eb9486"
        Material.theme: Material.Dark
        font.pixelSize: 6 * settings.pixelDensity
    }

    TextField {
        id: nextcloudToken
        anchors.top: nextcloudUrl.bottom
        anchors.topMargin: 10 * settings.pixelDensity
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 10 * settings.pixelDensity
        Material.accent: "#eb9486"
        Material.theme: Material.Dark
        placeholderText: qsTr("Enter Token")
        font.pixelSize: 6 * settings.pixelDensity
    }

    Button {
        id: doneButton
        anchors.top: nextcloudToken.bottom
        anchors.topMargin: 10 * settings.pixelDensity
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        Material.background: "#f3de8a"
        font.pixelSize: 8 * settings.pixelDensity
        height: 20 * settings.pixelDensity
        text: qsTr("CONNECT")
        onClicked: {
            restAPI.testConnection(nextcloudUrl.text, nextcloudToken.text);
        }
    }

    Toast {
        id: toast
        anchors.top: doneButton.bottom
        anchors.topMargin: 5 * settings.pixelDensity
    }

    LoadingIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        visible: false
    }
}
