import QtQuick 2.12
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import com.nextloadclient.nextloadclient 1.0
import "../basiccomponents"
import "../items"

BlankScreen {
    id: downloadScreen

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

    Menu {
        id: menu
        y: header.y + header.height
        x: parent.width - menu.width
        Material.theme: Material.Dark
        width: { //set width to the menus largest item width
                var result = 0;
                var padding = 0;
                for (var i = 0; i < count; ++i) {
                    var item = itemAt(i);
                    result = Math.max(item.contentItem.implicitWidth, result);
                    padding = Math.max(item.padding, padding);
                }
                return result + padding * 2;
        }
        height: 14 * settings.pixelDensity

        MenuItem {
            text: qsTr("Disconnect")
            Material.theme: Material.Dark
            font.pixelSize: 7 * settings.pixelDensity

            onClicked: {
                settings.token = "";
                settingsStorage.url = "";
                settings.backend = "";
                stackView.replace(Qt.resolvedUrl("qrc:/source/qml/screens/SelectBackendScreen.qml"));
            }
        }
    }

    Timer {
        id: refreshTimer
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            if(tabBar.currentIndex === 1)
                swipeView.itemAt(swipeView.currentIndex).loadData(false);
        }
    }

    TabBar {
        id: tabBar
        anchors.top: header.bottom
        Material.background: "#f3de8a"
        Material.accent: "#eb9486"
        width: parent.width
        currentIndex: swipeView.currentIndex
        TabButton {
            text: qsTr("Download")
            font.pixelSize: 6 * settings.pixelDensity
        }
        Component.onCompleted: {
            if(settings.backend !== "nextload-core") {
                var item = Qt.createQmlObject('import QtQuick.Controls 2.5; TabButton{}',
                                                              tabBar);
                item.text = qsTr("Progress");
                item.font.pixelSize = 6 * settings.pixelDensity;
                tabBar.addItem(item);
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: tabBar.currentIndex

        DownloadItem {
            id: downloadItem
        }

        ProgressItem {
            id: progressItem
        }

        onCurrentIndexChanged: {
            swipeView.itemAt(currentIndex).isActive();
        }
    }
}
