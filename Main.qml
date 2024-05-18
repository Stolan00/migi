import QtQuick
import QtQuick.Controls
import AppSettings


Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    // Window {
    //     visible: true
    //     width: 300
    //     height: 300

    //     TextArea {
    //         id: tokenSubmit
    //         placeholderText: qsTr("ENTER TOKEN")
    //     }

    //     Button {
    //         onClicked: {
    //             fileWriter.writeFile("token.txt", tokenSubmit.text)
    //         }
    //     }
    // }

    Item {
        Image {
            id: anime_pic
        }

        // Text
        // {
        //     id: responseArea
        //     wrapMode: Text.WordWrap
        //     width: 600
        //     height: 600
        // }
        Flickable {
            width: 600
            height: 600
            anchors.leftMargin: 5
            anchors.left: anime_pic.right

            Text
            {
                id: responseArea
                wrapMode: Text.WordWrap
                width: parent.parent.width
                height: parent.parent.height
                //readOnly: true
            }
        }

        Button
        {
            text: "Send GraphQL Request"
            anchors.horizontalCenter: anime_pic.horizontalCenter
            anchors.top: anime_pic.bottom
            anchors.topMargin: 5
            onClicked:
            {
                anilist.getViewerId()
            }
        }
    }

    Connections
    {
        target: anilist
        function onResponseReceived(response)
        {
            console.log(response.data.media)
            responseArea.text = response.data.Media.description
            //responseArea.text = response['data']['media']['description']
            anime_pic.source = response.data.Media.coverImage.large
        }
    }
}
