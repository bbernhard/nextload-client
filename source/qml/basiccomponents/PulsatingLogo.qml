import QtQuick 2.12
import QtGraphicalEffects 1.0

Item {
    id: pulsatingLogo
    signal pulsatingStopped()
    signal pulsatingStarted()
    //implicitWidth: 480
    //implicitHeight: 240
    property alias source: sourceImage.source

    function hide(delayed){
        if(delayed)
            timer.start();
        else{
            visible = false;
            zoomBlurAnimation.stop();
            pulsatingLogo.pulsatingStopped();
        }
    }

    function show(){
        zoomBlurAnimation.start();
        visible = true;
        pulsatingLogo.pulsatingStarted();
    }

    Timer{
        id: timer
        interval: 1000 //ms
        running: false
        repeat: false
        onTriggered: {
            visible = false;
            zoomBlurAnimation.stop();
            pulsatingLogo.pulsatingStopped();
        }
    }

    Image {
        id: sourceImage
        source: "qrc:/images/logo2.png"
        anchors.fill: parent
        //anchors.centerIn: parent
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
        visible: false
        fillMode: Image.PreserveAspectFit
    }

    ZoomBlur {
        id: zoomBlur
        anchors.fill: sourceImage
        source: sourceImage
        samples: 24

        SequentialAnimation {
            id: zoomBlurAnimation
            running: true
            loops: Animation.Infinite
            NumberAnimation { target: zoomBlur; property: "length"; to: 7; duration: 500 }
            NumberAnimation { target: zoomBlur; property: "length"; to: 0; duration: 500 }
        }
    }
}
