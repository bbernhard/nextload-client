import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import com.nextloadclient.nextloadclient 1.0
import "../basiccomponents"

BlankItem {
    HttpsRequestWorkerThread {
        id: restAPI
        signal listTasks();

        onListTasks: {
            ConnectionSettings.setBaseUrl(settings.url);
            loadingIndicator.visible = true;
            listTasksModel.clear();


            var listTasksRequest;
            if(settings.backend === "nextload-core") {
                console.log("not implemented");
            } else if(settings.backend === "ocDownloader") {
                listTasksRequest = Qt.createQmlObject('import com.nextloadclient.nextloadclient 1.0; OcDownloaderListRequest{}',
                                                                  restAPI);
                listTasksRequest.setToken(settings.token);
                restAPI.get(listTasksRequest);
            } else {
                console.log("invalid backend");
            }
        }
    }

    function populate(elements) {
        listTasksModel.clear();
        for(var i = 0; i < elements.length; i++) {
            var fileProgress = "unknown";
            if(("PROGRESS" in elements[i]) && ("ProgressString" in elements[i]["PROGRESS"]) && elements[i]["PROGRESS"]["ProgressString"] !== null)
                fileProgress = elements[i]["PROGRESS"]["ProgressString"];
            listTasksModel.append({"filename": elements[i]["FILENAME"], "fileProgress": fileProgress});
        }
    }

    Connections {
        target: restAPI
        onResultReady: {
            if(settings.backend === "nextload-core") {
                console.log("not implemented");
            } else if(settings.backend === "ocDownloader") {
                var jsonRes = JSON.parse(result);
                loadingIndicator.visible = false;
                if(errorCode === 0) {
                    if(jsonRes["ERROR"])
                        toast.show(qsTr("Error") + ":" + jsonRes["MESSAGE"]);
                    else
                        populate(jsonRes["QUEUE"]);
                } else {
                    if("message" in jsonRes)
                        toast.show(jsonRes["message"]);
                    else
                        toast.show(qsTr("Unknown error"));
                }
            } else {
                console.log("invalid backend");
            }
        }
    }

    onIsActive: {
        restAPI.listTasks();
    }

    Rectangle {
        anchors.fill: parent
        color: "#272838"
    }

    ListModel {
        id: listTasksModel
    }

    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.topMargin: 10 * settings.pixelDensity
        model: listTasksModel
        delegate: Item {
            width: parent.width
            height: 20 * settings.pixelDensity
            Text {
                id: filenameText
                text: filename
                anchors.left: parent.left
                anchors.leftMargin: 15 * settings.pixelDensity
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 4.5 * settings.pixelDensity
                color: "white"
            }
            Text {
                id: progressText
                text: fileProgress
                anchors.right: parent.right
                anchors.top: filenameText.bottom
                anchors.topMargin: 2 * settings.pixelDensity
                anchors.rightMargin: 12 * settings.pixelDensity
                font.pixelSize: 3 * settings.pixelDensity
                color: "white"
            }

            HorizontalSeparator {
                width: parent.width - (10 * settings.pixelDensity)
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                opacity: 0.5
            }
        }
    }

    Toast {
        id: toast
        anchors.bottom: loadingIndicator.top
        anchors.bottomMargin: 20 * settings.pixelDensity
    }

    LoadingIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        visible: false
    }
}
