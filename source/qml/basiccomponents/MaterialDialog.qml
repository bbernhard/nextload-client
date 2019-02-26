import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0


Popup {
    id: materialDialog
    signal firstButtonClicked()
    signal secondButtonClicked()
    property string headerText: "";
    property int headerTextSize: 6 * settings.pixelDensity;
    property int dialogWidth: 100;
    property string description: "";
    property int descriptionSize: 6 * settings.pixelDensity;
    property string firstButtonText: "";
    property string secondButtonText: "";
    property bool progressBar: false;
    width: materialDialog.dialogWidth + (10 * settings.pixelDensity) //parent.width
    height: (headerText.contentHeight + description.contentHeight +
                firstButton.height + (10 * settings.pixelDensity)) //parent.height
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    opacity: 0.8

    contentItem: Item {
        id: contentItem
        implicitWidth: parent.width
        implicitHeight: parent.height

        Text{
            id: headerText
            anchors.top: parent.top
            anchors.left: parent.left
            font.bold: true
            font.pixelSize: materialDialog.headerTextSize
            width: materialDialog.dialogWidth
            wrapMode: Text.WordWrap
            text: materialDialog.headerText
            visible: (materialDialog.headerText) === "" ? false : true
            height: (materialDialog.headerText === "") ? 3 * settings.pixelDensity: implicitHeight
        }

        LoadingIndicator{
            id: loadingIndicator
            visible: materialDialog.progressBar
            anchors.top: headerText.bottom
            anchors.left: parent.left
        }

        Text{
            id: description
            anchors.top: loadingIndicator.visible ? loadingIndicator.top : headerText.bottom
            anchors.topMargin: loadingIndicator.visible ? loadingIndicator.height/4 : 0
            anchors.left: loadingIndicator.visible ? loadingIndicator.right: parent.left
            anchors.leftMargin: loadingIndicator.visible ? 4 * settings.pixelDensity : 3 * settings.pixelDensity
            font.pixelSize: materialDialog.descriptionSize
            width: (loadingIndicator.visible) ?
                       (materialDialog.dialogWidth - loadingIndicator.width) : materialDialog.dialogWidth
            wrapMode: Text.WordWrap
            text: materialDialog.description
        }

        Button{
            id: firstButton
            visible: (materialDialog.firstButtonText === "") ? false: true
            text: materialDialog.firstButtonText
            flat: true
            font.pixelSize: 5 * settings.pixelDensity
            anchors.right: parent.right
            anchors.rightMargin: 4 * settings.pixelDensity
            anchors.bottom: parent.bottom
            onClicked: materialDialog.firstButtonClicked()
        }
        Button{
            id: secondButton
            visible: (materialDialog.secondButtonText === "") ? false: true
            text: materialDialog.secondButtonText
            flat: true
            font.pixelSize: 5 * settings.pixelDensity
            anchors.right: firstButton.left
            anchors.rightMargin: 10 * settings.pixelDensity
            anchors.bottom: parent.bottom
            onClicked: materialDialog.secondButtonClicked()
        }
    }
}
