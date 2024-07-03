import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import com.migi.models 1.0

Item {
    id: libraryContainer
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true
        spacing: 0  // Ensure no extra space between components

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40  // Set a fixed height for the TabBar
            z: 3  // Ensure TabBar is above everything else

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
            Layout.topMargin: 5  // Add a small margin below the TabBar
        }
    }

    function switchTab(index) {
        animeLibraryList.setStatusFilter(index)

    }

    Component.onCompleted: {
        switchTab(0)
    }
}
