import QtQuick 2.7

ShaderEffect {
    id: shaderEffect
    signal animationDone()
    signal animationStarted()
    property bool preventMultipleClicks: false;

    // Properties that will get bound to a uniform with the same name in the shader
    property color backgroundColor: "transparent"
    property color spreadColor: "#20000000"
    property point normTouchPos
    property real widthToHeightRatio: height / width
    // Our animated uniform property
    property real spread: 0
    opacity: 0

    ParallelAnimation {
        id: touchStartAnimation
        UniformAnimator {
            uniform: "spread"; target: shaderEffect
            from: 0; to: 1
            duration: 500; easing.type: Easing.InQuad
        }
        OpacityAnimator {
            target: shaderEffect
            from: 0; to: 1
            duration: 50; easing.type: Easing.InQuad
        }
    }

    ParallelAnimation {
        id: touchEndAnimation
        UniformAnimator {
            uniform: "spread"; target: shaderEffect
            from: shaderEffect.spread; to: 1
            duration: 500; easing.type: Easing.OutQuad
        }
        OpacityAnimator {
            target: shaderEffect
            from: 1; to: 0
            duration: 500; easing.type: Easing.OutQuad
        }
        onRunningChanged: {
            if(!running) shaderEffect.animationDone();
        }
    }

    fragmentShader: "
            varying mediump vec2 qt_TexCoord0;
            uniform lowp float qt_Opacity;
            uniform lowp vec4 backgroundColor;
            uniform lowp vec4 spreadColor;
            uniform mediump vec2 normTouchPos;
            uniform mediump float widthToHeightRatio;
            uniform mediump float spread;

            void main() {
                // Pin the touched position of the circle by moving the center as
                // the radius grows. Both left and right ends of the circle should
                // touch the item edges simultaneously.
                mediump float radius = (0.5 + abs(0.5 - normTouchPos.x)) * 1.0 * spread;
                mediump vec2 circleCenter =
                    normTouchPos + (vec2(0.5) - normTouchPos) * radius * 2.0;

                // Calculate everything according to the x-axis assuming that
                // the overlay is horizontal or square. Keep the aspect for the
                // y-axis since we're dealing with 0..1 coordinates.
                mediump float circleX = (qt_TexCoord0.x - circleCenter.x);
                mediump float circleY = (qt_TexCoord0.y - circleCenter.y) * widthToHeightRatio;

                // Use step to apply the color only if x2*y2 < r2.
                lowp vec4 tapOverlay =
                    spreadColor * step(circleX*circleX + circleY*circleY, radius*radius);
                gl_FragColor = (backgroundColor + tapOverlay) * qt_Opacity;
            }
        "

    function touchStart(x, y) {
        normTouchPos = Qt.point(x / width, y / height)
        touchEndAnimation.stop()
        touchStartAnimation.start()
    }
    function touchEnd() {
        touchStartAnimation.stop()
        touchEndAnimation.start()
    }

    Timer {
        id: touchEndTimer
        interval: 125
        onTriggered: {
            shaderEffect.animationStarted();
            shaderEffect.touchEnd();
        }
    }
    MouseArea{
        id: mouseArea
        property bool ready: true;
        anchors.fill: parent
        onClicked: {
            if(mouseArea.ready){
                shaderEffect.touchStart(width*0.8, height*0.66);
                touchEndTimer.start();
                if(shaderEffect.preventMultipleClicks){
                    mouseArea.ready = false;
                    readyTimer.start();
                }

            }
        }
    }

    Timer{
        id: readyTimer
        running: false
        interval: 1000
        repeat: false
        onTriggered: mouseArea.ready = true;
    }
}
