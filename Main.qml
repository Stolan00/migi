import QtQuick 2.15
import QtQuick.Controls 2.15
import AppSettings 1.0
import "./src/QML"

Window {
    width: 800
    height: 480
    visible: true
    title: qsTr("Migi")

    // Sidebar Component
    Sidebar {
        id: sidebar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 200
    }

    Item {
        anchors.left: sidebar.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Image {
            id: anime_pic
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Flickable {
            width: parent.width - 220
            height: 300
            anchors.top: anime_pic.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 10

            Text {
                id: responseArea
                wrapMode: Text.WordWrap
                width: parent.width
                height: parent.height
            }
        }

        Button {
            text: "Send GraphQL Request"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: responseArea.bottom
            anchors.topMargin: 5
            onClicked: {
                anilist.populateDatabase()
            }
        }
    }

    Connections {
        target: anilist
        function onResponseReceived(response) {
            console.log(response.data.media)
            responseArea.text = response.data.Media.description
            anime_pic.source = response.data.Media.coverImage.large
        }
    }
}
