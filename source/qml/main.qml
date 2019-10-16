import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import com.nextloadclient.nextloadclient 1.0
import "screens"

ApplicationWindow {
    id: main
    visible: true
    width: 640
    height: 800
    title: qsTr("nextload client")

    Component.onCompleted: {
        settings.pixelDensity = Screen.logicalPixelDensity //set pixel density
        if((settings.token === "") || (settings.url === "") || (settings.backend === "")) {
            stackView.replace(Qt.resolvedUrl("qrc:/source/qml/screens/SelectBackendScreen.qml"));
        } else {
            stackView.replace(Qt.resolvedUrl("qrc:/source/qml/screens/DownloadScreen.qml"));
        }
    }

    function onSuccessfulConnect() {
        stackView.replace(Qt.resolvedUrl("qrc:/source/qml/screens/DownloadScreen.qml"));
    }

    QtObject{
        id: settings
        property double pixelDensity
        property string token: keyChain.getToken()
        property string url: settingsStorage.url;
        property string backend: settingsStorage.backend;
        onTokenChanged: keyChain.setToken(settings.token);
    }

    Keychain{
        id: keyChain
        property alias token: settings.token
    }


    Settings {
        id: settingsStorage
        category: "General"
        property string url;
        property string backend;
    }

    StackView {
        id: stackView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        onCurrentItemChanged: {
            if(currentItem !== null)
                currentItem.isActive()
        }
    }

    QtObject {
        id: colorPalette
        property string backgroundColor: "#004847"//"#0a0a0a"
        property string foregroundColor: "#00ffff"
        property string secondaryColor: "orange" // "#D17D19"
        property string secondaryColorLight: "#027B87"
    }


    FontLoader{
        id: materialDesignLoader
        source: "../fonts/material-design-icons.ttf"
    }

    FontLoader{
        id: headerFont
        source: "../fonts/Elianto-Regular.ttf"
    }

    FontLoader{
        id: fontawesomeLoader
        source: "../fonts/fontawesome-webfont.ttf"
    }
}
