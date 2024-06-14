import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: iconButton
    width: parent.width
    height: 50
    color: selected ? "lightblue" : "transparent"

    property alias iconSource: icon.source
    property alias text: label.text
    property bool selected: false
    property int index: -1

    signal clicked

    Row {
        anchors.fill: parent
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id: icon
            width: 24
            height: 24
        }

        Label {
            id: label
            anchors.verticalCenter: parent.verticalCenter
            color: "black"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: iconButton.clicked()
    }
}
