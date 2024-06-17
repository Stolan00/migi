import QtQuick 2.15
import QtQuick.Controls 2.15
import "."

Item {
    width: 200
    height: parent.height

    property int selectedIndex: 1

    Column {
        anchors.fill: parent
        spacing: 10

        IconButton {
            iconSource: "images/icon1.png"
            text: "Home"
            index: 0
            selected: index == selectedIndex

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    selectedIndex = parent.index
                }
            }
        }

        IconButton {
            iconSource: "images/icon2.png"
            text: "Library"
            index: 1
            selected: index == selectedIndex

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    selectedIndex = parent.index
                }
            }
        }

        IconButton {
            iconSource: "images/icon3.png"
            text: "Settings"
            index: 2
            selected: index == selectedIndex

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    selectedIndex = parent.index
                }
            }
        }

        IconButton {
            iconSource: "images/icon4.png"
            text: "Profile"
            index: 3
            selected: index == selectedIndex

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    selectedIndex = parent.index
                }
            }
        }

        IconButton {
            iconSource: "images/icon5.png"
            text: "Help"
            index: 4
            selected: index == selectedIndex

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    selectedIndex = parent.index
                }
            }
        }
    }
}
