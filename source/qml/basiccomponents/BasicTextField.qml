import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.0

Item {
    id: basicTextField

    property alias text: textInput.text
    property alias placeholder: placeholderTextInput.text
    property alias validator: textInput.validator
    property alias echomode: textInput.echoMode
    property string textColor: colorPalette.secondaryColor
    property string placeholderTextColor: colorPalette.secondaryColorLight
    property string cursorColor: colorPalette.secondaryColorLight
    property alias selectByMouse: textInput.selectByMouse
    property alias icon: icon.text
    property string iconColor: colorPalette.secondaryColor
    property int inputMethodHints: Qt.ImhNoPredictiveText
    property bool passwordStrengthIndicator: false
    property int passwordScore: 0
    property var parentFlickable: null
    property int placeholderHorizontalAlignment: Text.AlignLeft

    implicitHeight: (passwordStrengthIndicator === false) ? 15 * settings.pixelDensity : 16 * settings.pixelDensity

    anchors.left: parent.left
    anchors.right: parent.right

    TextField {
        id: placeholderTextInput
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: icon.left
        anchors.bottom: parent.bottom
        //anchors.fill: parent
        anchors.leftMargin: 10 * settings.pixelDensity
        anchors.rightMargin: 4 * settings.pixelDensity
        font.family: "Roboto"
        font.pixelSize: 6 * settings.pixelDensity
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: basicTextField.placeholderHorizontalAlignment
        color: basicTextField.placeholderTextColor
        readOnly: true
        visible: textInput.length === 0
        Material.accent: basicTextField.cursorColor
    }

    MaterialIcon{
        id: icon
        anchors.verticalCenter: placeholderTextInput.verticalCenter
        anchors.verticalCenterOffset: -1 * settings.pixelDensity
        anchors.right: parent.right
        anchors.rightMargin: 10 * settings.pixelDensity
        font.pixelSize: 6 * settings.pixelDensity
        color: basicTextField.iconColor
    }

    TextField {
        id: textInput
        anchors.top: placeholderTextInput.top
        anchors.left: placeholderTextInput.left
        anchors.right: icon.left
        anchors.bottom: placeholderTextInput.bottom
        inputMethodHints: basicTextField.inputMethodHints
        activeFocusOnPress: true
        font.family: "Roboto"
        font.pixelSize: 6 * settings.pixelDensity
        verticalAlignment: Text.AlignVCenter
        color: basicTextField.textColor
        clip: true
        echoMode: TextInput.Normal
        Material.accent: basicTextField.cursorColor
        selectByMouse: true

        MouseArea {
            anchors.fill: parent
            onPressed: {
                textInput.forceActiveFocus();
                if (!Qt.inputMethod.visible) { //open virtual keyboard, when not already opened
                    Qt.inputMethod.show()
                }

                //position cursor on non-iOs devices
                if(Qt.platform.os !== "ios"){
                    var startPosition = textInput.positionAt(mouse.x, mouse.y);
                    textInput.cursorPosition = startPosition;
                }
            }
        }

    }

    Rectangle {
        id: separator
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textInput.bottom
        anchors.leftMargin: 5 * settings.pixelDensity
        anchors.rightMargin: 5 * settings.pixelDensity
        height: Math.max(1, Math.round(0.3 * settings.pixelDensity))
        color: colorPalette.secondaryColor
    }

    /*PasswordStrengthIndicator{
        id: passwordStrengthIndicator
        anchors.top: separator.bottom
        anchors.topMargin: 2 * settings.pixelDensity
        anchors.left: separator.left
        anchors.right: separator.right
        visible: basicTextField.passwordStrengthIndicator
        controlWidth: separator.width
        score: basicTextField.passwordScore
        height: (visible === false) ? 0 : undefined
    }*/

}
