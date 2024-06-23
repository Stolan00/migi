import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import com.migi.models 1.0

Item {
    id: libraryContainer
    width: parent.width
    height: parent.height

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            currentIndex: 0

            Repeater {
                model: [
                    {text: "Currently Watching", index: 0},
                    {text: "Plan to Watch", index: 1},
                    {text: "Completed", index: 2},
                    {text: "Dropped", index: 3}
                ]
                delegate: TabButton {
                    text: modelData.text
                    background: Rectangle {
                        color: tabBar.currentIndex === modelData.index ? "lightslategray" : "slategray"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            tabBar.currentIndex = modelData.index
                            libraryContainer.switchTab(modelData.index)
                        }
                    }
                }
            }
        }

        AnimeList {
            id: animeLibraryList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    function switchTab(index) {
        animeLibraryList.setStatusFilter(index)
    }

    Component.onCompleted: {
        switchTab(0)
    }
}
